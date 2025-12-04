#include "mundo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Cria um novo nó da árvore.
 * @param nome Nome do território
 * @param tipo Tipo do território (NORMAL, RECURSO, CHAVE)
 * @return Ponteiro para o novo Territorio
 */
Territorio *territorio_criar(const char *nome, TipoTerritorio tipo)
{
  Territorio *territorio = malloc(sizeof(Territorio));
  strncpy(territorio->nome, nome, TAMANHO_NOME - 1);
  territorio->nome[TAMANHO_NOME - 1] = '\0';
  territorio->tipo = tipo;
  territorio->ocupado = 0;
  territorio->escudo = 0;
  territorio->pai = NULL;
  territorio->esquerda = NULL;
  territorio->direita = NULL;
  territorio->irmao = NULL;
  return territorio;
}

/**
 * Constrói árvore binária completa de altura 3 (15 nós).
 * Distribui aleatoriamente 2 nós CHAVE e alguns nós RECURSO.
 * @return Raiz da árvore (Nucleo-X)
 */
Territorio *mundo_construir_completa()
{
  // Nível 0 (raiz)
  Territorio *nucleo_x = territorio_criar("Nucleo-X", NORMAL);

  // Nível 1
  Territorio *moonad = territorio_criar("Moonad", NORMAL);
  Territorio *sunad = territorio_criar("Sunad", NORMAL);

  // Nível 2
  Territorio *ounica = territorio_criar("Ounica", NORMAL);
  Territorio *pharia = territorio_criar("Pharia", NORMAL);
  Territorio *iphido = territorio_criar("Iphido", NORMAL);
  Territorio *leles = territorio_criar("Leles", NORMAL);

  // Nível 3 (folhas)
  Territorio *vivus = territorio_criar("Vivus", NORMAL);
  Territorio *irilon = territorio_criar("Irilon", NORMAL);
  Territorio *zealan = territorio_criar("Zealan", NORMAL);
  Territorio *yoria = territorio_criar("Yoria", NORMAL);
  Territorio *muwen = territorio_criar("Muwen", NORMAL);
  Territorio *ozidian = territorio_criar("Ozidian", NORMAL);
  Territorio *gukar = territorio_criar("Gukar", NORMAL);
  Territorio *vitalis = territorio_criar("Vitalis", NORMAL);

  // Conectar estrutura
  nucleo_x->esquerda = moonad;
  nucleo_x->direita = sunad;
  moonad->esquerda = ounica;
  moonad->direita = pharia;
  sunad->esquerda = iphido;
  sunad->direita = leles;
  ounica->esquerda = vivus;
  ounica->direita = irilon;
  pharia->esquerda = zealan;
  pharia->direita = yoria;
  iphido->esquerda = muwen;
  iphido->direita = ozidian;
  leles->esquerda = gukar;
  leles->direita = vitalis;

  // Configurar ponteiros pai e irmão
  arvore_configurar_ponteiros(nucleo_x, NULL);

  // Distribuir aleatoriamente tipos especiais (2 CHAVE + muitos RECURSO)
  // Não aplicar tipos especiais na raiz nem nas folhas extremas (spawn)
  Territorio *candidatos[] = {moonad, sunad, ounica, pharia, iphido, leles, irilon, zealan, yoria, muwen, ozidian, gukar}; // 12 candidatos
  int total_candidatos = 12;

  // Escolher 2 nós para CHAVE
  int idx1 = rand() % total_candidatos;
  int idx2;
  do
  {
    idx2 = rand() % total_candidatos;
  } while (idx2 == idx1);

  candidatos[idx1]->tipo = CHAVE;
  candidatos[idx2]->tipo = CHAVE;

  // Escolher MUITOS nós para RECURSO (7-9 nós, ~70% dos candidatos)
  int num_recursos = 7 + rand() % 3;
  for (int i = 0; i < num_recursos; i++)
  {
    int idx = rand() % total_candidatos;
    if (candidatos[idx]->tipo == NORMAL)
    {
      candidatos[idx]->tipo = RECURSO;
    }
  }

  return nucleo_x;
}

/**
 * Configura ponteiros pai e irmão recursivamente.
 * @param raiz Nó atual
 * @param pai_no Ponteiro para o nó pai
 */
void arvore_configurar_ponteiros(Territorio *raiz, Territorio *pai_no)
{
  if (!raiz)
    return;
  raiz->pai = pai_no;

  // Configurar irmãos
  if (raiz->esquerda && raiz->direita)
  {
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
void mundo_liberar(Territorio *raiz)
{
  if (!raiz)
    return;
  mundo_liberar(raiz->esquerda);
  mundo_liberar(raiz->direita);
  free(raiz);
}

/**
 * Busca a folha mais à esquerda (spawn jogador 1).
 * @param raiz Raiz da árvore
 * @return Folha extrema esquerda
 */
Territorio *buscar_folha_esquerda(Territorio *raiz)
{
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
Territorio *buscar_folha_direita(Territorio *raiz)
{
  if (!raiz)
    return NULL;
  if (!raiz->esquerda && !raiz->direita)
    return raiz;
  return buscar_folha_direita(raiz->direita);
}

/**
 * Função auxiliar para buscar nós CHAVE recursivamente.
 */
static void buscar_chaves_recursivo(Territorio *raiz, Territorio **chave1, Territorio **chave2)
{
  if (!raiz)
    return;

  if (raiz->tipo == CHAVE)
  {
    if (*chave1 == NULL)
    {
      *chave1 = raiz;
    }
    else if (*chave2 == NULL)
    {
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
Territorio *buscar_no_chave(Territorio *raiz, int qual)
{
  Territorio *chave1 = NULL;
  Territorio *chave2 = NULL;

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
void arvore_imprimir(Territorio *raiz, int nivel)
{
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
int calcular_nivel(Territorio *raiz, Territorio *alvo)
{
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
static void coletar_nos_nivel(Territorio *raiz, int nivel_alvo, int nivel_atual, Territorio **lista, int *contador)
{
  if (!raiz || *contador >= 15)
    return;

  if (nivel_atual == nivel_alvo)
  {
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
Territorio *buscar_vizinho_esquerda(Territorio *raiz, Territorio *atual)
{
  if (!raiz || !atual)
    return NULL;

  int nivel = calcular_nivel(raiz, atual);
  if (nivel < 0)
    return NULL;

  // Coletar todos os nós do mesmo nível
  Territorio *nos_nivel[15];
  int contador = 0;
  coletar_nos_nivel(raiz, nivel, 0, nos_nivel, &contador);

  // Encontrar posição do nó atual e retornar o anterior (se existir)
  for (int i = 0; i < contador; i++)
  {
    if (nos_nivel[i] == atual)
    {
      if (i > 0)
      {
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
Territorio *buscar_vizinho_direita(Territorio *raiz, Territorio *atual)
{
  if (!raiz || !atual)
    return NULL;

  int nivel = calcular_nivel(raiz, atual);
  if (nivel < 0)
    return NULL;

  // Coletar todos os nós do mesmo nível
  Territorio *nos_nivel[15];
  int contador = 0;
  coletar_nos_nivel(raiz, nivel, 0, nos_nivel, &contador);

  // Encontrar posição do nó atual e retornar o próximo (se existir)
  for (int i = 0; i < contador; i++)
  {
    if (nos_nivel[i] == atual)
    {
      if (i < contador - 1)
      {
        return nos_nivel[i + 1];
      }
      return NULL; // já é o último
    }
  }

  return NULL;
}
