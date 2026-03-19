#include "calc.h"

int calcular_fatorial (int n ) {
    if ( n < 0) return -1; // Erro para numeros negativos
    if ( n == 0 || n == 1) return 1;
    return n * calcular_fatorial ( n - 1) ;
}

/**
 * Somente vai dar o desconto para quem for menor de idade ou maior ou igual a 65 anos.
 **/
float calcular_desconto(float valor, float percentual, int idade) {
    float desc = 0;
    if (idade < 18 || idade >= 65) {
        desc = valor * (percentual / 100);
    }
    return valor - desc; // Sem desconto para outras idades
}