#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "player.h"
#include "tads/list.h"
#include "tads/stack.h"
#include "tree.h"

// Detecção de plataforma
#ifdef _WIN32
#include <windows.h>
#define LIMPAR_TELA "cls"
#define PLATAFORMA_WINDOWS 1
#else
#include <unistd.h>
#define LIMPAR_TELA "clear"
#define PLATAFORMA_WINDOWS 0
#endif

// Códigos de cor ANSI (funciona em Windows 10+ e Linux)
#define COR_RESET "\033[0m"
#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_AMARELO "\033[1;33m"
#define COR_AZUL "\033[1;34m"
#define COR_MAGENTA "\033[1;35m"
#define COR_CIANO "\033[1;36m"
#define COR_BRANCO "\033[1;37m"

// Variáveis globais para nós CHAVE e raiz
No *chave1_global = NULL;
No *chave2_global = NULL;
No *raiz_global = NULL;

// Limite de turnos
#define LIMITE_TURNOS 100

#define LIMITE_TURNOS 100

/**
 * Habilita cores ANSI no terminal Windows (Windows 10+).
 */
void habilitar_cores_ansi() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
#endif
}

/**
 * Limpa a tela do terminal.
 */
void limpar_tela() {
    system(LIMPAR_TELA);
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void pressionar_enter() {
    printf("\n%s[Pressione Enter para continuar...]%s\n", COR_CIANO, COR_RESET);
    getchar();
}

/**
 * Tenta mover jogador para nó adjacente (pai, esquerda, direita, irmão).
 * Se nó estiver ocupado por adversário, inicia combate.
 */
int tentar_movimento(No *destino, Jogador *jogador, Jogador *adversario) {
    if (!destino) {
        printf("Movimento invalido: destino nao existe.\n");
        return 0;
    }

    // Bloquear movimento para nó onde o adversário está posicionado
    if (destino == adversario->atual) {
        printf("%sMovimento bloqueado: o adversario esta neste no!%s\n", COR_VERMELHO, COR_RESET);
        return 0;
    }

    // Bloquear acesso à raiz até ter ambas as chaves
    if (destino == raiz_global) {
        if (!jogador->chave1_dominada || !jogador->chave2_dominada) {
            printf("\n*** ACESSO BLOQUEADO! ***\n");
            printf("O Nucleo-X so pode ser acessado apos dominar AMBAS as chaves!\n");
            printf("Chaves dominadas: %d/2\n", jogador->chave1_dominada + jogador->chave2_dominada);
            return 0;
        }
    }

    // Verifica se está ocupado por adversário
    if (destino->ocupado != 0 && destino->ocupado != jogador->id) {
        printf("\n%s*** COMBATE ***%s\n", COR_VERMELHO, COR_RESET);
        printf("No '%s' esta ocupado pelo adversario!\n", destino->nome);
        printf("%sEscudo do no: %d%s\n", COR_AMARELO, destino->escudo, COR_RESET);
        printf("%sSeu ataque: %d%s\n", COR_VERDE, jogador_ataque(jogador), COR_RESET);

        if (destino->escudo == 0) {
            printf("%sNo sem escudo! Conquistado automaticamente.%s\n", COR_VERDE, COR_RESET);

            // Se era nó CHAVE, transferir dominância da chave
            if (destino->tipo == CHAVE) {
                if (destino == chave1_global) {
                    adversario->chave1_dominada = 0;
                    printf("%sAdversario perdeu a CHAVE 1!%s\n", COR_VERMELHO, COR_RESET);
                } else if (destino == chave2_global) {
                    adversario->chave2_dominada = 0;
                    printf("%sAdversario perdeu a CHAVE 2!%s\n", COR_VERMELHO, COR_RESET);
                }
            }

            destino->ocupado = jogador->id;
        } else if (jogador_ataque(jogador) >= destino->escudo) {
            printf("%sAtaque suficiente! No conquistado.%s\n", COR_VERDE, COR_RESET);

            // Se era nó CHAVE, transferir dominância da chave
            if (destino->tipo == CHAVE) {
                if (destino == chave1_global) {
                    adversario->chave1_dominada = 0;
                    printf("%sAdversario perdeu a CHAVE 1!%s\n", COR_VERMELHO, COR_RESET);
                } else if (destino == chave2_global) {
                    adversario->chave2_dominada = 0;
                    printf("%sAdversario perdeu a CHAVE 2!%s\n", COR_VERMELHO, COR_RESET);
                }
            }

            destino->ocupado = jogador->id;
            destino->escudo = 0; // zera escudo após conquista
        } else {
            printf("%sAtaque insuficiente. Movimento bloqueado.%s\n", COR_VERMELHO, COR_RESET);
            return 0;
        }
    }

    // Movimento válido
    pilha_empilhar(jogador->historico, jogador->atual);
    jogador->atual = destino;

    // OCUPAÇÃO SEMPRE MUDA ao passar pelo território, independente de escudos
    // Só não muda se o jogador já ocupa o nó
    destino->ocupado = jogador->id;

    printf("%sMovimento realizado para: %s%s\n", COR_CIANO, destino->nome, COR_RESET);

    // Verifica se é nó CHAVE e marca como dominada (baseado em ocupação do nó)
    if (destino->tipo == CHAVE && destino->ocupado == jogador->id) {
        if (destino == chave1_global && !jogador->chave1_dominada) {
            jogador->chave1_dominada = 1;
            printf("\n%s*** CHAVE 1 DOMINADA! ***%s\n", COR_AMARELO, COR_RESET);
            printf("A chave permanece no no %s. Para perder a chave, o adversario deve conquistar este no!\n", destino->nome);
        } else if (destino == chave2_global && !jogador->chave2_dominada) {
            jogador->chave2_dominada = 1;
            printf("\n%s*** CHAVE 2 DOMINADA! ***%s\n", COR_AMARELO, COR_RESET);
            printf("A chave permanece no no %s. Para perder a chave, o adversario deve conquistar este no!\n", destino->nome);
        }
    }

    // Coleta recurso se for nó RECURSO (e converte nó para NORMAL)
    if (destino->tipo == RECURSO) {
        // Calcular nível do nó (quanto menor o nível, mais próximo da raiz)
        int nivel = calcular_nivel(raiz_global, destino);

        // Recursos melhores e em maior quantidade em níveis altos (próximo da raiz)
        printf("\n%s*** NO DE RECURSO! ***%s\n", COR_VERDE, COR_RESET);
        printf("Nivel do no: %d (0=raiz, 3=folhas)\n", nivel);

        if (nivel <= 1) {
            // Níveis 0-1 (próximo à raiz): 2-3 recursos, prioridade espada/escudo
            int quantidade = 2 + rand() % 2;
            printf("Recursos proximos a raiz! Coletando %d itens:\n", quantidade);
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"espada", "espada", "escudo", "escudo", "energia"};
                const char *rec = recursos[rand() % 5];
                printf("  + %s\n", rec);
                lista_adicionar(jogador->inventario, rec);
            }
        } else if (nivel == 2) {
            // Nível 2: 1-2 recursos
            int quantidade = 1 + rand() % 2;
            printf("Coletando %d item(ns):\n", quantidade);
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"espada", "escudo", "energia"};
                const char *rec = recursos[rand() % 3];
                printf("  + %s\n", rec);
                lista_adicionar(jogador->inventario, rec);
            }
        } else {
            // Nível 3 (folhas): 1 recurso
            const char *recursos[] = {"espada", "escudo", "energia"};
            const char *rec = recursos[rand() % 3];
            printf("Coletando: %s\n", rec);
            lista_adicionar(jogador->inventario, rec);
        }

        // Converter nó RECURSO em NORMAL (recurso consumido)
        destino->tipo = NORMAL;
        printf("No agora e NORMAL (recurso consumido).\n");
    }

    return 1;
}

/**
 * Conta quantos territórios um jogador domina.
 */
int contar_territorios(No *raiz, int id_jogador) {
    if (!raiz)
        return 0;

    int contador = (raiz->ocupado == id_jogador) ? 1 : 0;
    contador += contar_territorios(raiz->esquerda, id_jogador);
    contador += contar_territorios(raiz->direita, id_jogador);

    return contador;
}

/**
 * Menu de ações do jogador.
 * Loop continua até que uma ação que passa o turno seja executada.
 */
void turno_jogador(No *raiz, Jogador *jogador, Jogador *adversario) {
    int turno_concluido = 0;

    while (!turno_concluido) {
        limpar_tela();

        printf("\n%s========================================%s\n", COR_AZUL, COR_RESET);
        printf("%sTurno: %s (J%d)%s\n", COR_BRANCO, jogador->nome, jogador->id, COR_RESET);
        printf("%sPosicao: %s%s [tipo=%s, escudo=%d]\n",
               COR_CIANO,
               jogador->atual->nome,
               COR_RESET,
               jogador->atual->tipo == NORMAL    ? "NORMAL"
               : jogador->atual->tipo == RECURSO ? "RECURSO"
                                                 : "CHAVE",
               jogador->atual->escudo);
        printf("%sAtaque: %d%s | %sEscudos disp.: %d%s | %sChaves: %d/2%s\n",
               COR_VERMELHO,
               jogador_ataque(jogador),
               COR_RESET,
               COR_VERDE,
               jogador_escudos_disponiveis(jogador),
               COR_RESET,
               COR_AMARELO,
               jogador->chave1_dominada + jogador->chave2_dominada,
               COR_RESET);
        printf("%s========================================%s\n", COR_AZUL, COR_RESET);

        // Mostrar opções de movimento disponíveis
        printf("\nAcoes disponiveis:\n");
        printf("  1 - Subir (mover para PAI)\n");
        if (jogador->atual->esquerda)
            printf("  2 - Descer para FILHO ESQUERDO (%s)\n", jogador->atual->esquerda->nome);
        if (jogador->atual->direita)
            printf("  3 - Descer para FILHO DIREITO (%s)\n", jogador->atual->direita->nome);

        // Movimento lateral verdadeiro (nós adjacentes no mesmo nível)
        No *lateral_esq = buscar_vizinho_esquerda(raiz, jogador->atual);
        No *lateral_dir = buscar_vizinho_direita(raiz, jogador->atual);

        if (lateral_esq)
            printf("  4 - Movimento LATERAL para ESQUERDA (%s)\n", lateral_esq->nome);
        if (lateral_dir)
            printf("  5 - Movimento LATERAL para DIREITA (%s)\n", lateral_dir->nome);

        printf("  6 - Colocar ESCUDO no no atual\n");
        printf("  7 - Ver mapa\n");
        printf("  8 - Ver status do adversario\n");
        printf("  9 - Passar turno\n");
        printf("Escolha: ");

        int escolha = 0;
        if (scanf("%d", &escolha) != 1) {
            limpar_buffer();
            printf("Entrada invalida.\n");
            pressionar_enter();
            continue;
        }
        limpar_buffer();

        switch (escolha) {
        case 1: // Mover para pai
            if (jogador->atual->pai) {
                if (tentar_movimento(jogador->atual->pai, jogador, adversario)) {
                    turno_concluido = 1; // Movimento passa o turno
                }
            } else {
                printf("Nao ha no pai (voce ja esta na raiz).\n");
            }
            pressionar_enter();
            break;

        case 2: // Mover para filho esquerdo
            if (jogador->atual->esquerda) {
                if (tentar_movimento(jogador->atual->esquerda, jogador, adversario)) {
                    turno_concluido = 1;
                }
            } else {
                printf("Nao ha filho esquerdo.\n");
            }
            pressionar_enter();
            break;

        case 3: // Mover para filho direito
            if (jogador->atual->direita) {
                if (tentar_movimento(jogador->atual->direita, jogador, adversario)) {
                    turno_concluido = 1;
                }
            } else {
                printf("Nao ha filho direito.\n");
            }
            pressionar_enter();
            break;

        case 4: // Movimento lateral para esquerda
        {
            No *lateral_esq = buscar_vizinho_esquerda(raiz, jogador->atual);
            if (lateral_esq) {
                if (tentar_movimento(lateral_esq, jogador, adversario)) {
                    turno_concluido = 1;
                }
            } else {
                printf("Nao ha no adjacente a esquerda.\n");
            }
            pressionar_enter();
        } break;

        case 5: // Movimento lateral para direita
        {
            No *lateral_dir = buscar_vizinho_direita(raiz, jogador->atual);
            if (lateral_dir) {
                if (tentar_movimento(lateral_dir, jogador, adversario)) {
                    turno_concluido = 1;
                }
            } else {
                printf("Nao ha no adjacente a direita.\n");
            }
            pressionar_enter();
        } break;

        case 6: // Colocar escudo
            if (jogador_escudos_disponiveis(jogador) > 0) {
                lista_remover(jogador->inventario, "escudo");
                jogador->atual->escudo++;
                printf("Escudo colocado! Total no no: %d\n", jogador->atual->escudo);
                turno_concluido = 1; // Colocar escudo passa o turno
            } else {
                printf("Voce nao possui escudos no inventario.\n");
            }
            pressionar_enter();
            break;

        case 7: // Ver mapa (NÃO passa o turno)
            printf("\n=== MAPA DA ARVORE ===\n");
            arvore_imprimir(raiz, 0);
            pressionar_enter();
            break;

        case 8: // Ver status do adversário (NÃO passa o turno)
            printf("\n=== STATUS DO ADVERSARIO ===\n");
            printf("Nome: %s (J%d)\n", adversario->nome, adversario->id);
            printf("Posicao: %s\n", adversario->atual->nome);
            printf("Chaves dominadas: %d/2\n", adversario->chave1_dominada + adversario->chave2_dominada);
            printf("Ataque: %d\n", jogador_ataque(adversario));
            printf("Escudos disponiveis: %d\n", jogador_escudos_disponiveis(adversario));
            pressionar_enter();
            break;

        case 9: // Passar turno explicitamente
            printf("%s passou o turno.\n", jogador->nome);
            turno_concluido = 1;
            pressionar_enter();
            break;

        default:
            printf("Opcao invalida.\n");
            pressionar_enter();
            break;
        }
    }
}

/**
 * Verifica condição de vitória:
 * - Dominar ambas as CHAVES
 * - Estar no Nucleo-X
 */
int verificar_vitoria(Jogador *jogador, No *raiz) {
    if (jogador->chave1_dominada && jogador->chave2_dominada && jogador->atual == raiz) {
        return 1;
    }
    return 0;
}

int main() {
    srand((unsigned)time(NULL));
    habilitar_cores_ansi();
    limpar_tela();

    printf("%s====================================%s\n", COR_MAGENTA, COR_RESET);
    printf("%s       CODE BATTLE - v2.1%s\n", COR_MAGENTA, COR_RESET);
    printf("%s====================================%s\n", COR_MAGENTA, COR_RESET);
    printf("Objetivo: Dominar ambas as CHAVES e conquistar o Nucleo-X!\n\n");

    No *raiz = arvore_construir_completa();
    raiz_global = raiz; // Salvar raiz global

    // Identificar nós CHAVE
    chave1_global = buscar_no_chave(raiz, 1);
    chave2_global = buscar_no_chave(raiz, 2);

    printf("Nos CHAVE localizados:\n");
    printf("  CHAVE 1: %s\n", chave1_global ? chave1_global->nome : "N/A");
    printf("  CHAVE 2: %s\n", chave2_global ? chave2_global->nome : "N/A");

    // Posicionar jogadores
    No *spawn1 = buscar_folha_esquerda(raiz);
    No *spawn2 = buscar_folha_direita(raiz);

    Jogador *j1 = jogador_criar("Jogador 1", 1, spawn1);
    Jogador *j2 = jogador_criar("Jogador 2", 2, spawn2);

    printf("\nSpawn Jogador 1: %s\n", spawn1->nome);
    printf("Spawn Jogador 2: %s\n", spawn2->nome);

    printf("\n=== MAPA INICIAL ===\n");
    arvore_imprimir(raiz, 0);
    pressionar_enter();

    // Loop do jogo
    int turno = 1;
    int vencedor = 0;

    while (!vencedor) {
        Jogador *atual = (turno % 2 == 1) ? j1 : j2;
        Jogador *oponente = (turno % 2 == 1) ? j2 : j1;

        turno_jogador(raiz, atual, oponente);

        if (verificar_vitoria(atual, raiz)) {
            limpar_tela();
            printf("\n\n");
            printf("%s****************************************%s\n", COR_VERDE, COR_RESET);
            printf("%s***  %s VENCEU O JOGO!  ***%s\n", COR_VERDE, atual->nome, COR_RESET);
            printf("%s****************************************%s\n", COR_VERDE, COR_RESET);
            printf("Condicoes atendidas:\n");
            printf("  %s- Ambas CHAVES dominadas%s\n", COR_AMARELO, COR_RESET);
            printf("  %s- Nucleo-X conquistado%s\n", COR_AMARELO, COR_RESET);
            vencedor = atual->id;
            break;
        }

        // Verificar limite de turnos
        if (turno >= LIMITE_TURNOS) {
            limpar_tela();
            printf("\n\n");
            printf("%s****************************************%s\n", COR_AMARELO, COR_RESET);
            printf("%s***  LIMITE DE TURNOS ATINGIDO!  ***%s\n", COR_AMARELO, COR_RESET);
            printf("%s****************************************%s\n", COR_AMARELO, COR_RESET);

            int terr_j1 = contar_territorios(raiz, 1);
            int terr_j2 = contar_territorios(raiz, 2);

            printf("\n%sContagem de territorios:%s\n", COR_BRANCO, COR_RESET);
            printf("  %s: %d territorios\n", j1->nome, terr_j1);
            printf("  %s: %d territorios\n\n", j2->nome, terr_j2);

            if (terr_j1 > terr_j2) {
                printf("%s*** %s VENCE POR TERRITORIOS! ***%s\n", COR_VERDE, j1->nome, COR_RESET);
                vencedor = 1;
            } else if (terr_j2 > terr_j1) {
                printf("%s*** %s VENCE POR TERRITORIOS! ***%s\n", COR_VERDE, j2->nome, COR_RESET);
                vencedor = 2;
            } else {
                printf("%s*** EMPATE! Ambos dominam %d territorios ***%s\n", COR_AMARELO, terr_j1, COR_RESET);
            }
            break;
        }

        turno++;
    }

    // Limpeza
    jogador_destruir(j1);
    jogador_destruir(j2);
    arvore_liberar(raiz);

    printf("\nObrigado por jogar Code Battle!\n");
    return 0;
}
