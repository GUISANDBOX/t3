#include "criasvg.h"
#include <stdio.h>
#include "quadra.h"
#include <ctype.h>
#include <string.h>

void desenhaSVGQuadra(Quadra q, FILE *arq) {
    fprintf(arq, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke-width=\"%s\" fill=\"%s\" stroke=\"%s\" />\n", getX(q), getY(q), getW(q), getH(q), getSw(q), getCfill(q), getCstrk(q));
    fprintf(arq, "<text x=\"%lf\" y=\"%lf\">%s</text>\n", getX(q), getY(q), getCep(q));
}

void desenhaXvermelho(Quadra q, FILE *arq) {
    fprintf(arq, "<text x=\"%lf\" y=\"%lf\" stroke=\"red\">X</text>\n", getX(q), getY(q));
}

void desenhaInfoMoradoresQuadra(Quadra q, int numN, int numS, int numL, int numO, FILE *arq) {
    double x = getX(q);
    double y = getY(q);
    double w = getW(q);
    double h = getH(q);
    int total = numN + numS + numL + numO;

    // Face N (topo)
    fprintf(arq, "<rect x=\"%lf\" y=\"%lf\" width=\"20\" height=\"15\" fill=\"white\" opacity=\"0.9\" />\n", x + w / 2 - 10, y + 2);
    fprintf(arq, "<text x=\"%lf\" y=\"%lf\" font-family=\"sans-serif\" font-size=\"10\" text-anchor=\"middle\">%d</text>\n", x + w / 2, y + 15, numN);
    // Face S (fundo)
    fprintf(arq, "<rect x=\"%lf\" y=\"%lf\" width=\"20\" height=\"15\" fill=\"white\" opacity=\"0.9\" />\n", x + w / 2 - 10, y + h - 15);
    fprintf(arq, "<text x=\"%lf\" y=\"%lf\" font-family=\"sans-serif\" font-size=\"10\" text-anchor=\"middle\">%d</text>\n", x + w / 2, y + h - 5, numS);
    // Face L (direita)
    fprintf(arq, "<rect x=\"%lf\" y=\"%lf\" width=\"20\" height=\"15\" fill=\"white\" opacity=\"0.9\" />\n", x + w - 22, y + h / 2 - 7);
    fprintf(arq, "<text x=\"%lf\" y=\"%lf\" font-family=\"sans-serif\" font-size=\"10\" text-anchor=\"end\">%d</text>\n", x + w - 5, y + h / 2 + 3, numL);
    // Face O (esquerda)
    fprintf(arq, "<rect x=\"%lf\" y=\"%lf\" width=\"20\" height=\"15\" fill=\"white\" opacity=\"0.9\" />\n", x + 2, y + h / 2 - 7);
    fprintf(arq, "<text x=\"%lf\" y=\"%lf\" font-family=\"sans-serif\" font-size=\"10\" text-anchor=\"start\">%d</text>\n", x + 5, y + h / 2 + 3, numO);
    // Centro (Total)
    fprintf(arq, "<rect x=\"%lf\" y=\"%lf\" width=\"24\" height=\"20\" fill=\"white\" opacity=\"0.9\" />\n", x + w / 2 - 12, y + h / 2 - 10);
    fprintf(arq, "<text x=\"%lf\" y=\"%lf\" font-family=\"sans-serif\" font-size=\"14\" font-weight=\"bold\" text-anchor=\"middle\">%d</text>\n", x + w / 2, y + h / 2 + 5, total);
}

void desenhaMudancaSVG(Quadra q, char face, int num, const char *cpf, FILE *arq) {
    double x = getX(q);
    double y = getY(q);
    double w = getW(q);
    double h = getH(q);
    
    double posX = x;
    double posY = y;
    
    if (face == 'N') {
        posX = x + num;
        posY = y;
    } else if (face == 'S') {
        posX = x + num;
        posY = y + h;
    } else if (face == 'L') {
        posX = x + w;
        posY = y + num;
    } else if (face == 'O') {
        posX = x;
        posY = y + num;
    }

    char cpf_lower[20];
    for(int i = 0; cpf[i]; i++) {
        cpf_lower[i] = tolower(cpf[i]);
    }
    cpf_lower[strlen(cpf)] = '\0';

    fprintf(arq, "<rect x=\"%lf\" y=\"%lf\" width=\"10\" height=\"10\" fill=\"red\" />\n", posX - 5, posY - 5);
    fprintf(arq, "<text x=\"%lf\" y=\"%lf\" font-family=\"sans-serif\" font-size=\"8\" fill=\"black\">%s</text>\n", posX + 6, posY + 4, cpf_lower);
}

void desenhaCruzVermelhaSVG(Quadra q, char face, int num, FILE *arq) {
    double x = getX(q);
    double y = getY(q);
    double w = getW(q);
    double h = getH(q);
    
    double posX = x;
    double posY = y;
    
    if (face == 'N') { posX = x + num; posY = y; } 
    else if (face == 'S') { posX = x + num; posY = y + h; } 
    else if (face == 'L') { posX = x + w; posY = y + num; } 
    else if (face == 'O') { posX = x; posY = y + num; }

    fprintf(arq, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"2\" />\n", posX - 5, posY - 5, posX + 5, posY + 5);
    fprintf(arq, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"2\" />\n", posX - 5, posY + 5, posX + 5, posY - 5);
}

void desenhaCirculoPretoSVG(Quadra q, char face, int num, FILE *arq) {
    double x = getX(q);
    double y = getY(q);
    double w = getW(q);
    double h = getH(q);
    
    double posX = x;
    double posY = y;
    
    if (face == 'N') { posX = x + num; posY = y; } 
    else if (face == 'S') { posX = x + num; posY = y + h; } 
    else if (face == 'L') { posX = x + w; posY = y + num; } 
    else if (face == 'O') { posX = x; posY = y + num; }

    fprintf(arq, "<circle cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"black\" />\n", posX, posY);
}