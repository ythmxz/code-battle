#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inventario.h"

Inventario *inicializarInventario() {
    Inventario *inventario = (Inventario *)malloc(sizeof(Inventario));

    if (inventario == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o inventário.\n");
        exit(1);
    }

    inventario->cabeca = NULL;

    return inventario;
}

void adicionarItem(Inventario *inventario, const char *nomeItem) {
    Item *itemNovo = (Item *)malloc(sizeof(Item));

    if (itemNovo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o item.\n");
        exit(1);
    }

    itemNovo->nome = (char *)malloc(strlen(nomeItem) + 1);

    if (itemNovo->nome == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nome do item.\n");
        free(itemNovo);
        exit(1);
    }

    strcpy(itemNovo->nome, nomeItem);

    itemNovo->proximo = inventario->cabeca;
    inventario->cabeca = itemNovo;
}

int inventarioContem(Inventario *inventario, const char *nomeItem) {
    Item *itemAux = inventario->cabeca;

    while (itemAux != NULL) {
        if (strcmp(itemAux->nome, nomeItem) == 0) {
            return 1;
        }

        itemAux = itemAux->proximo;
    }

    return 0;
}

int contarItem(Inventario *inventario, const char *nome) {
    Item *itemAux = inventario->cabeca;
    int contador = 0;

    while (itemAux != NULL) {
        if (strcmp(itemAux->nome, nome) == 0) {
            contador++;
        }

        itemAux = itemAux->proximo;
    }

    return contador;
}

int removerItem(Inventario *inventario, const char *nome) {
    Item *itemAtual = inventario->cabeca;
    Item *itemAnterior = NULL;

    while (itemAtual != NULL) {
        if (strcmp(itemAtual->nome, nome) == 0) {
            if (itemAnterior != NULL) {
                itemAnterior->proximo = itemAtual->proximo;
            }

            else {
                inventario->cabeca = itemAtual->proximo;
            }

            free(itemAtual->nome);
            free(itemAtual);

            return 1;
        }

        itemAnterior = itemAtual;
        itemAtual = itemAtual->proximo;
    }

    return 0;
}

void imprimirInventario(Inventario *inventario) {
    Item *itemAux = inventario->cabeca;

    if (itemAux == NULL) {
        printf("INVENTÁRIO VAZIO!\n");
        return;
    }

    while (itemAux != NULL) {
        printf("- %s\n", itemAux->nome);
        itemAux = itemAux->proximo;
    }
}

void destruirInventario(Inventario *inventario) {
    Item *itemAux = inventario->cabeca;

    while (itemAux != NULL) {
        Item *itemProximo = itemAux->proximo;

        free(itemAux->nome);
        free(itemAux);

        itemAux = itemProximo;
    }

    free(inventario);
}
