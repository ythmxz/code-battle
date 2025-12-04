#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Lista *lista_criar() {
    Lista *l = malloc(sizeof(Lista));
    l->cabeca = NULL;
    return l;
}

void lista_adicionar(Lista *l, const char *nome) {
    Item *it = malloc(sizeof(Item));
    it->nome = malloc(strlen(nome) + 1);
    strcpy(it->nome, nome);
    it->proximo = l->cabeca;
    l->cabeca = it;
}

int lista_contem(Lista *l, const char *nome) {
    Item *atual = l->cabeca;
    while (atual) {
        if (strcmp(atual->nome, nome) == 0)
            return 1;
        atual = atual->proximo;
    }
    return 0;
}

int lista_contar(Lista *l, const char *nome) {
    int contador = 0;
    Item *atual = l->cabeca;
    while (atual) {
        if (strcmp(atual->nome, nome) == 0)
            contador++;
        atual = atual->proximo;
    }
    return contador;
}

int lista_remover(Lista *l, const char *nome) {
    Item *atual = l->cabeca;
    Item *anterior = NULL;

    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            if (anterior) {
                anterior->proximo = atual->proximo;
            } else {
                l->cabeca = atual->proximo;
            }
            free(atual->nome);
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return 0;
}

void lista_imprimir(Lista *l) {
    Item *atual = l->cabeca;
    if (!atual) {
        printf("  (vazio)\n");
        return;
    }
    while (atual) {
        printf("  - %s\n", atual->nome);
        atual = atual->proximo;
    }
}

void lista_destruir(Lista *l) {
    Item *atual = l->cabeca;
    while (atual) {
        Item *proximo = atual->proximo;
        free(atual->nome);
        free(atual);
        atual = proximo;
    }
    free(l);
}
