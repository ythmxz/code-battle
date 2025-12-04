#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Jogador *jogador_criar(const char *nome, int id, No *inicio) {
    Jogador *j = malloc(sizeof(Jogador));
    strncpy(j->nome, nome, 31);
    j->nome[31] = '\0';
    j->id = id;
    j->atual = inicio;
    j->inventario = lista_criar();
    j->historico = pilha_criar();
    j->chave1_dominada = 0;
    j->chave2_dominada = 0;

    if (inicio) {
        inicio->ocupado = id;
        inicio->escudo = 1; // spawn começa com 1 escudo
    }

    return j;
}

void jogador_destruir(Jogador *j) {
    if (!j)
        return;
    lista_destruir(j->inventario);
    pilha_destruir(j->historico);
    free(j);
}

void jogador_imprimir_status(Jogador *j) {
    printf("\n=== Status: %s (J%d) ===\n", j->nome, j->id);
    printf("Posicao atual: %s\n", j->atual ? j->atual->nome : "(null)");
    printf("Ataque (espadas): %d\n", jogador_ataque(j));
    printf("Escudos disponiveis: %d\n", jogador_escudos_disponiveis(j));
    printf("Chaves dominadas: %d/2\n", j->chave1_dominada + j->chave2_dominada);
    printf("Inventario:\n");
    lista_imprimir(j->inventario);
    printf("===========================\n");
}

int jogador_ataque(Jogador *j) {
    return lista_contar(j->inventario, "espada");
}

int jogador_escudos_disponiveis(Jogador *j) {
    return lista_contar(j->inventario, "escudo");
}
