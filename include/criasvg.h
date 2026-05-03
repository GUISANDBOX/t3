#ifndef CRIASVG
#define CRIASVG
#include <stdio.h>
#include "hashfile.h"
#include "quadra.h"
#include "pessoa.h"

/// @brief Desenha uma quadra no arquivo SVG
/// @param q quadra a ser desenhada
/// @param arq arquivo SVG a ser desenhado
void desenhaSVGQuadra(Quadra q, FILE *arq);

/// @brief Cria um X vermelho no local da quadra
/// @param q quadra onde o X será criado
/// @param arq arquivo SVG onde o X será criado
void desenhaXvermelho(Quadra q, FILE *arq);

/// @brief Desenha as informacoes de moradores da quadra
/// @param q quadra
/// @param numN numero de moradores na face N
/// @param numS numero de moradores na face S
/// @param numL numero de moradores na face L
/// @param numO numero de moradores na face O
/// @param arq arquivo SVG
void desenhaInfoMoradoresQuadra(Quadra q, int numN, int numS, int numL, int numO, FILE *arq);

/// @brief Desenha um quadrado vermelho e o cpf na mudanca de endereco
/// @param q quadra
/// @param face face da quadra (N, S, L, O)
/// @param num numero da residencia
/// @param cpf cpf da pessoa
/// @param arq arquivo SVG
void desenhaMudancaSVG(Quadra q, char face, int num, const char *cpf, FILE *arq);

/// @brief Desenha uma cruz vermelha no local de moradia
/// @param q quadra
/// @param face face da quadra
/// @param num numero da residencia
/// @param arq arquivo SVG
void desenhaCruzVermelhaSVG(Quadra q, char face, int num, FILE *arq);

/// @brief Desenha um circulo preto no local de moradia
/// @param q quadra
/// @param face face da quadra
/// @param num numero da residencia
/// @param arq arquivo SVG
void desenhaCirculoPretoSVG(Quadra q, char face, int num, FILE *arq);

#endif