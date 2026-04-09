#ifndef HASHFILE
#define HASHFILE
#include <stdio.h>

//programa de hashfile, onde cada item é um ponteiro genérico (void*), e o hashfile em si também é um ponteiro genérico (void*).

typedef void *HashItem; // Define que cada item do hashfile é um item genérico
typedef void *HashFile; // Define o tipo HashFile

/// @brief Cria um hashfile
/// @param nome nome do arquivo
/// @param d profundidade
/// @return objeto HashFile
HashFile criarHashFile(char *nome, int recordSize, int bucketSize);

HashFile lerHashFile(HashFile hash);

/// @brief retorna o número de buckets do hashfile
/// @param hash hashfile a ser consultado
/// @return número de buckets
int getBucketsLength(HashFile hash);

/// @brief Adiciona um item ao hashfile
/// @param hash hashfile onde o item será adicionado
/// @param item item a ser adicionado
/// @return 1 se o item foi adicionado com sucesso, 0 caso contrário
int adicionarHashItem(HashFile *hash, HashItem item);

/// @brief Busca um item no hashfile pelo valor
/// @param hash hashfile onde o item será buscado
/// @param valor valor do item a ser buscado 
/// @return o item encontrado ou NULL se não encontrado 
HashItem buscarHashItem(HashFile hash, int valor);

/// @brief Destrói o hashfile, liberando os recursos alocados 
/// @param hash hashfile a ser destruído
void destruirHashFile(HashFile hash);

/// @brief Função hash simples para strings
/// @param str string a ser hasheada
/// @return valor hash
int hashString(char *str);

#endif