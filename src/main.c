#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "componentes/historico/historico.h"
#include "componentes/inventario/inventario.h"
#include "componentes/jogador/jogador.h"
#include "componentes/mundo/mundo.h"
#include "utils/terminal/terminal.h"

#define LIMITE_TURNOS 100

Territorio *raiz_global = NULL;
Territorio *chave1_global = NULL;
Territorio *chave2_global = NULL;

int tentarMovimento(Territorio *destino, Jogador *jogador, Jogador *adversario) {
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
        printf("%sAtaque: %d%s\n", COR_VERDE, ataque(jogador), COR_RESET);

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

        else if (ataque(jogador) >= destino->escudo) {
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

    printf("%sJOGADOR %d se movimentou de '%s' para '%s'.%s\n", COR_CIANO, jogador->id, jogador->atual->nome, destino->nome, COR_RESET);

    empilharHistorico(jogador->historico, jogador->atual);
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
        int nivel = nivelTerritorio(destino, raiz_global);

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
                adicionarItem(jogador->inventario, rec);
            }
        } else if (nivel == 2) {
            // Nível 2: 1-2 recursos
            int quantidade = 1 + rand() % 2;
            printf("Coletando %d item(ns):\n\n", quantidade);
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"espada", "escudo", "escudo"};
                const char *rec = recursos[rand() % 3];
                printf("+ %s\n", rec);
                adicionarItem(jogador->inventario, rec);
            }
        } else {
            // Nível 3: 1 recurso
            const char *recursos[] = {"espada", "escudo"};
            const char *rec = recursos[rand() % 2];
            printf("Coletando:\n\n+%s\n", rec);
            adicionarItem(jogador->inventario, rec);
        }

        destino->tipo = NORMAL;
        printf("O território agora é NORMAL.\n");
    }

    return 1;
}

int contarTerritorios(Territorio *raiz, int id_jogador) {
    if (raiz == NULL)
        return 0;

    int contador = (raiz->ocupado == id_jogador) ? 1 : 0;
    contador += contarTerritorios(raiz->esquerda, id_jogador);
    contador += contarTerritorios(raiz->direita, id_jogador);

    return contador;
}

/**
 * Menu de ações do jogador.
 * Loop continua até que uma ação que passa o turno seja executada.
 */
void turnoJogador(Territorio *raiz, Jogador *jogador, Jogador *adversario) {
    int turno_concluido = 0;

    while (!turno_concluido) {
        limparTela();

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
               ataque(jogador),
               COR_RESET,
               COR_VERDE,
               escudos(jogador),
               COR_RESET,
               COR_AMARELO,
               jogador->chave1_dominada + jogador->chave2_dominada,
               COR_RESET);
        printf("%s========================================%s\n\n", COR_AZUL, COR_RESET);

        printf("Movimentação:\n\n");
        printf("1 - CIMA (%s)\n", jogador->atual->pai ? jogador->atual->pai->nome : "N/A");
        if (jogador->atual->esquerda)
            printf("2 - BAIXO ESQUERDA (%s)\n", jogador->atual->esquerda->nome);
        if (jogador->atual->direita)
            printf("3 - BAIXO DIREITA (%s)\n", jogador->atual->direita->nome);

        Territorio *lateral_esq = buscarIrmaoEsquerda(raiz, jogador->atual);
        Territorio *lateral_dir = buscarIrmaoDireita(raiz, jogador->atual);

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
            limparBuffer();
            printf("OPÇÃO INVÁLIDA.\n");
            prompt();
            continue;
        }
        limparBuffer();

        switch (escolha) {
        case 1: // Mover para pai
            if (jogador->atual->pai) {
                if (tentarMovimento(jogador->atual->pai, jogador, adversario)) {
                    turno_concluido = 1; // Movimento passa o turno
                }
            }

            prompt();
            break;

        case 2: // Mover para filho esquerdo
            if (jogador->atual->esquerda) {
                if (tentarMovimento(jogador->atual->esquerda, jogador, adversario)) {
                    turno_concluido = 1;
                }
            }

            prompt();
            break;

        case 3: // Mover para filho direito
            if (jogador->atual->direita) {
                if (tentarMovimento(jogador->atual->direita, jogador, adversario)) {
                    turno_concluido = 1;
                }
            }

            prompt();
            break;

        case 4: // Movimento lateral para esquerda
            Territorio *lateral_esq = buscarIrmaoEsquerda(raiz, jogador->atual);
            if (lateral_esq) {
                if (tentarMovimento(lateral_esq, jogador, adversario)) {
                    turno_concluido = 1;
                }
            }

            prompt();
            break;

        case 5: // Movimento lateral para direita
            Territorio *lateral_dir = buscarIrmaoDireita(raiz, jogador->atual);
            if (lateral_dir) {
                if (tentarMovimento(lateral_dir, jogador, adversario)) {
                    turno_concluido = 1;
                }
            }

            prompt();
            break;

        case 6: // Colocar escudo
            if (escudos(jogador) > 0) {
                removerItem(jogador->inventario, "escudo");
                jogador->atual->escudo++;
                printf("Escudo colocado! Escudos (%s): %d\n", jogador->atual->nome, jogador->atual->escudo);
                turno_concluido = 1; // Colocar escudo passa o turno
            }

            else {
                printf("Escudos insuficientes!\n");
            }

            prompt();
            break;

        case 7: // Ver mapa (NÃO passa o turno)
            printf("\n=== MAPA DA ARVORE ===\n");
            imprimirMundo(raiz, 0);
            prompt();
            break;

        case 8: // Ver status do adversário (NÃO passa o turno)
            printf("\n=== STATUS DO ADVERSARIO ===\n");
            printf("Nome: %s (J%d)\n", adversario->nome, adversario->id);
            printf("Território: %s\n", adversario->atual->nome);
            printf("Chaves: %d/2\n", adversario->chave1_dominada + adversario->chave2_dominada);
            printf("Ataque: %d\n", ataque(adversario));
            printf("Escudos: %d\n", escudos(adversario));

            prompt();
            break;

        case 9: // Passar turno explicitamente
            printf("%s passou.\n", jogador->nome);
            turno_concluido = 1;

            prompt();
            break;

        default:
            printf("OPÇÃO INVÁLIDA.\n");

            prompt();
            break;
        }
    }
}

int verificarVitoria(Jogador *jogador, Territorio *raiz) {
    if (jogador->chave1_dominada && jogador->chave2_dominada && jogador->atual == raiz) {
        return 1;
    }

    return 0;
}

int main() {
    srand((unsigned)time(NULL));
    limparTela();

    printf("%sCODE BATTLE%s\n\n", COR_MAGENTA, COR_RESET);
    printf("Objetivo: Dominar ambas as CHAVES e conquistar o Núcleo-X!\n\n");

    Territorio *raiz = criarMundo();
    raiz_global = raiz; // Salvar raiz global

    // Identificar nós CHAVE
    chave1_global = buscarChave(raiz, 1);
    chave2_global = buscarChave(raiz, 2);

    printf("Territórios CHAVE:\n");
    printf("  CHAVE 1: %s\n", chave1_global ? chave1_global->nome : "N/A");
    printf("  CHAVE 2: %s\n", chave2_global ? chave2_global->nome : "N/A");

    // Posicionar jogadores
    Territorio *spawn1 = buscarBaseEsquerda(raiz);
    Territorio *spawn2 = buscarBaseDireita(raiz);

    Jogador *j1 = criarJogador("Jogador 1", 1, spawn1);
    Jogador *j2 = criarJogador("Jogador 2", 2, spawn2);

    printf("\nSpawn Jogador 1: %s\n", spawn1->nome);
    printf("Spawn Jogador 2: %s\n", spawn2->nome);

    printf("\n=== MAPA INICIAL ===\n");
    imprimirMundo(raiz, 0);
    prompt();

    // Loop do jogo
    int turno = 1;
    int vencedor = 0;

    while (!vencedor) {
        Jogador *atual = (turno % 2 == 1) ? j1 : j2;
        Jogador *oponente = (turno % 2 == 1) ? j2 : j1;

        turnoJogador(raiz, atual, oponente);

        if (verificarVitoria(atual, raiz)) {
            limparTela();
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
            limparTela();
            printf("\n\n");
            printf("%s****************************************%s\n", COR_AMARELO, COR_RESET);
            printf("%sLIMITE DE TURNOS ATINGIDO!%s\n", COR_AMARELO, COR_RESET);
            printf("%s****************************************%s\n", COR_AMARELO, COR_RESET);

            int terr_j1 = contarTerritorios(raiz, 1);
            int terr_j2 = contarTerritorios(raiz, 2);

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
    destruirJogador(j1);
    destruirJogador(j2);
    destruirMundo(raiz);

    printf("\nObrigado por jogar Code Battle!\n");
    return 0;
}
