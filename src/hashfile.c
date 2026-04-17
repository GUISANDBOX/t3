#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashfile.h"

typedef struct {
    int globalDepth;
    int recordSize;
    int numBuckets;
    int bucketSize;
    long directoryOffset;
    long bucketDataOffset;
    int keyOffset;
    int keySize;
} Header;

typedef struct {
    int localDepth;
    int numRecords;
    long nextBucket;
} BucketMeta;

typedef struct {
    Header header;
    long *directory;
    FILE *file;
} sHashFile;

static int writeHeader(sHashFile *hashFile) {
    if (fseek(hashFile->file, 0, SEEK_SET) != 0) {
        return 0;
    }
    return fwrite(&hashFile->header, sizeof(Header), 1, hashFile->file) == 1;
}

static int writeDirectory(sHashFile *hashFile) {
    if (fseek(hashFile->file, hashFile->header.directoryOffset, SEEK_SET) != 0) {
        return 0;
    }
    return fwrite(hashFile->directory, sizeof(long), hashFile->header.numBuckets, hashFile->file) == (size_t)hashFile->header.numBuckets;
}

static int readDirectory(sHashFile *hashFile) {
    if (fseek(hashFile->file, hashFile->header.directoryOffset, SEEK_SET) != 0) {
        return 0;
    }
    return fread(hashFile->directory, sizeof(long), hashFile->header.numBuckets, hashFile->file) == (size_t)hashFile->header.numBuckets;
}

static int writeBucketMeta(sHashFile *hashFile, long bucketOffset, BucketMeta *bucket) {
    if (fseek(hashFile->file, bucketOffset, SEEK_SET) != 0) {
        return 0;
    }
    return fwrite(bucket, sizeof(BucketMeta), 1, hashFile->file) == 1;
}

static int readBucketMeta(sHashFile *hashFile, long bucketOffset, BucketMeta *bucket) {
    if (fseek(hashFile->file, bucketOffset, SEEK_SET) != 0) {
        return 0;
    }
    return fread(bucket, sizeof(BucketMeta), 1, hashFile->file) == 1;
}

static int bucketCapacity(sHashFile *hashFile) {
    int available = hashFile->header.bucketSize - (int)sizeof(BucketMeta);
    if (available <= 0 || hashFile->header.recordSize <= 0) {
        return 0;
    }
    return available / hashFile->header.recordSize;
}

static long allocateBucketPage(sHashFile *hashFile) {
    if (fseek(hashFile->file, 0, SEEK_END) != 0) {
        return -1;
    }
    return ftell(hashFile->file);
}

static int expandDirectory(sHashFile *hashFile) {
    int oldNumBuckets = hashFile->header.numBuckets;
    int newNumBuckets = oldNumBuckets << 1;
    long *newDirectory = (long*)malloc((size_t)newNumBuckets * sizeof(long));
    if (!newDirectory) {
        return 0;
    }

    for (int i = 0; i < oldNumBuckets; i++) {
        newDirectory[2 * i] = hashFile->directory[i];
        newDirectory[2 * i + 1] = hashFile->directory[i];
    }

    if (fseek(hashFile->file, 0, SEEK_END) != 0) {
        free(newDirectory);
        return 0;
    }

    long newDirOffset = ftell(hashFile->file);
    if (fwrite(newDirectory, sizeof(long), newNumBuckets, hashFile->file) != (size_t)newNumBuckets) {
        free(newDirectory);
        return 0;
    }

    hashFile->header.globalDepth++;
    hashFile->header.numBuckets = newNumBuckets;
    hashFile->header.directoryOffset = newDirOffset;

    if (!writeHeader(hashFile)) {
        free(newDirectory);
        return 0;
    }

    fflush(hashFile->file);
    free(hashFile->directory);
    hashFile->directory = newDirectory;
    return 1;
}

static int writeRecordToBucket(sHashFile *hashFile, HashItem item, int bucketIndex) {
    long bucketOffset = hashFile->directory[bucketIndex];
    BucketMeta bucketMeta;
    if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
        return 0;
    }

    int maxRecords = bucketCapacity(hashFile);
    if (bucketMeta.numRecords >= maxRecords) {
        return 0;
    }

    long recordPosition = bucketOffset + sizeof(BucketMeta) + (long)bucketMeta.numRecords * hashFile->header.recordSize;
    if (fseek(hashFile->file, recordPosition, SEEK_SET) != 0) {
        return 0;
    }

    if (fwrite(item, hashFile->header.recordSize, 1, hashFile->file) != 1) {
        return 0;
    }

    bucketMeta.numRecords++;
    if (!writeBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
        return 0;
    }

    fflush(hashFile->file);
    return 1;
}

static int splitBucket(sHashFile *hashFile, int bucketIndex) {
    long oldBucketOffset = hashFile->directory[bucketIndex];
    BucketMeta oldBucket;
    if (!readBucketMeta(hashFile, oldBucketOffset, &oldBucket)) {
        return 0;
    }

    int oldLocalDepth = oldBucket.localDepth;
    int newLocalDepth = oldLocalDepth + 1;
    if (newLocalDepth > hashFile->header.globalDepth) {
        return 0;
    }

    int oldNumRecords = oldBucket.numRecords;
    long newBucketOffset = allocateBucketPage(hashFile);
    if (newBucketOffset < 0) {
        return 0;
    }

    BucketMeta newBucket = {
        .localDepth = newLocalDepth,
        .numRecords = 0,
        .nextBucket = 0
    };

    oldBucket.localDepth = newLocalDepth;
    oldBucket.numRecords = 0;

    int numBuckets = hashFile->header.numBuckets;
    for (int i = 0; i < numBuckets; i++) {
        if (hashFile->directory[i] == oldBucketOffset) {
            if (((i >> oldLocalDepth) & 1) != 0) {
                hashFile->directory[i] = newBucketOffset;
            }
        }
    }

    if (!writeDirectory(hashFile)) {
        return 0;
    }

    if (!writeBucketMeta(hashFile, oldBucketOffset, &oldBucket)) {
        return 0;
    }

    if (!writeBucketMeta(hashFile, newBucketOffset, &newBucket)) {
        return 0;
    }

    fflush(hashFile->file);

    if (oldNumRecords > 0) {
        int recordSize = hashFile->header.recordSize;
        char *buffer = (char*)malloc((size_t)oldNumRecords * recordSize);
        if (!buffer) {
            return 0;
        }

        long readPosition = oldBucketOffset + sizeof(BucketMeta);
        if (fseek(hashFile->file, readPosition, SEEK_SET) != 0) {
            free(buffer);
            return 0;
        }

        if (fread(buffer, recordSize, (size_t)oldNumRecords, hashFile->file) != (size_t)oldNumRecords) {
            free(buffer);
            return 0;
        }

        for (int i = 0; i < oldNumRecords; i++) {
            char *record = buffer + (size_t)i * recordSize;
            char *recordKey = record + hashFile->header.keyOffset;
            int targetBucketIndex = getKey(recordKey, hashFile->header.globalDepth);
            if (targetBucketIndex < 0 || targetBucketIndex >= hashFile->header.numBuckets) {
                free(buffer);
                return 0;
            }
            if (!writeRecordToBucket(hashFile, record, targetBucketIndex)) {
                free(buffer);
                return 0;
            }
        }

        free(buffer);
    }

    return 1;
}

HashFile criarHashFile(char *nome, int recordSize, int bucketSize) {
    int d = 1;
    int numBuckets = 1 << d;
    Header header = {
        .globalDepth = d,
        .recordSize = recordSize,
        .numBuckets = numBuckets,
        .bucketSize = bucketSize,
        .directoryOffset = sizeof(Header),
        .bucketDataOffset = sizeof(Header) + numBuckets * sizeof(long),
        .keyOffset = 0,
        .keySize = 0
    };

    if (recordSize <= 0 || bucketSize <= (int)sizeof(BucketMeta)) {
        return NULL;
    }

    sHashFile *hashFile = (sHashFile*)malloc(sizeof(sHashFile));
    if (!hashFile) {
        return NULL;
    }

    hashFile->directory = (long*)malloc((size_t)numBuckets * sizeof(long));
    if (!hashFile->directory) {
        free(hashFile);
        return NULL;
    }

    for (int i = 0; i < numBuckets; i++) {
        hashFile->directory[i] = header.bucketDataOffset + (long)i * bucketSize;
    }

    hashFile->header = header;
    hashFile->file = fopen(nome, "wb+");
    if (!hashFile->file) {
        free(hashFile->directory);
        free(hashFile);
        return NULL;
    }

    if (fwrite(&hashFile->header, sizeof(Header), 1, hashFile->file) != 1) {
        fclose(hashFile->file);
        free(hashFile->directory);
        free(hashFile);
        return NULL;
    }

    if (fwrite(hashFile->directory, sizeof(long), numBuckets, hashFile->file) != (size_t)numBuckets) {
        fclose(hashFile->file);
        free(hashFile->directory);
        free(hashFile);
        return NULL;
    }

    BucketMeta bucket = {
        .localDepth = d,
        .numRecords = 0,
        .nextBucket = 0
    };

    for (int i = 0; i < numBuckets; i++) {
        if (!writeBucketMeta(hashFile, hashFile->directory[i], &bucket)) {
            fclose(hashFile->file);
            free(hashFile->directory);
            free(hashFile);
            return NULL;
        }
    }

    fflush(hashFile->file);
    return hashFile;
}

HashFile lerHashFile(char *file_name) {
    sHashFile *hashFile = (sHashFile*)malloc(sizeof(sHashFile));
    if (!hashFile) {
        return NULL;
    }

    hashFile->file = fopen(file_name, "rb+");
    if (!hashFile->file) {
        free(hashFile);
        return NULL;
    }

    if (fread(&hashFile->header, sizeof(Header), 1, hashFile->file) != 1) {
        fclose(hashFile->file);
        free(hashFile);
        return NULL;
    }

    if (hashFile->header.numBuckets <= 0) {
        fclose(hashFile->file);
        free(hashFile);
        return NULL;
    }

    hashFile->directory = (long*)malloc((size_t)hashFile->header.numBuckets * sizeof(long));
    if (!hashFile->directory) {
        fclose(hashFile->file);
        free(hashFile);
        return NULL;
    }

    if (!readDirectory(hashFile)) {
        fclose(hashFile->file);
        free(hashFile->directory);
        free(hashFile);
        return NULL;
    }

    return hashFile;
}

void printHashFileInfo(HashFile hash) {
    sHashFile* hashFile = (sHashFile*)hash;
    if (!hashFile) {
        return;
    }

    printf("Global Depth: %d\n", hashFile->header.globalDepth);
    printf("Record Size: %d\n", hashFile->header.recordSize);
    printf("Number of Buckets: %d\n", hashFile->header.numBuckets);
    printf("Bucket Size: %d\n", hashFile->header.bucketSize);
    printf("Directory Offset: %ld\n", hashFile->header.directoryOffset);
    printf("Bucket Data Offset: %ld\n", hashFile->header.bucketDataOffset);
    printf("Key Offset: %d\n", hashFile->header.keyOffset);
    printf("Key Size: %d\n", hashFile->header.keySize);
    printf("\nDirectory:\n");
    for (int i = 0; i < hashFile->header.numBuckets; i++) {
        printf("Dir[%d] = %ld\n", i, hashFile->directory[i]);
    }
}

int getBucketsLength(HashFile hash){
    if (!hash) return 0;
    sHashFile* hashFile = (sHashFile*)hash;
    return hashFile->header.numBuckets;
}

int getKey(char *key, int depth) {
    if (!key || depth <= 0) {
        return 0;
    }

    unsigned int hashed = (unsigned int)hashString(key);
    unsigned int mask;

    if (depth >= (int)(sizeof(mask) * 8)) {
        mask = ~0u;
    } else {
        mask = (1u << depth) - 1u;
    }

    return (int)(hashed & mask);
}

int adicionarHashItem(HashFile *hash, HashItem item, char *key){
    if (!hash || !*hash || !item || !key) {
        return 0;
    }

    sHashFile *hashFile = (sHashFile *)(*hash);
    if (!hashFile->file) {
        return 0;
    }

    while (1) {
        int bucketIndex = getKey(key, hashFile->header.globalDepth);
        if (bucketIndex < 0 || bucketIndex >= hashFile->header.numBuckets) {
            return 0;
        }

        if (writeRecordToBucket(hashFile, item, bucketIndex)) {
            return 1;
        }

        long bucketOffset = hashFile->directory[bucketIndex];
        BucketMeta bucketMeta;
        if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
            return 0;
        }

        if (bucketMeta.localDepth == hashFile->header.globalDepth) {
            if (!expandDirectory(hashFile)) {
                return 0;
            }
            continue;
        }

        if (!splitBucket(hashFile, bucketIndex)) {
            return 0;
        }
    }
}

HashItem buscarHashItem(HashFile hash, char *key){
    if (!hash || !key) {
        return NULL;
    }

    sHashFile* hashFile = (sHashFile*)hash;
    if (!hashFile->file) {
        return NULL;
    }

    int bucketIndex = getKey(key, hashFile->header.globalDepth);
    if (bucketIndex < 0 || bucketIndex >= hashFile->header.numBuckets) {
        return NULL;
    }

    long bucketOffset = hashFile->directory[bucketIndex];
    BucketMeta bucketMeta;
    if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
        return NULL;
    }

    int recordSize = hashFile->header.recordSize;
    if (recordSize <= 0 || bucketMeta.numRecords <= 0) {
        return NULL;
    }

    char *record = (char*)malloc(recordSize);
    if (!record) {
        return NULL;
    }

    long currentPosition = bucketOffset + sizeof(BucketMeta);
    size_t keyLength = strlen(key) + 1;

    for (int i = 0; i < bucketMeta.numRecords; i++) {
        if (fseek(hashFile->file, currentPosition, SEEK_SET) != 0) {
            break;
        }

        if (fread(record, recordSize, 1, hashFile->file) != 1) {
            break;
        }

        char *recordKey = record + hashFile->header.keyOffset;
        if (strncmp(recordKey, key, keyLength) == 0) {
            return (HashItem)record;
        }

        currentPosition += recordSize;
    }

    free(record);
    return NULL;
}

void destruirHashFile(HashFile hash){
    if (!hash) return;
    sHashFile* hashFile = (sHashFile*)hash;
    if (hashFile->directory) {
        free(hashFile->directory);
    }
    if (hashFile->file) {
        fclose(hashFile->file);
    }
    free(hashFile);
}

int hashString(char *str) {
    int hash = 0;
    while (*str) {
        hash = (hash * 31) + *str++;
    }
    return hash;
}
