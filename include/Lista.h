#ifndef LISTA
#define LISTA
#include <stdio.h>

//programa para listas.

typedef void* Item; // Define que cada item da lista é uma lista genérica
typedef void* Lista; // Define o tipo Lista

Lista crialista();
//cria a lista

void adicionar(Lista *l, Item item, int tipo);
// Adiciona um item na Lista

Item remover(Lista *l, int *tipo);
// Remove o elemento do fim da lista.

void exibirlista(Lista l);
//exibe os elementos da lista.

int listavazia(Lista l);
//verifica se a lista está vazia

void limpaLista(Lista *l);
//limpa a lista

#endif