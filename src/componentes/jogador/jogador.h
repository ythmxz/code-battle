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
/// @param nome
/// @param id
/// @param inicio
/// @return
Jogador *criarJogador(const char *nome, int id, Territorio *inicio);

/// @brief
/// @param jogador
/// @return
int ataque(Jogador *jogador);

/// @brief
/// @param jogador
/// @return
int escudos(Jogador *jogador);

/// @brief
/// @param jogador
void imprimirJogador(Jogador *jogador);

/// @brief
/// @param jogador
void destruirJogador(Jogador *jogador);

#endif // JOGADOR_H
