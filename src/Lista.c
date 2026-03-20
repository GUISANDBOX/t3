#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista.h"

struct elemento {
    Item item;
    int tipo; // 0 - char*, 1 - int, 2 - double
    struct elemento *prox;
};

typedef struct elemento sNoItem;

void adicionar(Lista *f, Item item, int tipo) {
    sNoItem **inicio = (sNoItem **)f;

    sNoItem *novo = (sNoItem *)malloc(sizeof(sNoItem));
    if(!novo){
        printf("Erro na alocação\n");
        return;
    }

    novo->item = item;
    novo->tipo = tipo;
    novo->prox = NULL;

    // Se a lista está vazia, novo é o primeiro
    if (*inicio == NULL) {
        *inicio = novo;
        return;
    }

    // Percorre até o último
    sNoItem *atual = *inicio;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }

    // Insere no final
    atual->prox = novo;
}


Item remover(Lista *f, int *tipo) {
    sNoItem **inicio = (sNoItem **)f;

    if (*inicio == NULL) { // Evita remover de lista vazia
        printf("Lista vazia!\n");
        return NULL;
    }

    sNoItem *atual = *inicio;
    sNoItem *anterior = NULL;

    // Percorre até o último nó
    while (atual->prox != NULL) {
        anterior = atual;
        atual = atual->prox;
    }

    // Se só havia um elemento
    if (anterior == NULL) {
        *inicio = NULL; // lista agora fica vazia
    } else {
        anterior->prox = NULL; // o penúltimo vira o último
    }

    Item removido = atual->item;
    *tipo = atual->tipo;
    free(atual); // desaloca o nó removido
    return removido;
}

void exibirlista(Lista f){
    sNoItem *atual= (sNoItem *)f;
    while(atual!=NULL){
        if (atual->tipo==0) {
            printf("%s\n", (char *)atual->item);
        }
        else if (atual->tipo==1) {
            printf("\n\nInt:\n");
        }
        else if (atual->tipo==2) {
            printf("\n\nDouble:\n");
        }
        atual=atual->prox;
    }
    printf("\n");
}

Lista crialista(){
    return NULL;
}

int listavazia(Lista l) {
    sNoItem *atual= (sNoItem *)l;
    if(atual==NULL){
        return 1;
    }
    return 0;
}

void limpaLista(Lista *l) {
    if (l == NULL) {
        return; // Lista já é nula, nada a limpar
    }
    sNoItem **inicio = (sNoItem **)l;
    sNoItem *atual = *inicio;
    sNoItem *prox;

    while (atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    *inicio = NULL; // Define a lista como vazia após limpar
}