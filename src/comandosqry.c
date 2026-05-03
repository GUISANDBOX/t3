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

    HashItem *listaQuadras = malloc(100000 * sizeof(HashItem));
    int numQuadras = getListaItens(Hgeo, listaQuadras);
    for (int i = 0; i < numQuadras; i++) {
        Quadra q = (Quadra)listaQuadras[i];
        desenhaSVGQuadra(q, arqsvg);
        free(q);
    }
    free(listaQuadras);
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
            printf("Iniciando pq");
            char cep[10];
            fscanf(arqqry, "%s", cep);
            Quadra q = buscarHashItem(Hgeo, cep);
            if (!q) {
                fprintf(arqtxt, "Quadra %s não encontrada\n", cep);
            } else {
                int numN = 0, numS = 0, numL = 0, numO = 0;
                
                HashItem *listaPessoas = malloc(100000 * sizeof(HashItem));
                int numPessoas = getListaItens(Hpm, listaPessoas);
                for (int i = 0; i < numPessoas; i++) {
                    Pessoa p = (Pessoa)listaPessoas[i];
                    if (temMoradia(p) && strcmp(getMoradiaCep(p), cep) == 0) {
                        char face = getMoradiaFace(p);
                        if (face == 'N') numN++;
                        else if (face == 'S') numS++;
                        else if (face == 'L') numL++;
                        else if (face == 'O') numO++;
                    }
                    free(p);
                }
                free(listaPessoas);
                
                desenhaInfoMoradoresQuadra(q, numN, numS, numL, numO, arqsvg);
                destruirQuadra(q);
            }
        }
        else if (strcmp(comando, "censo") == 0) {
            fprintf(arqtxt, "Censo realizado:\n");
            int habitantes = 0, moradores = 0, semTetos = 0;
            int habHomens = 0, habMulheres = 0;
            int morHomens = 0, morMulheres = 0;
            int semTetoHomens = 0, semTetoMulheres = 0;

            HashItem *listaPessoas = malloc(100000 * sizeof(HashItem));
            int numPessoas = getListaItens(Hpm, listaPessoas);
            
            for (int i = 0; i < numPessoas; i++) {
                Pessoa p = (Pessoa)listaPessoas[i];
                char sexo = getSexo(p);
                int moradia = temMoradia(p);
                
                habitantes++;
                if (sexo == 'M') habHomens++;
                else if (sexo == 'F') habMulheres++;

                if (moradia) {
                    moradores++;
                    if (sexo == 'M') morHomens++;
                    else if (sexo == 'F') morMulheres++;
                } else {
                    semTetos++;
                    if (sexo == 'M') semTetoHomens++;
                    else if (sexo == 'F') semTetoMulheres++;
                }
                free(p);
            }
            free(listaPessoas);

            double propMorHab = habitantes > 0 ? (double)moradores / habitantes : 0;
            double pctHabHomens = habitantes > 0 ? ((double)habHomens / habitantes) * 100 : 0;
            double pctHabMulheres = habitantes > 0 ? ((double)habMulheres / habitantes) * 100 : 0;
            double pctMorHomens = moradores > 0 ? ((double)morHomens / moradores) * 100 : 0;
            double pctMorMulheres = moradores > 0 ? ((double)morMulheres / moradores) * 100 : 0;
            double pctSemTetoHomens = semTetos > 0 ? ((double)semTetoHomens / semTetos) * 100 : 0;
            double pctSemTetoMulheres = semTetos > 0 ? ((double)semTetoMulheres / semTetos) * 100 : 0;

            fprintf(arqtxt, "Numero total de habitantes: %d\n", habitantes);
            fprintf(arqtxt, "Numero total de moradores: %d\n", moradores);
            fprintf(arqtxt, "Proporcao moradores/habitantes: %.2lf\n", propMorHab);
            fprintf(arqtxt, "Numero de homens: %d\n", habHomens);
            fprintf(arqtxt, "Numero de mulheres: %d\n", habMulheres);
            fprintf(arqtxt, "%% de habitantes homens: %.2lf%%\n", pctHabHomens);
            fprintf(arqtxt, "%% de habitantes mulheres: %.2lf%%\n", pctHabMulheres);
            fprintf(arqtxt, "%% de moradores homens: %.2lf%%\n", pctMorHomens);
            fprintf(arqtxt, "%% de moradores mulheres: %.2lf%%\n", pctMorMulheres);
            fprintf(arqtxt, "Numero total de sem-tetos: %d\n", semTetos);
            fprintf(arqtxt, "%% sem-tetos homens: %.2lf%%\n", pctSemTetoHomens);
            fprintf(arqtxt, "%% sem-tetos mulheres: %.2lf%%\n", pctSemTetoMulheres);
            fprintf(arqtxt, "\n");
        }
        else if (strcmp(comando, "h?") == 0) {
            char cpf[20];
            fscanf(arqqry, "%s", cpf);
            Pessoa p = (Pessoa)buscarHashItem(Hpm, cpf);
            if (p) {
                fprintf(arqtxt, "Informacoes da Pessoa (h?):\n");
                fprintf(arqtxt, "CPF: %s\n", getCpf(p));
                fprintf(arqtxt, "Nome: %s %s\n", getNome(p), getSobrenome(p));
                fprintf(arqtxt, "Sexo: %c\n", getSexo(p));
                fprintf(arqtxt, "Nascimento: %s\n", getNascimento(p));
                if (temMoradia(p)) {
                    fprintf(arqtxt, "Moradia: CEP %s, Face %c, Num %d, Compl %s\n", 
                            getMoradiaCep(p), getMoradiaFace(p), getMoradiaNum(p), getMoradiaComplemento(p));
                } else {
                    fprintf(arqtxt, "Moradia: Sem-teto.\n");
                }
                fprintf(arqtxt, "\n");
                destruirPessoa(p);
            } else {
                fprintf(arqtxt, "h?: Pessoa com CPF %s nao encontrada.\n\n", cpf);
            }
        }
        else if (strcmp(comando, "nasc") == 0) {
            char cpf[20], nome[100], sobrenome[100], sexo, nascimento[11];
            fscanf(arqqry, "%s %s %s %c %s", cpf, nome, sobrenome, &sexo, nascimento);
            Pessoa p = criaPessoa(cpf, nome, sobrenome, sexo, nascimento);
            adicionarHashItem(&Hpm, p, getCpf(p));
            destruirPessoa(p);
        }
        else if (strcmp(comando, "rip") == 0) {
            char cpf[20];
            fscanf(arqqry, "%s", cpf);
            Pessoa p = (Pessoa)buscarHashItem(Hpm, cpf);
            if (p) {
                fprintf(arqtxt, "Falecimento (rip):\n");
                fprintf(arqtxt, "CPF: %s\n", getCpf(p));
                fprintf(arqtxt, "Nome: %s %s\n", getNome(p), getSobrenome(p));
                fprintf(arqtxt, "Sexo: %c\n", getSexo(p));
                fprintf(arqtxt, "Nascimento: %s\n", getNascimento(p));
                
                if (temMoradia(p)) {
                    char cep[15];
                    strcpy(cep, getMoradiaCep(p));
                    char face = getMoradiaFace(p);
                    int num = getMoradiaNum(p);
                    fprintf(arqtxt, "Moradia: CEP %s, Face %c, Num %d, Compl %s\n", 
                            cep, face, num, getMoradiaComplemento(p));
                            
                    Quadra q = buscarHashItem(Hgeo, cep);
                    if (q) {
                        desenhaCruzVermelhaSVG(q, face, num, arqsvg);
                        destruirQuadra(q);
                    }
                } else {
                    fprintf(arqtxt, "Moradia: Sem-teto.\n");
                }
                fprintf(arqtxt, "\n");
                destruirPessoa(p);
                removerHashItem(Hpm, cpf);
            } else {
                fprintf(arqtxt, "rip: Pessoa com CPF %s nao encontrada.\n\n", cpf);
            }
        }
        else if (strcmp(comando, "mud") == 0) {
            printf("Iniciando mud");
            char cpf[20], cep[10], face_str[15], cmpl[100];
            int num;
            fscanf(arqqry, "%s %s %s %d %s", cpf, cep, face_str, &num, cmpl);
            
            char face = face_str[0];
            if (strcmp(face_str, "Face.N") == 0 || strcmp(face_str, "N") == 0) face = 'N';
            else if (strcmp(face_str, "Face.S") == 0 || strcmp(face_str, "S") == 0) face = 'S';
            else if (strcmp(face_str, "Face.L") == 0 || strcmp(face_str, "L") == 0 || strcmp(face_str, "Face.E") == 0 || strcmp(face_str, "E") == 0) face = 'L';
            else if (strcmp(face_str, "Face.W") == 0 || strcmp(face_str, "W") == 0 || strcmp(face_str, "Face.O") == 0 || strcmp(face_str, "O") == 0) face = 'O';

            Pessoa p = (Pessoa)buscarHashItem(Hpm, cpf);
            if (p) {
                atribuirMoradia(p, cep, face, num, cmpl);
                atualizarHashItem(&Hpm, p, getCpf(p));
                destruirPessoa(p);
            }
            
            Quadra q = buscarHashItem(Hgeo, cep);
            if (q) {
                desenhaMudancaSVG(q, face, num, cpf, arqsvg);
                destruirQuadra(q);
            }

            fprintf(arqtxt, "Mudança: CPF %s para CEP %s, Face %s, Num %d, Compl %s\n", cpf, cep, face_str, num, cmpl);
            printf("Terminado mud\n");
        }
        else if (strcmp(comando, "dspj") == 0) {
            char cpf[20];
            fscanf(arqqry, "%s", cpf);
            Pessoa p = (Pessoa)buscarHashItem(Hpm, cpf);
            if (p) {
                fprintf(arqtxt, "Despejo (dspj):\n");
                fprintf(arqtxt, "CPF: %s\n", getCpf(p));
                fprintf(arqtxt, "Nome: %s %s\n", getNome(p), getSobrenome(p));
                
                if (temMoradia(p)) {
                    char cep[15];
                    strcpy(cep, getMoradiaCep(p));
                    char face = getMoradiaFace(p);
                    int num = getMoradiaNum(p);
                    fprintf(arqtxt, "Moradia Anterior: CEP %s, Face %c, Num %d, Compl %s\n", 
                            cep, face, num, getMoradiaComplemento(p));
                            
                    Quadra q = buscarHashItem(Hgeo, cep);
                    if (q) {
                        desenhaCirculoPretoSVG(q, face, num, arqsvg);
                        destruirQuadra(q);
                    }
                    
                    removerMoradia(p);
                    atualizarHashItem(&Hpm, p, getCpf(p));
                } else {
                    fprintf(arqtxt, "Moradia: A pessoa ja era sem-teto.\n");
                }
                fprintf(arqtxt, "\n");
                destruirPessoa(p);
            } else {
                fprintf(arqtxt, "dspj: Pessoa com CPF %s nao encontrada.\n\n", cpf);
            }
        }
    } while (1);
    fprintf(arqsvg, "</svg>\n");
}