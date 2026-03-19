#include<stdio.h>
#include "calc.h"
#include <stddef.h>
#include <stdint.h>

// Retorna um índice em [0, table_size)
size_t hash_indice(const char *s, size_t table_size) {
    if (!s || table_size == 0) return 0;

    uint32_t h = 5381u;                 // djb2
    for (unsigned char c = (unsigned char)*s; c; c = (unsigned char)*++s) {
        h = ((h << 5) + h) + c;         // h * 33 + c
    }
    return (size_t)(h % table_size);
}


int main() {
    int numero = -1;
    int resultado = calcular_fatorial(numero);
    printf("O fatorial de %d e %d\n", numero, resultado);

    int x = 10;
    int y = 3;
    float res = (float)x/y;
    printf("Resultado %f\n", res);

    int compara = x>y; // Retorna 1 se for verdadeiro e 0 se for falso
    printf("Comparacao %d\n", compara);

    float zero = 0;
    if (zero) {
        printf("O numero %d e diferente de zero\n", numero);
    }

    size_t M = 101; // tamanho da tabela (de preferência primo)
    printf("%zu\n", hash_indice("banana", M));
    printf("%zu\n", hash_indice("abacaxi", M));
    printf("%zu\n", hash_indice("abacate", M));

    return 0;
}