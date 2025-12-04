#ifndef LIST_H
#define LIST_H

/**
 * TAD Lista Encadeada
 * Usada para armazenar inventário do jogador (espadas, escudos, energia).
 */

/**
 * Estrutura de um item da lista.
 * Cada item contém um nome (recurso) e ponteiro para próximo.
 */
typedef struct Item {
    char *nome;
    struct Item *proximo;
} Item;

/**
 * Estrutura da lista encadeada.
 * Mantém ponteiro para o primeiro item (cabeça).
 */
typedef struct Lista {
    Item *cabeca;
} Lista;

/**
 * Cria uma nova lista vazia.
 * @return Ponteiro para a lista criada
 */
Lista *lista_criar();

/**
 * Adiciona um item ao inventário.
 * @param l Ponteiro para a lista
 * @param nome Nome do recurso a adicionar
 */
void lista_adicionar(Lista *l, const char *nome);

/**
 * Verifica se a lista contém um item específico.
 * @param l Ponteiro para a lista
 * @param nome Nome do recurso a procurar
 * @return 1 se encontrado, 0 caso contrário
 */
int lista_contem(Lista *l, const char *nome);

/**
 * Conta quantas ocorrências de um item existem na lista.
 * @param l Ponteiro para a lista
 * @param nome Nome do recurso a contar
 * @return Quantidade de ocorrências
 */
int lista_contar(Lista *l, const char *nome);

/**
 * Remove uma ocorrência de um item da lista.
 * @param l Ponteiro para a lista
 * @param nome Nome do recurso a remover
 * @return 1 se removido, 0 se não encontrado
 */
int lista_remover(Lista *l, const char *nome);

/**
 * Imprime todos os itens da lista.
 * @param l Ponteiro para a lista
 */
void lista_imprimir(Lista *l);

/**
 * Libera memória da lista e todos seus itens.
 * @param l Ponteiro para a lista
 */
void lista_destruir(Lista *l);

#endif // LIST_H
