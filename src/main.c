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
#else
#include <unistd.h>
#define LIMPAR_TELA "clear"
#endif

// Cores da interface
#define COR_RESET "\033[0m"
#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_AMARELO "\033[1;33m"
#define COR_AZUL "\033[1;34m"
#define COR_MAGENTA "\033[1;35m"
#define COR_CIANO "\033[1;36m"
#define COR_BRANCO "\033[1;37m"

#define LIMITE_TURNOS 100

No *raiz_global = NULL;
No *chave1_global = NULL;
No *chave2_global = NULL;

void limpar_tela() {
    system(LIMPAR_TELA);
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void pressionar_enter() {
    printf("\n%s[Pressione ENTER para continuar]%s\n", COR_CIANO, COR_RESET);
    getchar();
}

int tentar_movimento(No *destino, Jogador *jogador, Jogador *adversario) {
    if (destino == NULL) {
        printf("%sMOVIMENTO INVÁLIDO%s\n", COR_VERMELHO, COR_RESET);
        printf("%sO destino não existe.%s\n", COR_VERMELHO, COR_RESET);

        return 0;
    }

    if (destino == adversario->atual) {
        printf("%sMOVIMENTO BLOQUEADO%s\n", COR_VERMELHO, COR_RESET);
        printf("%sO J%d está neste território!%s\n", COR_VERMELHO, adversario->id, COR_RESET);

        return 0;
    }

    if (destino == raiz_global) {
        if (jogador->chave1_dominada == 0 || jogador->chave2_dominada == 0) {
            printf("%sMOVIMENTO INVÁLIDO%s\n", COR_VERMELHO, COR_RESET);
            printf("%sO Núcleo-X só pode ser acessado após dominar AMBAS as chaves!%s\n\n", COR_VERMELHO, COR_RESET);
            printf("%sChaves dominadas: %d/2%s\n", COR_AMARELO, jogador->chave1_dominada + jogador->chave2_dominada, COR_RESET);

            return 0;
        }
    }

    if (destino->ocupado != 0 && destino->ocupado != jogador->id) {
        printf("%sCOMBATE%s\n", COR_AZUL, COR_RESET);
        printf("O território '%s' está ocupado por J%d!\n", destino->nome, destino->ocupado);
        printf("%sEscudo: %d%s\n", COR_VERDE, destino->escudo, COR_RESET);
        printf("%sAtaque: %d%s\n", COR_VERDE, jogador_ataque(jogador), COR_RESET);

        if (destino->escudo == 0) {
            printf("%sCONQUISTADO%s\n", COR_VERDE, COR_RESET);
            printf("%sO território não possuia escudo!%s\n", COR_MAGENTA, COR_RESET);

            if (destino->tipo == CHAVE) {
                if (destino == chave1_global) {
                    adversario->chave1_dominada = 0;
                    printf("%sJ%d perdeu a CHAVE 1!%s\n", COR_VERMELHO, adversario->id, COR_RESET);
                }

                else if (destino == chave2_global) {
                    adversario->chave2_dominada = 0;
                    printf("%sJ%d perdeu a CHAVE 2!%s\n", COR_VERMELHO, adversario->id, COR_RESET);
                }
            }

            destino->ocupado = jogador->id;
        }

        else if (jogador_ataque(jogador) >= destino->escudo) {
            printf("%sCONQUISTADO%s\n", COR_VERDE, COR_RESET);
            printf("%sVocê superou as defesas do território!%s\n", COR_VERDE, COR_RESET);

            if (destino->tipo == CHAVE) {
                if (destino == chave1_global) {
                    adversario->chave1_dominada = 0;
                    printf("%sJ%d perdeu a CHAVE 1!%s\n", COR_VERMELHO, adversario->id, COR_RESET);
                }

                else if (destino == chave2_global) {
                    adversario->chave2_dominada = 0;
                    printf("%sJ%d perdeu a CHAVE 2!%s\n", COR_VERMELHO, adversario->id, COR_RESET);
                }
            }

            destino->ocupado = jogador->id;
            destino->escudo -= 1;
        }

        else {
            printf("%sNÃO CONQUISTADO%s\n", COR_VERMELHO, COR_RESET);
            printf("%sAs defesas do território são muito fortes!%s\n", COR_VERMELHO, COR_RESET);

            return 0;
        }
    }

    printf("%sJOGADOR %d se movimentou de '%s' para '%s'.\n", COR_CIANO, jogador->id, jogador->atual->nome, destino->nome, COR_RESET);

    pilha_empilhar(jogador->historico, jogador->atual);
    jogador->atual = destino;
    destino->ocupado = jogador->id;

    if (destino->tipo == CHAVE && destino->ocupado == jogador->id) {
        if (destino == chave1_global && !jogador->chave1_dominada) {
            jogador->chave1_dominada = 1;
            printf("%sCHAVE 1 DOMINADA%s\n", COR_AMARELO, COR_RESET);
            printf("Se o território for dominado, a chave será perdida!\n");
        }

        else if (destino == chave2_global && !jogador->chave2_dominada) {
            jogador->chave2_dominada = 1;
            printf("%sCHAVE 2 DOMINADA%s\n", COR_AMARELO, COR_RESET);
            printf("Se o território for dominado, a chave será perdida!\n");
        }
    }

    if (destino->tipo == RECURSO) {
        int nivel = calcular_nivel(raiz_global, destino);

        // Recursos melhores e em maior quantidade em níveis altos (próximo da raiz)
        printf("%sRECURSO%s\n", COR_AZUL, COR_RESET);

        if (nivel <= 1) {
            // Níveis 0-1: 2-3 recursos
            int quantidade = 2 + rand() % 2;
            printf("Coletando %d item(ns):\n\n", quantidade);
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"espada", "espada", "espada", "escudo", "escudo"};
                const char *rec = recursos[rand() % 5];
                printf("+ %s\n", rec);
                lista_adicionar(jogador->inventario, rec);
            }
        } else if (nivel == 2) {
            // Nível 2: 1-2 recursos
            int quantidade = 1 + rand() % 2;
            printf("Coletando %d item(ns):\n\n", quantidade);
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"espada", "escudo", "escudo"};
                const char *rec = recursos[rand() % 3];
                printf("+ %s\n", rec);
                lista_adicionar(jogador->inventario, rec);
            }
        } else {
            // Nível 3: 1 recurso
            const char *recursos[] = {"espada", "escudo"};
            const char *rec = recursos[rand() % 2];
            printf("Coletando:\n\n+%s\n", rec);
            lista_adicionar(jogador->inventario, rec);
        }

        destino->tipo = NORMAL;
        printf("O território agora é NORMAL.\n");
    }

    return 1;
}

int contar_territorios(No *raiz, int id_jogador) {
    if (raiz == NULL)
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

        printf("%s========================================%s\n", COR_AZUL, COR_RESET);
        printf("%sTurno: %s (J%d)%s\n", COR_BRANCO, jogador->nome, jogador->id, COR_RESET);
        printf("%sTerritório: %s%s [%s | %d ESCUDO(S)]\n",
               COR_CIANO,
               jogador->atual->nome,
               COR_RESET,
               jogador->atual->tipo == NORMAL    ? "NORMAL"
               : jogador->atual->tipo == RECURSO ? "RECURSO"
                                                 : "CHAVE",
               jogador->atual->escudo);
        printf("%sAtaque: %d%s | %sEscudos: %d%s | %sChaves: %d/2%s\n",
               COR_VERMELHO,
               jogador_ataque(jogador),
               COR_RESET,
               COR_VERDE,
               jogador_escudos_disponiveis(jogador),
               COR_RESET,
               COR_AMARELO,
               jogador->chave1_dominada + jogador->chave2_dominada,
               COR_RESET);
        printf("%s========================================%s\n\n", COR_AZUL, COR_RESET);

        printf("Movimentação:\n\n");
        printf("1 - CIMA (%s)\n", jogador->atual->pai);
        if (jogador->atual->esquerda)
            printf("2 - BAIXO ESQUERDA (%s)\n", jogador->atual->esquerda->nome);
        if (jogador->atual->direita)
            printf("3 - BAIXO DIREITA (%s)\n", jogador->atual->direita->nome);

        No *lateral_esq = buscar_vizinho_esquerda(raiz, jogador->atual);
        No *lateral_dir = buscar_vizinho_direita(raiz, jogador->atual);

        if (lateral_esq)
            printf("4 - LATERAL ESQUERDA (%s)\n", lateral_esq->nome);
        if (lateral_dir)
            printf("5 - LATERAL DIREITA (%s)\n", lateral_dir->nome);

        printf("\nAções:\n\n");

        printf("6 - Colocar ESCUDO\n");
        printf("7 - Ver MAPA\n");
        printf("8 - STATUS adversário\n");
        printf("9 - PASSAR\n\n");

        printf("> ");

        int escolha = 0;
        if (scanf("%d", &escolha) != 1) {
            limpar_buffer();
            printf("OPÇÃO INVÁLIDA.\n");
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
            }

            pressionar_enter();
            break;

        case 2: // Mover para filho esquerdo
            if (jogador->atual->esquerda) {
                if (tentar_movimento(jogador->atual->esquerda, jogador, adversario)) {
                    turno_concluido = 1;
                }
            }

            pressionar_enter();
            break;

        case 3: // Mover para filho direito
            if (jogador->atual->direita) {
                if (tentar_movimento(jogador->atual->direita, jogador, adversario)) {
                    turno_concluido = 1;
                }
            }

            pressionar_enter();
            break;

        case 4: // Movimento lateral para esquerda
            No *lateral_esq = buscar_vizinho_esquerda(raiz, jogador->atual);
            if (lateral_esq) {
                if (tentar_movimento(lateral_esq, jogador, adversario)) {
                    turno_concluido = 1;
                }
            }

            pressionar_enter();
            break;

        case 5: // Movimento lateral para direita
            No *lateral_dir = buscar_vizinho_direita(raiz, jogador->atual);
            if (lateral_dir) {
                if (tentar_movimento(lateral_dir, jogador, adversario)) {
                    turno_concluido = 1;
                }
            }

            pressionar_enter();
            break;

        case 6: // Colocar escudo
            if (jogador_escudos_disponiveis(jogador) > 0) {
                lista_remover(jogador->inventario, "escudo");
                jogador->atual->escudo++;
                printf("Escudo colocado! Escudos (%s): %d\n", jogador->atual->nome, jogador->atual->escudo);
                turno_concluido = 1; // Colocar escudo passa o turno
            }

            else {
                printf("Escudos insuficientes!\n");
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
            printf("Território: %s\n", adversario->atual->nome);
            printf("Chaves: %d/2\n", adversario->chave1_dominada + adversario->chave2_dominada);
            printf("Ataque: %d\n", jogador_ataque(adversario));
            printf("Escudos: %d\n", jogador_escudos_disponiveis(adversario));

            pressionar_enter();
            break;

        case 9: // Passar turno explicitamente
            printf("%s passou.\n", jogador->nome);
            turno_concluido = 1;

            pressionar_enter();
            break;

        default:
            printf("OPÇÃO INVÁLIDA.\n");

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
    limpar_tela();

    printf("%s====================================%s\n", COR_MAGENTA, COR_RESET);
    printf("%s       CODE BATTLE - v2.1%s\n", COR_MAGENTA, COR_RESET);
    printf("%s====================================%s\n", COR_MAGENTA, COR_RESET);
    printf("Objetivo: Dominar ambas as CHAVES e conquistar o Núcleo-X!\n\n");

    No *raiz = arvore_construir_completa();
    raiz_global = raiz; // Salvar raiz global

    // Identificar nós CHAVE
    chave1_global = buscar_no_chave(raiz, 1);
    chave2_global = buscar_no_chave(raiz, 2);

    printf("Territórios CHAVE:\n");
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
            printf("%s%s VENCEU O JOGO!%s\n", COR_VERDE, atual->nome, COR_RESET);
            printf("%s****************************************%s\n", COR_VERDE, COR_RESET);
            printf("Condicoes atendidas:\n");
            printf("  %s- Ambas CHAVES dominadas%s\n", COR_AMARELO, COR_RESET);
            printf("  %s- Núcleo-X conquistado%s\n", COR_AMARELO, COR_RESET);
            vencedor = atual->id;
            break;
        }

        // Verificar limite de turnos
        if (turno >= LIMITE_TURNOS) {
            limpar_tela();
            printf("\n\n");
            printf("%s****************************************%s\n", COR_AMARELO, COR_RESET);
            printf("%sLIMITE DE TURNOS ATINGIDO!%s\n", COR_AMARELO, COR_RESET);
            printf("%s****************************************%s\n", COR_AMARELO, COR_RESET);

            int terr_j1 = contar_territorios(raiz, 1);
            int terr_j2 = contar_territorios(raiz, 2);

            printf("\n%sContagem de territórios:%s\n", COR_BRANCO, COR_RESET);
            printf("  %s: %d territórios\n", j1->nome, terr_j1);
            printf("  %s: %d territórios\n\n", j2->nome, terr_j2);

            if (terr_j1 > terr_j2) {
                printf("%s%s VENCE POR TERRITÓRIOS!%s\n", COR_VERDE, j1->nome, COR_RESET);
                vencedor = 1;
            } else if (terr_j2 > terr_j1) {
                printf("%s%s VENCE POR TERRITÓRIOS!%s\n", COR_VERDE, j2->nome, COR_RESET);
                vencedor = 2;
            } else {
                printf("%sEMPATE! Ambos dominam %d territórios%s\n", COR_AMARELO, terr_j1, COR_RESET);
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
