#ifndef PM_H
#define PM_H
#include "hashfile.h"
#include "item.h"

/* Processa os comandos do arquivo .pm e adiciona os elementos no hashfile */
HashFile processaPm(FILE *arqpm, HashFile H);

#endif