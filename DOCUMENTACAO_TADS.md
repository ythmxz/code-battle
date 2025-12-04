/**
 * DOCUMENTAÇÃO TÉCNICA - TADs
 * Code Battle - Tipos Abstratos de Dados
 */

// =============================================================================
// TAD LISTA ENCADEADA (src/tads/list.h)
// =============================================================================

/**
 * Estrutura: Lista encadeada simples (inserção no início)
 * Uso: Armazenar inventário do jogador (espadas, escudos, energia)
 */

// EXEMPLO DE USO:

#include "tads/list.h"

Lista *inv = lista_criar();

// Adicionar itens
lista_adicionar(inv, "espada");
lista_adicionar(inv, "escudo");
lista_adicionar(inv, "espada"); // pode ter duplicatas

// Verificar existência
if (lista_contem(inv, "espada")) {
    printf("Jogador possui espada!\n");
}

// Contar ocorrências
int num_espadas = lista_contar(inv, "espada"); // retorna 2

// Remover uma ocorrência
lista_remover(inv, "espada"); // remove apenas 1 espada
// Agora tem: 1 espada, 1 escudo

// Imprimir todos os itens
lista_imprimir(inv);
// Saída:
//   - escudo
//   - espada

// Liberar memória
lista_destruir(inv);


// =============================================================================
// TAD PILHA (src/tads/stack.h)
// =============================================================================

/**
 * Estrutura: Pilha (LIFO - Last In, First Out)
 * Uso: Armazenar histórico de movimentação do jogador
 */

// EXEMPLO DE USO:

#include "tads/stack.h"

Pilha *hist = pilha_criar();

// Empilhar nós visitados
pilha_empilhar(hist, no_atual);
pilha_empilhar(hist, proximo_no);

// Verificar topo sem remover
No *ultimo = pilha_topo(hist); // retorna proximo_no

// Desempilhar (remover e retornar)
No *voltar = pilha_desempilhar(hist); // retorna proximo_no
// Agora topo é: no_atual

// Verificar se está vazia
if (pilha_vazia(hist)) {
    printf("Sem histórico de movimentos\n");
}

// Liberar memória
pilha_destruir(hist);


// =============================================================================
// TAD ÁRVORE BINÁRIA (src/tree.h)
// =============================================================================

/**
 * Estrutura: Árvore binária completa com ponteiros pai/irmão
 * Uso: Representar mapa do jogo (15 nós, altura 3)
 */

// ESTRUTURA DE UM NÓ:
typedef struct No {
    char nome[32];           // Ex: "Nucleo-X", "A", "B"...
    TipoTerritorio tipo;     // NORMAL, RECURSO ou CHAVE
    int ocupado;             // 0=vazio, 1=jogador1, 2=jogador2
    int escudo;              // Proteção do nó (0 a N)
    struct No *pai;          // Ponteiro para nó pai
    struct No *esquerda;     // Filho esquerdo
    struct No *direita;      // Filho direito
    struct No *irmao;        // Nó irmão (esquerdo <-> direito)
} No;

// EXEMPLO DE USO:

#include "tree.h"

// Criar árvore completa
No *raiz = arvore_construir_completa();
// Cria 15 nós automaticamente com tipos aleatórios

// Buscar nós específicos
No *spawn1 = buscar_folha_esquerda(raiz);  // Spawn jogador 1
No *spawn2 = buscar_folha_direita(raiz);   // Spawn jogador 2
No *chave1 = buscar_no_chave(raiz, 1);     // Primeira CHAVE
No *chave2 = buscar_no_chave(raiz, 2);     // Segunda CHAVE

// Navegar pela árvore
No *atual = spawn1;
No *subir = atual->pai;           // Mover para pai
No *descer_esq = atual->esquerda; // Mover para filho esquerdo
No *descer_dir = atual->direita;  // Mover para filho direito
No *lateral = atual->irmao;       // Mover lateralmente

// Modificar propriedades
atual->ocupado = 1;      // Jogador 1 domina o nó
atual->escudo = 3;       // Colocar 3 escudos no nó

// Imprimir árvore (debug)
arvore_imprimir(raiz, 0);
// Saída:
// Nucleo-X [NORM ocp=0 esc=0]
//   A [NORM ocp=0 esc=0]
//     C [REC ocp=0 esc=0]
//       ...

// Liberar memória
arvore_liberar(raiz);


// =============================================================================
// JOGADOR (src/player.h)
// =============================================================================

/**
 * Estrutura: Representa um jogador no jogo
 */

// ESTRUTURA:
typedef struct Jogador {
    char nome[32];
    int id;                    // 1 ou 2
    No *atual;                 // Posição atual
    Lista *inventario;         // Recursos (espadas, escudos)
    Pilha *historico;          // Histórico de movimentos
    int chave1_dominada;       // 1 se dominou CHAVE 1
    int chave2_dominada;       // 1 se dominou CHAVE 2
} Jogador;

// EXEMPLO DE USO:

#include "player.h"

// Criar jogador
Jogador *j1 = jogador_criar("Jogador 1", 1, spawn1);

// Adicionar recursos ao inventário
lista_adicionar(j1->inventario, "espada");
lista_adicionar(j1->inventario, "escudo");

// Calcular estatísticas
int ataque = jogador_ataque(j1);              // conta espadas
int escudos = jogador_escudos_disponiveis(j1); // conta escudos

// Mover jogador
pilha_empilhar(j1->historico, j1->atual); // salvar posição
j1->atual = j1->atual->pai;                // mover para pai
j1->atual->ocupado = j1->id;               // dominar novo nó

// Dominar CHAVE
if (j1->atual == chave1) {
    j1->chave1_dominada = 1;
}

// Imprimir status
jogador_imprimir_status(j1);
// Saída:
// === Status: Jogador 1 (J1) ===
// Posicao atual: A
// Ataque (espadas): 1
// Escudos disponiveis: 1
// Chaves dominadas: 1/2
// Inventario:
//   - escudo
//   - espada
// ===========================

// Liberar memória
jogador_destruir(j1);


// =============================================================================
// FLUXO COMPLETO DO JOGO
// =============================================================================

int main() {
    // 1. Inicializar árvore
    No *raiz = arvore_construir_completa();

    // 2. Identificar nós especiais
    No *chave1 = buscar_no_chave(raiz, 1);
    No *chave2 = buscar_no_chave(raiz, 2);
    No *spawn1 = buscar_folha_esquerda(raiz);
    No *spawn2 = buscar_folha_direita(raiz);

    // 3. Criar jogadores
    Jogador *j1 = jogador_criar("Jogador 1", 1, spawn1);
    Jogador *j2 = jogador_criar("Jogador 2", 2, spawn2);

    // 4. Loop do jogo
    while (!vencedor) {
        // Turno do jogador
        // - Escolher ação (mover, colocar escudo, etc)
        // - Atualizar estado
        // - Verificar vitória
    }

    // 5. Limpeza
    jogador_destruir(j1);
    jogador_destruir(j2);
    arvore_liberar(raiz);

    return 0;
}

// =============================================================================
// CONCEITOS UTILIZADOS
// =============================================================================

/*
 * ✅ Alocação dinâmica de memória (malloc/free)
 * ✅ Ponteiros para estruturas (No*, Jogador*, Lista*, Pilha*)
 * ✅ TAD Lista Encadeada (inventário)
 * ✅ TAD Pilha (histórico de movimentos)
 * ✅ Árvore binária completa (15 nós)
 * ✅ Ponteiros múltiplos (pai, esquerda, direita, irmão)
 * ✅ Recursividade (busca de nós, configuração de ponteiros)
 * ✅ Modularização (separação .h e .c)
 * ✅ Documentação em português
 */
