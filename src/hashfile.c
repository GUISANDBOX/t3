#include <stdio.h>
#include <stdlib.h>
#include "hashfile.h"

typedef struct {
    int globalDepth;
    int recordSize;
    int numBuckets;
    int bucketSize;
    int keyOffset;
    int keySize;
    long tableOffset;
    long bucketOffset;
} Header;

typedef struct {
    int localDepth;
    int numRecords;
    long nextBucket; // Offset do próximo bucket no arquivo
} Bucket;

typedef struct {
    Header header;
    Bucket *table;
    FILE *file;
} sHashFile;

HashFile criarHashFile(char *nome, int recordSize, int bucketSize) {
    int d = 1;
    Header header = {
        .globalDepth = d,
        .recordSize = recordSize,
        .bucketSize = bucketSize,
        .numBuckets = d << 1,
        .keyOffset = 0,
        .keySize = sizeof(int),
        .tableOffset = sizeof(Header),
        .bucketOffset = sizeof(Header) // Adiciona o cabeçalho para calcular o offset do bucket
    };

    header.bucketOffset = header.tableOffset + header.numBuckets * sizeof(long); // Atualiza o bucket offset de acordo com o tamanho da tabela
    
    sHashFile* hashFile = (sHashFile*)malloc(sizeof(sHashFile));
    
    if (!hashFile) {
        printf("Erro na alocação do hashfile\n");
        return NULL;
    }

    hashFile->table = (Bucket*)malloc(header.numBuckets * sizeof(Bucket));
    if (!hashFile->table) {
        printf("Erro na alocação da tabela do hashfile\n");
        free(hashFile);
        return NULL;
    }
    for (int i=0; i<header.numBuckets; i++) {
        hashFile->table[i].nextBucket = header.bucketOffset + i * header.bucketSize;
        hashFile->table[i].localDepth = header.globalDepth;
        hashFile->table[i].numRecords = 0;
    }

    hashFile->header = header;
    hashFile->file = fopen(nome, "wb+");
    if (!hashFile->file) {
        printf("Erro ao criar o arquivo %s\n", nome);
        free(hashFile);
        return NULL;
    }
    
    // Grava o cabeçalho no arquivo binário
    fwrite(&header, sizeof(Header), 1, hashFile->file);
    
    // Grava a tabela de buckets no arquivo
    fwrite(hashFile->table, sizeof(Bucket), header.numBuckets, hashFile->file);

    fclose(hashFile->file); // Fecha arquivo para salvar

    return hashFile;
}

void printHashFileInfo(HashFile hash) {
    sHashFile* hashFile = (sHashFile*)hash;
    printf("Global Depth: %d\n", hashFile->header.globalDepth);
    printf("Record Size: %d\n", hashFile->header.recordSize);
    printf("Number of Buckets: %d\n", hashFile->header.numBuckets);
    printf("Bucket Size: %d\n", hashFile->header.bucketSize);
    printf("Key Offset: %d\n", hashFile->header.keyOffset);
    printf("Key Size: %d\n", hashFile->header.keySize);
    printf("Table Offset: %ld\n", hashFile->header.tableOffset);
    printf("Bucket Offset: %ld\n", hashFile->header.bucketOffset);
    // Dump da tabela
    printf("\nBucket Table:\n");
    for (int i = 0; i < hashFile->header.numBuckets; i++) {
        printf("Bucket %d: position %ld, Local Depth %d, Number of Records %d\n", i, hashFile->table[i].nextBucket, hashFile->table[i].localDepth, hashFile->table[i].numRecords);
    }
}

HashFile lerHashFile(char *file_name) {
    sHashFile* hashFile = (sHashFile*)malloc(sizeof(sHashFile));
    if (!hashFile) {
        printf("Erro na alocação do hashfile\n");
        return NULL;
    }
    hashFile->file = fopen(file_name, "rb");
    if (!hashFile->file) {
        printf("Erro ao abrir o arquivo %s\n", file_name);
        free(hashFile);
        return NULL;
    }
    fread(&hashFile->header, sizeof(Header), 1, hashFile->file);
    hashFile->table = (Bucket*)malloc(hashFile->header.numBuckets * sizeof(Bucket));
    if (!hashFile->table) {
        printf("Erro na alocação da tabela do hashfile\n");
        fclose(hashFile->file);
        free(hashFile);
        return NULL;
    }
    fread(hashFile->table, sizeof(Bucket), hashFile->header.numBuckets, hashFile->file);
    
    fclose(hashFile->file);
    return hashFile;
}

int getBucketsLength(HashFile hash){
    
}

int adicionarHashItem(HashFile *hash, HashItem item){
    
}

HashItem buscarHashItem(HashFile hash, int valor){
    
}

void destruirHashFile(HashFile hash){
    if (!hash) return;
    sHashFile* hashFile = (sHashFile*)hash;
    if (hashFile->table) {
        free(hashFile->table);
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