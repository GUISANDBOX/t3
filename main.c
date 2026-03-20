#include<stdio.h>
#include "calc.h"
#include <stddef.h>
#include <stdint.h>
#include "Lista.h"

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

    Lista lista = crialista();
    adicionar(&lista, "Primeiro", 0);
    adicionar(&lista, "Segundo", 0);
    adicionar(&lista, "Terceiro", 0);
    exibirlista(lista);

    return 0;
}