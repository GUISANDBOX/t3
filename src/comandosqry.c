#include "comandosqry.h"
#include <string.h>
#include <stdio.h>
#include "criasvg.h"
#include "hashfile.h"
#include "quadra.h"
#include "pessoa.h"
#include <stdlib.h>

void processaQry(FILE *arqqry, HashFile Hpm, HashFile Hgeo, FILE *arqtxt, FILE *arqsvg) {
    char comando[100];
    int z;

    fprintf(arqsvg, "<svg width=\"5000\" height=\"5000\" xmlns=\"http://www.w3.org/2000/svg\">\n");

    // Desenha todas as quadras da base antes de processar o QRY
    // HashItem *listaQuadras = malloc(100000 * sizeof(HashItem));
    // int numQuadras = getListaItens(Hgeo, listaQuadras);
    // for (int i = 0; i < numQuadras; i++) {
    //     Quadra q = (Quadra)listaQuadras[i];
    //     desenhaSVGQuadra(q, arqsvg);
    //     free(q); // getListaItens aloca memoria para cada registro retornado
    // }
    // free(listaQuadras);


    do {
        z = fscanf(arqqry, " %99s", comando);
        if (z != 1) break;

        printf("Lendo comando QRY: %s\n", comando);
        if (strcmp(comando, "rq") == 0) {
            char cep[10];
            fscanf(arqqry, "%s", cep);
            Quadra q = buscarHashItem(Hgeo, cep);
            if (!q) {
                fprintf(arqtxt, "Quadra %s não encontrada\n", cep);
            } else {
                removerHashItem(Hgeo, cep);
                desenhaXvermelho(q, arqsvg);
                fprintf(arqtxt, "Removendo quadra %s\n", cep);
                
                // Remove a moradia das pessoas que moravam nessa quadra
                HashItem *listaPessoas = malloc(100000 * sizeof(HashItem));
                int numPessoas = getListaItens(Hpm, listaPessoas);
                for (int i = 0; i < numPessoas; i++) {
                    Pessoa p = (Pessoa)listaPessoas[i];
                    if (temMoradia(p) && strcmp(getMoradiaCep(p), cep) == 0) {
                        removerMoradia(p);
                        atualizarHashItem(&Hpm, p, getCpf(p));
                        fprintf(arqtxt, "CPF: %s, Nome: %s %s atingido\n", getCpf(p), getNome(p), getSobrenome(p));
                    }
                    free(p);
                }
                free(listaPessoas);
            }
        }
        else if (strcmp(comando, "pq") == 0) {
            char cep[10];
            fscanf(arqqry, "%s", cep);
            // Implementar procurar quadra com cep
            fprintf(arqtxt, "Procurar quadra %s\n", cep);
        }
        else if (strcmp(comando, "censo") == 0) {
            // Implementar censo, sem parâmetros
            fprintf(arqtxt, "Censo realizado\n");
        }
        else if (strcmp(comando, "h?") == 0) {
            char cpf[12];
            fscanf(arqqry, "%s", cpf);
            // Implementar verificar habitação com cpf
            fprintf(arqtxt, "Verificar habitação para CPF %s\n", cpf);
        }
        else if (strcmp(comando, "nasc") == 0) {
            char cpf[12], nome[100], sobrenome[100], sexo, nascimento[11];
            fscanf(arqqry, "%s %s %s %c %s", cpf, nome, sobrenome, &sexo, nascimento);
            // Implementar nascimento, criar pessoa
            fprintf(arqtxt, "Nascimento: CPF %s, Nome %s %s, Sexo %c, Nascimento %s\n", cpf, nome, sobrenome, sexo, nascimento);
        }
        else if (strcmp(comando, "rip") == 0) {
            char cpf[12];
            fscanf(arqqry, "%s", cpf);
            // Implementar remover pessoa com cpf
            fprintf(arqtxt, "Remover pessoa CPF %s\n", cpf);
        }
        else if (strcmp(comando, "mud") == 0) {
            char cpf[12], cep[10], face[10], cmpl[100];
            int num;
            fscanf(arqqry, "%s %s %s %d %s", cpf, cep, face, &num, cmpl);
            // Implementar mudança de endereço
            fprintf(arqtxt, "Mudança: CPF %s para CEP %s, Face %s, Num %d, Compl %s\n", cpf, cep, face, num, cmpl);
        }
        else if (strcmp(comando, "dspj") == 0) {
            char cpf[12];
            fscanf(arqqry, "%s", cpf);
            // Implementar despejar pessoa
            fprintf(arqtxt, "Despejar pessoa CPF %s\n", cpf);
        }
    } while (1);
    HashItem *listaQuadras = malloc(100000 * sizeof(HashItem));
    int numQuadras = getListaItens(Hgeo, listaQuadras);
    for (int i = 0; i < numQuadras; i++) {
        Quadra q = (Quadra)listaQuadras[i];
        desenhaSVGQuadra(q, arqsvg);
        free(q); // getListaItens aloca memoria para cada registro retornado
    }
    free(listaQuadras);
    fprintf(arqsvg, "</svg>\n");
}