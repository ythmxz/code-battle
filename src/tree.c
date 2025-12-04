#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Cria um novo nó da árvore.
 * @param nome Nome do território
 * @param tipo Tipo do território (NORMAL, RECURSO, CHAVE)
 * @return Ponteiro para o novo nó
 */
No *no_criar(const char *nome, TipoTerritorio tipo) {
    No *n = malloc(sizeof(No));
    strncpy(n->nome, nome, TAMANHO_NOME - 1);
    n->nome[TAMANHO_NOME - 1] = '\0';
    n->tipo = tipo;
    n->ocupado = 0;
    n->escudo = 0;
    n->pai = NULL;
    n->esquerda = NULL;
    n->direita = NULL;
    n->irmao = NULL;
    return n;
}

/**
 * Constrói árvore binária completa de altura 3 (15 nós).
 * Distribui aleatoriamente 2 nós CHAVE e alguns nós RECURSO.
 * @return Raiz da árvore (Nucleo-X)
 */
No *arvore_construir_completa() {
    // Nível 0 (raiz)
    No *n1 = no_criar("Nucleo-X", NORMAL);

    // Nível 1
    No *n2 = no_criar("A", NORMAL);
    No *n3 = no_criar("B", NORMAL);

    // Nível 2
    No *n4 = no_criar("C", NORMAL);
    No *n5 = no_criar("D", NORMAL);
    No *n6 = no_criar("E", NORMAL);
    No *n7 = no_criar("F", NORMAL);

    // Nível 3 (folhas)
    No *n8 = no_criar("G", NORMAL);
    No *n9 = no_criar("H", NORMAL);
    No *n10 = no_criar("I", NORMAL);
    No *n11 = no_criar("J", NORMAL);
    No *n12 = no_criar("K", NORMAL);
    No *n13 = no_criar("L", NORMAL);
    No *n14 = no_criar("M", NORMAL);
    No *n15 = no_criar("N", NORMAL);

    // Conectar estrutura
    n1->esquerda = n2;
    n1->direita = n3;
    n2->esquerda = n4;
    n2->direita = n5;
    n3->esquerda = n6;
    n3->direita = n7;
    n4->esquerda = n8;
    n4->direita = n9;
    n5->esquerda = n10;
    n5->direita = n11;
    n6->esquerda = n12;
    n6->direita = n13;
    n7->esquerda = n14;
    n7->direita = n15;

    // Configurar ponteiros pai e irmão
    arvore_configurar_ponteiros(n1, NULL);

    // Distribuir aleatoriamente tipos especiais (2 CHAVE + muitos RECURSO)
    // Não aplicar tipos especiais na raiz nem nas folhas extremas (spawn)
    No *candidatos[] = {n2, n3, n4, n5, n6, n7, n9, n10, n11, n12, n13, n14}; // 12 candidatos
    int total_candidatos = 12;

    // Escolher 2 nós para CHAVE
    int idx1 = rand() % total_candidatos;
    int idx2;
    do {
        idx2 = rand() % total_candidatos;
    } while (idx2 == idx1);

    candidatos[idx1]->tipo = CHAVE;
    candidatos[idx2]->tipo = CHAVE;

    // Escolher MUITOS nós para RECURSO (7-9 nós, ~70% dos candidatos)
    int num_recursos = 7 + rand() % 3;
    for (int i = 0; i < num_recursos; i++) {
        int idx = rand() % total_candidatos;
        if (candidatos[idx]->tipo == NORMAL) {
            candidatos[idx]->tipo = RECURSO;
        }
    }

    return n1;
}

/**
 * Configura ponteiros pai e irmão recursivamente.
 * @param raiz Nó atual
 * @param pai_no Ponteiro para o nó pai
 */
void arvore_configurar_ponteiros(No *raiz, No *pai_no) {
    if (!raiz)
        return;
    raiz->pai = pai_no;

    // Configurar irmãos
    if (raiz->esquerda && raiz->direita) {
        raiz->esquerda->irmao = raiz->direita;
        raiz->direita->irmao = raiz->esquerda;
    }

    arvore_configurar_ponteiros(raiz->esquerda, raiz);
    arvore_configurar_ponteiros(raiz->direita, raiz);
}

/**
 * Libera memória da árvore recursivamente.
 * @param raiz Raiz da árvore
 */
void arvore_liberar(No *raiz) {
    if (!raiz)
        return;
    arvore_liberar(raiz->esquerda);
    arvore_liberar(raiz->direita);
    free(raiz);
}

/**
 * Busca a folha mais à esquerda (spawn jogador 1).
 * @param raiz Raiz da árvore
 * @return Folha extrema esquerda
 */
No *buscar_folha_esquerda(No *raiz) {
    if (!raiz)
        return NULL;
    if (!raiz->esquerda && !raiz->direita)
        return raiz;
    return buscar_folha_esquerda(raiz->esquerda);
}

/**
 * Busca a folha mais à direita (spawn jogador 2).
 * @param raiz Raiz da árvore
 * @return Folha extrema direita
 */
No *buscar_folha_direita(No *raiz) {
    if (!raiz)
        return NULL;
    if (!raiz->esquerda && !raiz->direita)
        return raiz;
    return buscar_folha_direita(raiz->direita);
}

/**
 * Função auxiliar para buscar nós CHAVE recursivamente.
 */
static void buscar_chaves_recursivo(No *raiz, No **chave1, No **chave2) {
    if (!raiz)
        return;

    if (raiz->tipo == CHAVE) {
        if (*chave1 == NULL) {
            *chave1 = raiz;
        } else if (*chave2 == NULL) {
            *chave2 = raiz;
        }
    }

    buscar_chaves_recursivo(raiz->esquerda, chave1, chave2);
    buscar_chaves_recursivo(raiz->direita, chave1, chave2);
}

/**
 * Busca nó CHAVE (primeira ou segunda ocorrência).
 * @param raiz Raiz da árvore
 * @param qual 1=primeira CHAVE, 2=segunda CHAVE
 * @return Ponteiro para nó CHAVE encontrado
 */
No *buscar_no_chave(No *raiz, int qual) {
    No *chave1 = NULL;
    No *chave2 = NULL;

    buscar_chaves_recursivo(raiz, &chave1, &chave2);

    if (qual == 1)
        return chave1;
    else if (qual == 2)
        return chave2;

    return NULL;
}

/**
 * Imprime árvore de forma hierárquica (debug).
 * @param raiz Raiz da árvore
 * @param nivel Nível de profundidade (para indentação)
 */
void arvore_imprimir(No *raiz, int nivel) {
    if (!raiz)
        return;
    for (int i = 0; i < nivel; i++)
        printf("  ");

    const char *tipo_str[] = {"NORM", "REC", "CHAVE"};
    printf("%s [%s ocp=%d esc=%d]\n", raiz->nome, tipo_str[raiz->tipo], raiz->ocupado, raiz->escudo);

    arvore_imprimir(raiz->esquerda, nivel + 1);
    arvore_imprimir(raiz->direita, nivel + 1);
}

/**
 * Calcula o nível de um nó na árvore.
 * @param raiz Raiz da árvore
 * @param alvo Nó cujo nível será calculado
 * @return Nível do nó (0=raiz, 1=filhos da raiz, 2, 3=folhas) ou -1 se não encontrado
 */
int calcular_nivel(No *raiz, No *alvo) {
    if (!raiz)
        return -1;
    if (raiz == alvo)
        return 0;

    int nivel_esq = calcular_nivel(raiz->esquerda, alvo);
    if (nivel_esq >= 0)
        return nivel_esq + 1;

    int nivel_dir = calcular_nivel(raiz->direita, alvo);
    if (nivel_dir >= 0)
        return nivel_dir + 1;

    return -1;
}

/**
 * Coleta todos os nós de um nível específico (auxiliar).
 */
static void coletar_nos_nivel(No *raiz, int nivel_alvo, int nivel_atual, No **lista, int *contador) {
    if (!raiz || *contador >= 15)
        return;

    if (nivel_atual == nivel_alvo) {
        lista[*contador] = raiz;
        (*contador)++;
        return;
    }

    coletar_nos_nivel(raiz->esquerda, nivel_alvo, nivel_atual + 1, lista, contador);
    coletar_nos_nivel(raiz->direita, nivel_alvo, nivel_atual + 1, lista, contador);
}

/**
 * Busca nó adjacente à esquerda no mesmo nível.
 * @param raiz Raiz da árvore
 * @param atual Nó atual
 * @return Nó adjacente à esquerda ou NULL se não existir
 */
No *buscar_vizinho_esquerda(No *raiz, No *atual) {
    if (!raiz || !atual)
        return NULL;

    int nivel = calcular_nivel(raiz, atual);
    if (nivel < 0)
        return NULL;

    // Coletar todos os nós do mesmo nível
    No *nos_nivel[15];
    int contador = 0;
    coletar_nos_nivel(raiz, nivel, 0, nos_nivel, &contador);

    // Encontrar posição do nó atual e retornar o anterior (se existir)
    for (int i = 0; i < contador; i++) {
        if (nos_nivel[i] == atual) {
            if (i > 0) {
                return nos_nivel[i - 1];
            }
            return NULL; // já é o primeiro
        }
    }

    return NULL;
}

/**
 * Busca nó adjacente à direita no mesmo nível.
 * @param raiz Raiz da árvore
 * @param atual Nó atual
 * @return Nó adjacente à direita ou NULL se não existir
 */
No *buscar_vizinho_direita(No *raiz, No *atual) {
    if (!raiz || !atual)
        return NULL;

    int nivel = calcular_nivel(raiz, atual);
    if (nivel < 0)
        return NULL;

    // Coletar todos os nós do mesmo nível
    No *nos_nivel[15];
    int contador = 0;
    coletar_nos_nivel(raiz, nivel, 0, nos_nivel, &contador);

    // Encontrar posição do nó atual e retornar o próximo (se existir)
    for (int i = 0; i < contador; i++) {
        if (nos_nivel[i] == atual) {
            if (i < contador - 1) {
                return nos_nivel[i + 1];
            }
            return NULL; // já é o último
        }
    }

    return NULL;
}
