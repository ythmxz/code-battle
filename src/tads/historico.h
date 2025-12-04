#ifndef HISTORICO_H
#define HISTORICO_H

#include "../tree.h"

/// @file stack.h
/// @brief Definições e funções para o histórico de ações.

/// @brief Estrutura de uma ação no histórico.
typedef struct Acao {
    struct No *territorio;
    struct Acao *proximo;
} Acao;

/// @brief Estrutura do histórico.
typedef struct Historico {
    Acao *topo;
} Historico;

/// @brief Inicializa um novo histórico vazio.
/// @return Ponteiro para o histórico criado
Historico *inicializarHistorico();

/// @brief Empilha um território no histórico.
/// @param historico Ponteiro para o histórico
/// @param territorio Ponteiro para o território a ser empilhado
void empilharHistorico(Historico *historico, struct No *territorio);

/// @brief Desempilha o território do topo do histórico.
/// @param historico Ponteiro para o histórico
/// @return Ponteiro para o território desempilhado (NULL se vazio)
struct No *desempilharHistorico(Historico *historico);

/// @brief Retorna o território do topo do histórico sem desempilhar.
/// @param historico Ponteiro para o histórico
/// @return Ponteiro para o território do topo (NULL se vazio)
struct No *topoHistorico(Historico *historico);

/// @brief Verifica se o histórico está vazio.
/// @param historico Ponteiro para o histórico
/// @return 1 se vazio, 0 caso contrário
int historicoVazio(Historico *historico);

/// @brief Libera memória do histórico.
/// @param historico Ponteiro para o histórico
void destruirHistorico(Historico *historico);

#endif // HISTORICO_H
