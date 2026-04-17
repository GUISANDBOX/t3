#include <string.h>
#include "comandospm.h"
#include "hashfile.h"
#include "pessoa.h"

HashFile processaPm(FILE *arqpm, HashFile H) {
    char comando[100];
    int z, i;
    char cpf[12], nome[100], sobrenome[100], sexo, nasc[11];
    char cep[10], face, compl[100];
    int num;

    do {
        z = fscanf(arqpm, " %99s", comando);
        if (z != 1) break;

        if (comando[0] == 'p') {
            fscanf(arqpm, "%s %s %s %c %s", cpf, nome, sobrenome, &sexo, nasc);
            Pessoa p = criaPessoa(cpf, nome, sobrenome, sexo, nasc);
            if(!adicionarHashItem(&H, p, getCpf(p))) {
                //printf("Erro ao adicionar item ao hashfile\n");
            }
            printf("Pessoa criada: %s %s %s\n", cpf, nome, sobrenome);
        }
        else if (comando[0] == 'm') {
            double rx, ry, rw, rh;
            fscanf(arqpm, "%s %s %c %d %s", cpf, cep, &face, &num, compl);
            printf("Atribuindo pessoa %s em %s na face %c\n", cpf, cep, face);
        }
    } while (1);

    return H;
}