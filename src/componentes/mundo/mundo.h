#ifndef MUNDO_H
#define MUNDO_H

#define TAMANHO_NOME 32

/// @file mundo.h
/// @brief Definições e funções para a árvore de territórios.

/// @brief Tipos de territórios.
typedef enum { NORMAL = 0, RECURSO = 1, CHAVE = 2, BATALHA = 3 } TipoTerritorio;

/// @brief Estrutura de um território.
typedef struct Territorio {
    char nome[TAMANHO_NOME];
    TipoTerritorio tipo;
    int ocupado; // 0 = VAZIO, 1 = JOGADOR1, 2 = JOGADOR2
    int escudo;
    struct Territorio *pai;
    struct Territorio *esquerda;
    struct Territorio *direita;
} Territorio;

/// @brief Cria um novo território.
/// @param nome Nome do território
/// @param tipo Tipo do território
/// @return Ponteiro para o território criado
Territorio *criarTerritorio(const char *nome, TipoTerritorio tipo);

/// @brief Cria o mundo inicial com a estrutura de territórios.
/// @return Ponteiro para o território raiz do mundo criado
Territorio *criarMundo();

/// @brief Configura ponteiros pai e irmãos na árvore do mundo.
/// @param territorio Ponteiro para o território atual
/// @param territorioPai Ponteiro para o território pai
void configurarPonteirosMundo(Territorio *territorio, Territorio *territorioPai);

/// @brief Busca o território mais à esquerda.
/// @param territorio Ponteiro para o território raiz da busca
/// @return Ponteiro para o território mais à esquerda
Territorio *buscarBaseEsquerda(Territorio *territorio);

/// @brief Busca o território mais à direita.
/// @param territorio Ponteiro para o território raiz da busca
/// @return Ponteiro para o território mais à direita
Territorio *buscarBaseDireita(Territorio *territorio);

/// @brief Busca o território CHAVE especificado.
/// @param territorio Ponteiro para o território raiz da busca
/// @param chave Número da chave (1 ou 2)
/// @return Ponteiro para o território da chave especificada
Territorio *buscarChave(Territorio *territorio, int chave);

/// @brief Calcula o nível de um território na árvore.
/// @param territorio Ponteiro para o território alvo
/// @param nucleo Ponteiro para o território raiz (Núcleo-X)
/// @return Nível do território (0 = Raiz, 3 = Folhas)
int nivelTerritorio(Territorio *territorio, Territorio *nucleo);

/// @brief Busca o território irmão à esquerda no mesmo nível.
/// @param territorio Ponteiro para o território raiz da busca
/// @param atual Ponteiro para o território atual
/// @return Ponteiro para o território irmão à esquerda
Territorio *buscarIrmaoEsquerda(Territorio *territorio, Territorio *atual);

/// @brief Busca o território irmão à direita no mesmo nível.
/// @param territorio Ponteiro para o território raiz da busca
/// @param atual Ponteiro para o território atual
/// @return Ponteiro para o território irmão à direita
Territorio *buscarIrmaoDireita(Territorio *territorio, Territorio *atual);

/// @brief Imprime a estrutura do mundo a partir de um território.
/// @param territorio Ponteiro para o território raiz da impressão
/// @param nivel Nível atual na árvore (para indentação)
void imprimirMundo(Territorio *territorio, int nivel);

/// @brief Destrói a estrutura do mundo a partir de um território.
/// @param territorio Ponteiro para o território raiz da destruição
void destruirMundo(Territorio *territorio);

#endif // MUNDO_H
