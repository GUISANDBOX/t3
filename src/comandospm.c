#include <string.h>
#include "comandospm.h"
#include "hashfile.h"
#include "pessoa.h"

HashFile processaPm(FILE *arqpm, HashFile H) {
    char comando[100];
    int z, i=0;
    char cpf[20], nome[100], sobrenome[100], sexo, nasc[11];
    char cep[10], face, compl[100];
    int num;

    do {
        z = fscanf(arqpm, " %99s", comando);
        if (z != 1) break;

        if (comando[0] == 'p') {
            fscanf(arqpm, "%19s %99s %99s %c %10s", cpf, nome, sobrenome, &sexo, nasc);
            Pessoa p = criaPessoa(cpf, nome, sobrenome, sexo, nasc);
            if(!adicionarHashItem(&H, p, getCpf(p))) {
                printf("Erro ao adicionar item ao hashfile\n");
            }
            destruirPessoa(p);
            printf("Pessoa criada: %s %s %s\n", cpf, nome, sobrenome);
            if (i==10 || i==50 || i==100) {
                printf("HashFile após %d pessoas:\n", i);
                printHashFileInfo(H);
            }
            i++;
        }
        else if (comando[0] == 'm') {
            fscanf(arqpm, "%19s %9s %c %d %99s", cpf, cep, &face, &num, compl);
            printf("Atribuindo pessoa %s em %s na face %c\n", cpf, cep, face);
            Pessoa p = (Pessoa)buscarHashItem(H, cpf);
            if (!p) {
                printf("Aviso: CPF %s não encontrado para moradia\n", cpf);
            } else {
                atribuirMoradia(p, cep, face, num, compl);
                if (!atualizarHashItem(&H, p, getCpf(p))) {
                    printf("Erro ao atualizar moradia para %s\n", cpf);
                }
                destruirPessoa(p);
            }
        }
    } while (1);

    return H;
}