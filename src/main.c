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

Territorio *raizGlobal = NULL;
Territorio *chave1Global = NULL;
Territorio *chave2Global = NULL;

int tentarMovimento(Territorio *destino, Jogador *jogador, Jogador *adversario) {
    if (destino == NULL) {
        printf("%sMOVIMENTO INVÁLIDO%s\n", COR_VERMELHO, COR_RESET);
        printf("%sO destino não existe.%s\n\n", COR_VERMELHO, COR_RESET);

        return 0;
    }

    if (destino == adversario->atual) {
        printf("%sMOVIMENTO BLOQUEADO%s\n", COR_VERMELHO, COR_RESET);
        printf("%sO J%d está neste território!%s\n\n", COR_VERMELHO, adversario->id, COR_RESET);

        return 0;
    }

    if (destino == raizGlobal) {
        if (jogador->chave1Dominada == 0 || jogador->chave2Dominada == 0) {
            printf("%sMOVIMENTO INVÁLIDO%s\n", COR_VERMELHO, COR_RESET);
            printf("%sO Núcleo-X só pode ser acessado após dominar AMBAS as chaves!%s\n\n", COR_VERMELHO, COR_RESET);
            printf("%sChaves dominadas: %d/2%s\n", COR_AMARELO, jogador->chave1Dominada + jogador->chave2Dominada, COR_RESET);

            return 0;
        }
    }

    if (destino->ocupado != 0 && destino->ocupado != jogador->id) {
        printf("%sCOMBATE%s\n", COR_AZUL, COR_RESET);
        printf("O território '%s' está ocupado por J%d!\n\n", destino->nome, destino->ocupado);
        printf("%sEscudo: %d%s X %sAtaque: %d%s\n\n", COR_VERDE, destino->escudo, COR_RESET, COR_VERDE, ataque(jogador), COR_RESET);

        if (destino->escudo == 0) {
            printf("%sCONQUISTADO%s\n", COR_VERDE, COR_RESET);
            printf("%sO território não possuia escudo!%s\n\n", COR_MAGENTA, COR_RESET);

            if (destino->tipo == CHAVE) {
                if (destino == chave1Global) {
                    adversario->chave1Dominada = 0;
                    printf("%s%s perdeu a CHAVE 1!%s\n", COR_VERMELHO, adversario->nome, COR_RESET);
                }

                else if (destino == chave2Global) {
                    adversario->chave2Dominada = 0;
                    printf("%s%s perdeu a CHAVE 2!%s\n", COR_VERMELHO, adversario->nome, COR_RESET);
                }
            }

            destino->ocupado = jogador->id;
        }

        else if (ataque(jogador) >= destino->escudo) {
            printf("%sCONQUISTADO%s\n", COR_VERDE, COR_RESET);
            printf("%sVocê superou as defesas do território!%s\n\n", COR_VERDE, COR_RESET);

            if (destino->tipo == CHAVE) {
                if (destino == chave1Global) {
                    adversario->chave1Dominada = 0;
                    printf("%s%s perdeu a CHAVE 1!%s\n", COR_VERMELHO, adversario->nome, COR_RESET);
                }

                else if (destino == chave2Global) {
                    adversario->chave2Dominada = 0;
                    printf("%s%s perdeu a CHAVE 2!%s\n", COR_VERMELHO, adversario->nome, COR_RESET);
                }
            }

            destino->ocupado = jogador->id;
            destino->escudo -= 1;
        }

        else {
            printf("%sNÃO CONQUISTADO%s\n", COR_VERMELHO, COR_RESET);
            printf("%sAs defesas do território são muito fortes!%s\n\n", COR_VERMELHO, COR_RESET);

            return 0;
        }
    }

    printf("%s%s se movimentou de '%s' para '%s'.%s\n\n", COR_CIANO, jogador->nome, jogador->atual->nome, destino->nome, COR_RESET);

    empilharHistorico(jogador->historico, jogador->atual);
    jogador->atual = destino;
    destino->ocupado = jogador->id;

    if (destino->tipo == CHAVE && destino->ocupado == jogador->id) {
        if (destino == chave1Global && !jogador->chave1Dominada) {
            jogador->chave1Dominada = 1;
            printf("%sCHAVE 1 DOMINADA%s\n", COR_AMARELO, COR_RESET);
            printf("Se o território for dominado, a chave será perdida!\n");
        }

        else if (destino == chave2Global && !jogador->chave2Dominada) {
            jogador->chave2Dominada = 1;
            printf("%sCHAVE 2 DOMINADA%s\n", COR_AMARELO, COR_RESET);
            printf("Se o território for dominado, a chave será perdida!\n");
        }
    }

    if (destino->tipo == RECURSO) {
        int nivel = nivelTerritorio(destino, raizGlobal);

        // Recursos melhores e em maior quantidade em níveis altos (próximo da raiz)
        printf("%sRECURSO%s\n", COR_AZUL, COR_RESET);

        if (nivel <= 1) {
            // Níveis 0-1: 2-3 recursos
            int quantidade = 2 + rand() % 2;
            printf("Coletando %d item(ns):\n\n", quantidade);
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"Espada", "Espada", "Espada", "Escudo", "Escudo"};
                const char *rec = recursos[rand() % 5];
                printf("+ %s\n", rec);
                adicionarItem(jogador->inventario, rec);
            }
        } else if (nivel == 2) {
            // Nível 2: 1-2 recursos
            int quantidade = 1 + rand() % 2;
            printf("Coletando %d item(ns):\n\n", quantidade);
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"Espada", "Escudo", "Escudo"};
                const char *rec = recursos[rand() % 3];
                printf("+ %s\n", rec);
                adicionarItem(jogador->inventario, rec);
            }
        } else {
            // Nível 3: 1 recurso
            const char *recursos[] = {"Espada", "Escudo"};
            const char *rec = recursos[rand() % 2];
            printf("Coletando:\n\n+%s\n", rec);
            adicionarItem(jogador->inventario, rec);
        }

        destino->tipo = NORMAL;
        printf("\nO território agora é NORMAL.\n");
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

void turnoJogador(Territorio *raiz, Jogador *jogador, Jogador *adversario, int turno) {
    int turnoConcluido = 0;

    while (!turnoConcluido) {
        limparTela();

        printf("%s========================================%s\n", COR_AZUL, COR_RESET);
        printf("%sTurno: %d | %s (J%d)%s\n", COR_CIANO, turno, jogador->nome, jogador->id, COR_RESET);
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
               jogador->chave1Dominada + jogador->chave2Dominada,
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

        if (escudos(jogador) > 0)
            printf("6 - Colocar ESCUDO\n");

        printf("7 - Ver MAPA\n");
        printf("8 - STATUS adversário\n");
        printf("9 - PASSAR turno\n\n");

        printf("> ");

        int escolha = 0;
        if (scanf("%d", &escolha) != 1 || (escolha == 2 && !jogador->atual->esquerda) || (escolha == 3 && !jogador->atual->direita) ||
            (escolha == 4 && !lateral_esq) || (escolha == 5 && !lateral_dir)) {
            printf("\n");
            limparBuffer();
            printf("OPÇÃO INVÁLIDA.\n");
            prompt();
            continue;
        }

        printf("\n");
        limparBuffer();

        switch (escolha) {
        case 1: // Mover para pai
            if (jogador->atual->pai) {
                if (tentarMovimento(jogador->atual->pai, jogador, adversario)) {
                    turnoConcluido = 1; // Movimento passa o turno
                }
            }

            prompt();
            break;

        case 2: // Mover para filho esquerdo
            if (jogador->atual->esquerda) {
                if (tentarMovimento(jogador->atual->esquerda, jogador, adversario)) {
                    turnoConcluido = 1;
                }
            }

            prompt();
            break;

        case 3: // Mover para filho direito
            if (jogador->atual->direita) {
                if (tentarMovimento(jogador->atual->direita, jogador, adversario)) {
                    turnoConcluido = 1;
                }
            }

            prompt();
            break;

        case 4: // Movimento lateral para esquerda
            Territorio *lateral_esq = buscarIrmaoEsquerda(raiz, jogador->atual);
            if (lateral_esq) {
                if (tentarMovimento(lateral_esq, jogador, adversario)) {
                    turnoConcluido = 1;
                }
            }

            prompt();
            break;

        case 5: // Movimento lateral para direita
            Territorio *lateral_dir = buscarIrmaoDireita(raiz, jogador->atual);
            if (lateral_dir) {
                if (tentarMovimento(lateral_dir, jogador, adversario)) {
                    turnoConcluido = 1;
                }
            }

            prompt();
            break;

        case 6: // Colocar escudo
            if (escudos(jogador) > 0) {
                removerItem(jogador->inventario, "Escudo");
                jogador->atual->escudo++;
                printf("Escudo colocado!\nESCUDOS (%s): %d\n", jogador->atual->nome, jogador->atual->escudo);
                turnoConcluido = 1; // Colocar escudo passa o turno
            }

            prompt();
            break;

        case 7: // Ver mapa (NÃO passa o turno)
            printf("MAPA:\n\n");
            imprimirMundo(raiz, 0);
            prompt();
            break;

        case 8: // Ver status do adversário (NÃO passa o turno)
            printf("STATUS ADVERSÁRIO:\n\n");
            printf("Nome: %s (J%d)\n", adversario->nome, adversario->id);
            printf("Território: %s\n", adversario->atual->nome);
            printf("Chaves: %d/2\n", adversario->chave1Dominada + adversario->chave2Dominada);
            printf("Ataque: %d | Escudos: %d\n", ataque(adversario), escudos(adversario));

            prompt();
            break;

        case 9: // Passar turno explicitamente
            printf("%s passou.\n", jogador->nome);
            turnoConcluido = 1;

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
    if (jogador->chave1Dominada && jogador->chave2Dominada && jogador->atual == raiz) {
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
    raizGlobal = raiz; // Salvar raiz global

    // Identificar nós CHAVE
    chave1Global = buscarChave(raiz, 1);
    chave2Global = buscarChave(raiz, 2);

    printf("Territórios CHAVE:\n\n");
    printf("CHAVE 1: %s\n", (chave1Global != NULL) ? chave1Global->nome : "N/A");
    printf("CHAVE 2: %s\n\n", (chave2Global != NULL) ? chave2Global->nome : "N/A");

    // Posicionar jogadores
    Territorio *spawn1 = buscarBaseEsquerda(raiz);
    Territorio *spawn2 = buscarBaseDireita(raiz);

    char nomeJogador1[32];
    char nomeJogador2[32];

    printf("Digite o nome do Jogador 1: ");
    fgets(nomeJogador1, sizeof(nomeJogador1), stdin);
    nomeJogador1[strcspn(nomeJogador1, "\n")] = 0;

    printf("Digite o nome do Jogador 2: ");
    fgets(nomeJogador2, sizeof(nomeJogador2), stdin);
    nomeJogador2[strcspn(nomeJogador2, "\n")] = 0;

    Jogador *j1 = criarJogador(nomeJogador1, 1, spawn1);
    Jogador *j2 = criarJogador(nomeJogador2, 2, spawn2);

    limparTela();
    printf("%sJOGO INICIADO!%s\n\n", COR_VERDE, COR_RESET);

    printf("%s nasce em: %s\n", j1->nome, spawn1->nome);
    printf("%s nasce em: %s\n\n", j2->nome, spawn2->nome);

    printf("MAPA INICIAL:\n\n");
    imprimirMundo(raiz, 0);
    prompt();

    // Loop do jogo
    int turno = 1;
    int vencedor = 0;

    while (!vencedor) {
        Jogador *atual = (turno % 2 == 1) ? j1 : j2;
        Jogador *oponente = (turno % 2 == 1) ? j2 : j1;

        turnoJogador(raiz, atual, oponente, turno);

        if (verificarVitoria(atual, raiz)) {
            limparTela();
            printf("%s%s VENCEU O JOGO!%s\n\n", COR_VERDE, atual->nome, COR_RESET);
            printf("%s- Ambas CHAVES dominadas%s\n", COR_AMARELO, COR_RESET);
            printf("%s- Núcleo-X conquistado%s\n", COR_AMARELO, COR_RESET);
            vencedor = atual->id;
            break;
        }

        // Verificar limite de turnos
        if (turno >= LIMITE_TURNOS) {
            limparTela();
            printf("%sLIMITE DE TURNOS ATINGIDO!%s\n\n", COR_AMARELO, COR_RESET);

            int territoriosJogador1 = contarTerritorios(raiz, 1);
            int territoriosJogador2 = contarTerritorios(raiz, 2);

            printf("\n%sContagem de territórios:%s\n\n", COR_BRANCO, COR_RESET);
            printf("  %s: %d territórios\n", j1->nome, territoriosJogador1);
            printf("  %s: %d territórios\n\n", j2->nome, territoriosJogador2);

            if (territoriosJogador1 > territoriosJogador2) {
                printf("%s%s VENCE POR TERRITÓRIOS!%s\n", COR_VERDE, j1->nome, COR_RESET);
                vencedor = 1;
            } else if (territoriosJogador2 > territoriosJogador1) {
                printf("%s%s VENCE POR TERRITÓRIOS!%s\n", COR_VERDE, j2->nome, COR_RESET);
                vencedor = 2;
            } else {
                printf("%sEMPATE! Ambos dominaram %d territórios%s\n", COR_AMARELO, territoriosJogador1, COR_RESET);
            }
            break;
        }

        turno++;
    }

    // Limpeza
    destruirJogador(j1);
    destruirJogador(j2);
    destruirMundo(raiz);

    printf("\nObrigado por jogar Code Battle!\n\n");
    return 0;
}
