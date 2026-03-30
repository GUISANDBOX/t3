#ifndef QUA_H
#define QUA_H
#include<stdio.h>

//programa para quadras.

typedef void *Quadra;

Quadra criaQuadra(char cep[10], double x, double y, double w, double h, char corb[100], char corp[100], int id);
//cria a quadra com suas informações

#endif