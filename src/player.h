#ifndef PLAYER_H
#define PLAYER_H

#include "tads/list.h"
#include "tads/stack.h"
#include "tree.h"

/**
 * Estrutura do jogador.
 * Contém:
 * - nome e id
 * - posição atual na árvore
 * - inventário (lista de recursos)
 * - histórico de movimentação (pilha)
 * - flags de controle (chaves dominadas)
 */
typedef struct Jogador {
    char nome[32];
    int id; // 1 ou 2
    No *atual;
    Lista *inventario;
    Pilha *historico;
    int chave1_dominada; // 1 se dominou a primeira CHAVE
    int chave2_dominada; // 1 se dominou a segunda CHAVE
} Jogador;

/**
 * Cria um novo jogador.
 * @param nome Nome do jogador
 * @param id ID do jogador (1 ou 2)
 * @param inicio Nó inicial (spawn)
 * @return Ponteiro para jogador criado
 */
Jogador *jogador_criar(const char *nome, int id, No *inicio);

/**
 * Libera memória do jogador.
 * @param j Ponteiro para o jogador
 */
void jogador_destruir(Jogador *j);

/**
 * Imprime status completo do jogador.
 * @param j Ponteiro para o jogador
 */
void jogador_imprimir_status(Jogador *j);

/**
 * Calcula ataque total do jogador (quantidade de espadas).
 * @param j Ponteiro para o jogador
 * @return Valor de ataque
 */
int jogador_ataque(Jogador *j);

/**
 * Calcula escudos disponíveis no inventário.
 * @param j Ponteiro para o jogador
 * @return Quantidade de escudos
 */
int jogador_escudos_disponiveis(Jogador *j);

#endif // PLAYER_H
