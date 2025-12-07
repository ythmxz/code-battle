#include "jogador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Jogador *criarJogador(const char *nome, int id, Territorio *inicio) {
    Jogador *jogador = (Jogador *)malloc(sizeof(Jogador));

    if (jogador == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o jogador.\n");
        exit(1);
    }

    strncpy(jogador->nome, nome, 31);
    jogador->nome[31] = '\0';
    jogador->id = id;
    jogador->atual = inicio;
    jogador->inventario = inicializarInventario();
    jogador->historico = inicializarHistorico();
    jogador->chave1Dominada = 0;
    jogador->chave2Dominada = 0;

    if (inicio != NULL) {
        inicio->ocupado = id;
        inicio->escudo = 1;
    }

    return jogador;
}

void destruirJogador(Jogador *jogador) {
    if (jogador == NULL) {
        return;
    }

    destruirInventario(jogador->inventario);
    destruirHistorico(jogador->historico);

    free(jogador);
}

void imprimirJogador(Jogador *jogador) {
    printf("Status: %s (J%d)\n", jogador->nome, jogador->id);
    printf("Território: %s\n", jogador->atual->nome);
    printf("Ataque: %d\n", ataque(jogador));
    printf("Escudos: %d\n", escudos(jogador));
    printf("Chaves: %d/2\n\n", jogador->chave1Dominada + jogador->chave2Dominada);
    printf("Inventário:\n\n");
    imprimirInventario(jogador->inventario);
}

int ataque(Jogador *jogador) {
    return contarItem(jogador->inventario, "Espada");
}

int escudos(Jogador *jogador) {
    return contarItem(jogador->inventario, "Escudo");
}
