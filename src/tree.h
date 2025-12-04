#ifndef TREE_H
#define TREE_H

#define TAMANHO_NOME 32

/**
 * Tipos de território (nó):
 * - NORMAL: passagem livre
 * - RECURSO: coleta itens (espada, escudo, energia)
 * - CHAVE: necessário dominar ambas para vitória
 */
typedef enum { NORMAL = 0, RECURSO = 1, CHAVE = 2 } TipoTerritorio;

/**
 * Estrutura de um nó da árvore (território).
 * Cada nó possui:
 * - nome: identificador
 * - tipo: NORMAL, RECURSO ou CHAVE
 * - ocupado: 0=vazio, 1=jogador1, 2=jogador2
 * - escudo: quantidade de escudo no nó
 * - pai, esquerda, direita, irmao: ponteiros para navegação
 */
typedef struct No {
    char nome[TAMANHO_NOME];
    TipoTerritorio tipo;
    int ocupado; // 0=vazio, 1=jogador1, 2=jogador2
    int escudo;  // proteção do nó (decresce ao ser atacado)
    struct No *pai;
    struct No *esquerda;
    struct No *direita;
    struct No *irmao; // ponteiro para o nó irmão (esquerdo <-> direito)
} No;

// Funções de criação e manipulação da árvore
No *no_criar(const char *nome, TipoTerritorio tipo);
No *arvore_construir_completa();
void arvore_liberar(No *raiz);
void arvore_imprimir(No *raiz, int nivel);
void arvore_configurar_ponteiros(No *raiz, No *pai_no);

// Funções de busca
No *buscar_folha_esquerda(No *raiz);
No *buscar_folha_direita(No *raiz);
No *buscar_no_chave(No *raiz, int qual); // qual=1 primeira, qual=2 segunda

// Função utilitária
int calcular_nivel(No *raiz, No *alvo); // retorna nível do nó (0=raiz, 3=folhas)

// Funções para movimento lateral
No *buscar_vizinho_esquerda(No *raiz, No *atual); // busca nó adjacente à esquerda no mesmo nível
No *buscar_vizinho_direita(No *raiz, No *atual);  // busca nó adjacente à direita no mesmo nível

#endif // TREE_H
