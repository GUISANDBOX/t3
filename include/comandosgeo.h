#ifndef GEO_H
#define GEO_H
#include "hashfile.h"
#include <stdio.h>

/* Processa os comandos do arquivo .geo e adiciona os elementos no hashfile */
HashFile processaGeo(FILE *arqgeo, HashFile H);

#endif