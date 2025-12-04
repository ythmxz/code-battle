#ifndef MUNDO_H
#define MUNDO_H

#define TAMANHO_NOME 32

/**
 * Tipos de território (nó):
 * - NORMAL: passagem livre
 * - RECURSO: coleta itens (espada, escudo, energia)
 * - CHAVE: necessário dominar ambas para vitória
 */
typedef enum
{
  NORMAL = 0,
  RECURSO = 1,
  CHAVE = 2
} TipoTerritorio;

/**
 * Estrutura de um nó da árvore (território).
 * Cada nó possui:
 * - nome: identificador
 * - tipo: NORMAL, RECURSO ou CHAVE
 * - ocupado: 0=vazio, 1=jogador1, 2=jogador2
 * - escudo: quantidade de escudo no nó
 * - pai, esquerda, direita, irmao: ponteiros para navegação
 */
typedef struct No
{
  char nome[TAMANHO_NOME];
  TipoTerritorio tipo;
  int ocupado; // 0=vazio, 1=jogador1, 2=jogador2
  int escudo;  // proteção do nó (decresce ao ser atacado)
  struct No *pai;
  struct No *esquerda;
  struct No *direita;
  struct No *irmao; // ponteiro para o nó irmão (esquerdo <-> direito)
} Territorio;

// Funções de criação e manipulação da árvore
Territorio *territorio_criar(const char *nome, TipoTerritorio tipo);
Territorio *mundo_construir_completa();
void mundo_liberar(Territorio *raiz);
void mundo_imprimir(Territorio *raiz, int nivel);
void mundo_configurar_ponteiros(Territorio *raiz, Territorio *pai_no);

// Funções de busca
Territorio *buscar_folha_esquerda(Territorio *raiz);
Territorio *buscar_folha_direita(Territorio *raiz);
Territorio *buscar_no_chave(Territorio *raiz, int qual); // qual=1 primeira, qual=2 segunda

// Função utilitária
int calcular_nivel(Territorio *raiz, Territorio *alvo); // retorna nível do nó (0=raiz, 3=folhas)

// Funções para movimento lateral
Territorio *buscar_vizinho_esquerda(Territorio *raiz, Territorio *atual); // busca nó adjacente à esquerda no mesmo nível
Territorio *buscar_vizinho_direita(Territorio *raiz, Territorio *atual);  // busca nó adjacente à direita no mesmo nível

#endif // MUNDO_H
