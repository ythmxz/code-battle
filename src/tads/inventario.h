#ifndef INVENTARIO_H
#define INVENTARIO_H

/// @file inventario.h
/// @brief Definições e funções para o inventário de itens.

/// @brief Estrutura de um item no inventário.
typedef struct Item {
    char *nome;
    struct Item *proximo;
} Item;

/// @brief Estrutura do inventário, contendo ponteiro para o primeiro item.
typedef struct Inventario {
    Item *cabeca;
} Inventario;

/// @brief Inicializa um novo inventário vazio.
/// @return Ponteiro para o inventário criado
Inventario *inicializarInventario();

/// @brief Adiciona um item ao inventário.
/// @param inventario Ponteiro para o inventário
/// @param nomeItem Nome do recurso a adicionar
void adicionarItem(Inventario *inventario, const char *nomeItem);

/// @brief Verifica se o inventário contém um item específico.
/// @param inventario Ponteiro para o inventário
/// @param nomeItem Nome do recurso a buscar
/// @return 1 se encontrado, 0 caso contrário
int inventarioContem(Inventario *inventario, const char *nomeItem);

/// @brief Conta quantas ocorrências de um item existem no inventário.
/// @param inventario Ponteiro para o inventário
/// @param nomeItem Nome do recurso a contar
/// @return Quantidade de ocorrências do item
int contarItem(Inventario *inventario, const char *nomeItem);

/// @brief Remove uma ocorrência de um item do inventário.
/// @param inventario Ponteiro para o inventário
/// @param nomeItem Nome do recurso a ser removido
/// @return 1 se removido, 0 se não encontrado
int removerItem(Inventario *inventario, const char *nomeItem);

/// @brief Imprime todos os itens do inventário.
/// @param inventario Ponteiro para o inventário a ser impresso.
void imprimirInventario(Inventario *inventario);

/// @brief Libera a memória alocada para o inventário.
/// @param inventario Ponteiro para o inventário a ser destruído.
void destruirInventario(Inventario *inventario);

#endif // INVENTARIO_H
