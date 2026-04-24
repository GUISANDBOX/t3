#include "comandosqry.h"
#include <string.h>
#include <stdio.h>

void processaQry(FILE *arqqry, HashFile Hpm, HashFile Hgeo, FILE *arqtxt) {
    char comando[100];
    int z;

    do {
        z = fscanf(arqqry, " %99s", comando);
        if (z != 1) break;

        printf("Lendo comando QRY: %s\n", comando);
        if (strcmp(comando, "rq") == 0) {
            char cep[10];
            fscanf(arqqry, "%s", cep);
            // Implementar remoção da quadra com cep
            fprintf(arqtxt, "Remover quadra %s\n", cep);
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
}