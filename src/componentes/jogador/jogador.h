#ifndef JOGADOR_H
#define JOGADOR_H

#include "componentes/historico/historico.h"
#include "componentes/inventario/inventario.h"
#include "componentes/mundo/mundo.h"

/// @brief Estrutura que representa um jogador no jogo.
typedef struct Jogador {
    char nome[32];
    int id;
    Territorio *atual;
    Inventario *inventario;
    Historico *historico;
    int chave1_dominada;
    int chave2_dominada;
} Jogador;

/// @brief Cria um novo jogador.
/// @param nome Nome do jogador
/// @param id Identificador do jogador
/// @param inicio Território inicial do jogador
/// @return Ponteiro para o jogador criado
Jogador *criarJogador(const char *nome, int id, Territorio *inicio);

/// @brief Calcula o ataque do jogador.
/// @param jogador Ponteiro para o jogador
/// @return Valor do ataque
int ataque(Jogador *jogador);

/// @brief Calcula a quantidade de escudos do jogador.
/// @param jogador Ponteiro para o jogador
/// @return Quantidade de escudos
int escudos(Jogador *jogador);

/// @brief Imprime as informações do jogador.
/// @param jogador Ponteiro para o jogador
void imprimirJogador(Jogador *jogador);

/// @brief Destrói o jogador e libera a memória associada.
/// @param jogador Ponteiro para o jogador
void destruirJogador(Jogador *jogador);

#endif // JOGADOR_H
