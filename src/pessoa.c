#include "pessoa.h"
#include <stdlib.h>
#include <string.h>

struct sPessoa {
    char cpf[16];
    char nome[100];
    char sobrenome[100];
    char sexo;
    char nascimento[11];
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