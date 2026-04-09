#include <stdio.h>
#include <stdlib.h>
#include "hashfile.h"

typedef struct {
    int globalDepth;
    int recordSize;
    int bucketSize;
    int keyOffset;
    int keySize;
    long directoryOffset;
    long bucketOffset;
} Header;

typedef struct {
    Header header;
    FILE *file;
} sHashFile;

HashFile criarHashFile(char *nome, int recordSize, int bucketSize) {
    printf("1");Header header = {
        .globalDepth = 1,
        .recordSize = recordSize,
        .bucketSize = bucketSize,
        .keyOffset = 0,
        .keySize = sizeof(int),
        .directoryOffset = sizeof(Header),
        .bucketOffset = sizeof(Header) + (1 << 0) * sizeof(long)
    };
    printf("2");
    sHashFile* hashFile = (sHashFile*)malloc(sizeof(sHashFile));
    printf("3");
    if (!hashFile) {
        printf("Erro na alocação do hashfile\n");
        return NULL;
    }
    hashFile->header = header;
    hashFile->file = fopen(nome, "wb+");
    if (!hashFile->file) {
        printf("Erro ao criar o arquivo %s\n", nome);
        free(hashFile);
        return NULL;
    }
    fwrite(&header, sizeof(Header), 1, hashFile->file);
    return hashFile;
}

HashFile lerHashFile(HashFile hash) {
    sHashFile* hashFile = (sHashFile*)hash;
    fread(&hashFile->header, sizeof(Header), 1, hashFile->file);
    printf("HashFile lido: globalDepth=%d, recordSize=%d, bucketSize=%d\n", hashFile->header.globalDepth, hashFile->header.recordSize, hashFile->header.bucketSize);
    return hashFile;
}

int getBucketsLength(HashFile hash){
    
}

int adicionarHashItem(HashFile *hash, HashItem item){
    
}

HashItem buscarHashItem(HashFile hash, int valor){
    
}

void destruirHashFile(HashFile hash){
    sHashFile* hashFile = (sHashFile*)hash;
    fclose(hashFile->file);
    free(hashFile);
}

int hashString(char *str) {
    int hash = 0;
    while (*str) {
        hash = (hash * 31) + *str++;
    }
    return hash;
}