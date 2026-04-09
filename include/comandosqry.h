#ifndef QRY_H
#define QRY_H
#include "hashfile.h"

/********** Funções para processar os comandos do arquivo .qry **********/

void processaQry(FILE *arqqry, HashFile Hpm, HashFile Hgeo, FILE *arqtxt);
//processa os comandos do arquivo .qry e adiciona os elementos no hashfile

#endif