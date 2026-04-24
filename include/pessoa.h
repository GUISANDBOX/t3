#ifndef PESSOA_H
#define PESSOA_H
#include <stdio.h>

//programa para pessoas.

typedef void *Pessoa;

//cria a pessoa com suas informações
Pessoa criaPessoa(const char *cpf, const char *nome, const char *sobrenome, char sexo, const char *nascimento);

//destrói a pessoa
void destruirPessoa(Pessoa p);

// getters
char* getCpf(Pessoa p);
char* getNome(Pessoa p);
char* getSobrenome(Pessoa p);
char getSexo(Pessoa p);
char* getNascimento(Pessoa p);

// moradia
void atribuirMoradia(Pessoa p, const char *cep, char face, int num, const char *complemento);
int temMoradia(Pessoa p);
char* getMoradiaCep(Pessoa p);
char getMoradiaFace(Pessoa p);
int getMoradiaNum(Pessoa p);
char* getMoradiaComplemento(Pessoa p);

size_t tamanhoPessoa(void);

void printPessoa(Pessoa p);

#endif