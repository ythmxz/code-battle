#ifndef STACK_H
#define STACK_H

#include "../tree.h"

/**
 * TAD Pilha
 * Usada para armazenar histórico de movimentação do jogador.
 */

/**
 * Estrutura de um nó da pilha.
 * Cada nó contém um ponteiro para território visitado.
 */
typedef struct NoPilha {
    struct No *valor;
    struct NoPilha *proximo;
} NoPilha;

/**
 * Estrutura da pilha.
 * Mantém ponteiro para o topo.
 */
typedef struct Pilha {
    NoPilha *topo;
} Pilha;

/**
 * Cria uma nova pilha vazia.
 * @return Ponteiro para a pilha criada
 */
Pilha *pilha_criar();

/**
 * Empilha um território no histórico.
 * @param p Ponteiro para a pilha
 * @param v Ponteiro para o nó (território)
 */
void pilha_empilhar(Pilha *p, struct No *v);

/**
 * Desempilha e retorna o último território visitado.
 * @param p Ponteiro para a pilha
 * @return Ponteiro para o nó desempilhado (NULL se vazia)
 */
struct No *pilha_desempilhar(Pilha *p);

/**
 * Consulta o topo da pilha sem remover.
 * @param p Ponteiro para a pilha
 * @return Ponteiro para o nó no topo (NULL se vazia)
 */
struct No *pilha_topo(Pilha *p);

/**
 * Verifica se a pilha está vazia.
 * @param p Ponteiro para a pilha
 * @return 1 se vazia, 0 caso contrário
 */
int pilha_vazia(Pilha *p);

/**
 * Libera memória da pilha.
 * @param p Ponteiro para a pilha
 */
void pilha_destruir(Pilha *p);

#endif // STACK_H
