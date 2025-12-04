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
    jogador->chave1_dominada = 0;
    jogador->chave2_dominada = 0;

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
    printf("\n=== Status: %s (J%d) ===\n", jogador->nome, jogador->id);
    printf("Território: %s\n", jogador->atual->nome);
    printf("Ataque: %d\n", ataque(jogador));
    printf("Escudos: %d\n", escudos(jogador));
    printf("Chaves: %d/2\n", jogador->chave1_dominada + jogador->chave2_dominada);
    printf("Inventário:\n");
    imprimirInventario(jogador->inventario);
    printf("===========================\n");
}

int ataque(Jogador *jogador) {
    return contarItem(jogador->inventario, "espada");
}

int escudos(Jogador *jogador) {
    return contarItem(jogador->inventario, "escudo");
}
