#include "pessoa.h"
#include <stdlib.h>
#include <string.h>

struct sMoradia {
    char cep[10];
    char face;
    int num;
    char complemento[100];
};

struct sPessoa {
    char cpf[16];
    char nome[100];
    char sobrenome[100];
    char sexo;
    char nascimento[11];
    char temMoradia;
    struct sMoradia moradia;
};

typedef struct sPessoa sPessoa;

size_t tamanhoPessoa(void) {
    return sizeof(sPessoa);
}

Pessoa criaPessoa(const char *cpf, const char *nome, const char *sobrenome, char sexo, const char *nascimento) {
    sPessoa* nova = (sPessoa*)malloc(sizeof(sPessoa));
    if (!nova) {
        printf("Erro na alocação de pessoa\n");
        return NULL;
    }
    strcpy(nova->cpf, cpf);
    strcpy(nova->nome, nome);
    strcpy(nova->sobrenome, sobrenome);
    nova->sexo = sexo;
    strcpy(nova->nascimento, nascimento);
    nova->temMoradia = 0;
    nova->moradia.cep[0] = '\0';
    nova->moradia.face = '\0';
    nova->moradia.num = 0;
    nova->moradia.complemento[0] = '\0';
    return nova;
}

void destruirPessoa(Pessoa p) {
    free(p);
}

char* getCpf(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->cpf;
}

char* getNome(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->nome;
}

char* getSobrenome(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->sobrenome;
}

char getSexo(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->sexo;
}

char* getNascimento(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->nascimento;
}

void atribuirMoradia(Pessoa p, const char *cep, char face, int num, const char *complemento) {
    if (!p) return;
    sPessoa* pessoa = (sPessoa*)p;
    pessoa->temMoradia = 1;
    strncpy(pessoa->moradia.cep, cep, sizeof(pessoa->moradia.cep) - 1);
    pessoa->moradia.cep[sizeof(pessoa->moradia.cep) - 1] = '\0';
    pessoa->moradia.face = face;
    pessoa->moradia.num = num;
    strncpy(pessoa->moradia.complemento, complemento, sizeof(pessoa->moradia.complemento) - 1);
    pessoa->moradia.complemento[sizeof(pessoa->moradia.complemento) - 1] = '\0';
}

int temMoradia(Pessoa p) {
    if (!p) return 0;
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->temMoradia != 0;
}

char* getMoradiaCep(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->moradia.cep;
}

char getMoradiaFace(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->moradia.face;
}

int getMoradiaNum(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->moradia.num;
}

char* getMoradiaComplemento(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    return pessoa->moradia.complemento;
}

void printPessoa(Pessoa p) {
    sPessoa* pessoa = (sPessoa*)p;
    printf("CPF: %s\n", pessoa->cpf);
    printf("Nome: %s %s\n", pessoa->nome, pessoa->sobrenome);
    printf("Sexo: %c\n", pessoa->sexo);
    printf("Nascimento: %s\n", pessoa->nascimento);
    if (pessoa->temMoradia) {
        printf("Moradia: CEP=%s, face=%c, num=%d, complemento=%s\n",
               pessoa->moradia.cep,
               pessoa->moradia.face,
               pessoa->moradia.num,
               pessoa->moradia.complemento);
    } else {
        printf("Moradia: sem moradia\n");
    }
}