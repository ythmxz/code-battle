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
        printf("O destino não existe.\n\n");
        return 0;
    }

    if (destino == adversario->atual) {
        printf("%sMOVIMENTO BLOQUEADO%s\n", COR_VERMELHO, COR_RESET);
        printf("%s está neste território!\n\n", adversario->nome);
        return 0;
    }

    if (destino == raizGlobal) {
        if (jogador->chave1Dominada == 0 || jogador->chave2Dominada == 0) {
            printf("%sACESSO NEGADO%s\n", COR_AMARELO, COR_RESET);
            printf("O Núcleo-X requer %sAMBAS as CHAVES%s!\n", COR_AMARELO, COR_RESET);
            printf("Chaves dominadas: %s%d/2%s\n\n", COR_AMARELO, jogador->chave1Dominada + jogador->chave2Dominada, COR_RESET);
            return 0;
        }
    }

    if (destino->ocupado != 0 && destino->ocupado != jogador->id) {
        printf("%sCOMBATE!%s\n", COR_VERMELHO, COR_RESET);
        printf("Território '%s%s%s' ocupado por %s%s%s\n\n", COR_CIANO, destino->nome, COR_RESET, COR_AMARELO, adversario->nome, COR_RESET);
        printf("%sDefesa:%s %d   %sAtaque:%s %d\n\n", COR_AZUL, COR_RESET, destino->escudo, COR_VERMELHO, COR_RESET, ataque(jogador));

        if (destino->escudo == 0) {
            printf("%sCONQUISTADO!%s\n", COR_VERDE, COR_RESET);
            printf("O território estava sem defesas!\n\n");

            if (destino->tipo == CHAVE) {
                if (destino == chave1Global) {
                    adversario->chave1Dominada = 0;
                    printf("%s%s perdeu a CHAVE 1!%s\n\n", COR_AMARELO, adversario->nome, COR_RESET);
                } else if (destino == chave2Global) {
                    adversario->chave2Dominada = 0;
                    printf("%s%s perdeu a CHAVE 2!%s\n\n", COR_AMARELO, adversario->nome, COR_RESET);
                }
            }

            destino->ocupado = jogador->id;
        } else if (ataque(jogador) >= destino->escudo) {
            printf("%sCONQUISTADO!%s\n", COR_VERDE, COR_RESET);
            printf("Defesas do território foram superadas!\n\n");

            if (destino->tipo == CHAVE) {
                if (destino == chave1Global) {
                    adversario->chave1Dominada = 0;
                    printf("%s%s perdeu a CHAVE 1!%s\n\n", COR_AMARELO, adversario->nome, COR_RESET);
                } else if (destino == chave2Global) {
                    adversario->chave2Dominada = 0;
                    printf("%s%s perdeu a CHAVE 2!%s\n\n", COR_AMARELO, adversario->nome, COR_RESET);
                }
            }

            destino->ocupado = jogador->id;
            destino->escudo -= 1;
        } else {
            printf("%sATAQUE FALHOU!%s\n", COR_VERMELHO, COR_RESET);
            printf("Defesas muito fortes! Ataque insuficiente.\n\n");
            return 0;
        }
    }

    printf("%s%s moveu-se: %s%s%s -> %s%s%s\n\n",
           COR_CIANO,
           jogador->nome,
           COR_MAGENTA,
           jogador->atual->nome,
           COR_RESET,
           COR_MAGENTA,
           destino->nome,
           COR_RESET);

    empilharHistorico(jogador->historico, jogador->atual);
    jogador->atual = destino;
    destino->ocupado = jogador->id;

    if (destino->tipo == CHAVE && destino->ocupado == jogador->id) {
        if (destino == chave1Global && !jogador->chave1Dominada) {
            jogador->chave1Dominada = 1;
            printf("%sCHAVE 1 DOMINADA!%s\n", COR_AMARELO, COR_RESET);
            printf("Se o território for perdido, a chave será perdida!\n\n");
        } else if (destino == chave2Global && !jogador->chave2Dominada) {
            jogador->chave2Dominada = 1;
            printf("%sCHAVE 2 DOMINADA!%s\n", COR_AMARELO, COR_RESET);
            printf("Se o território for perdido, a chave será perdida!\n\n");
        }
    }

    if (destino->tipo == RECURSO) {
        int nivel = nivelTerritorio(destino, raizGlobal);

        printf("%sRECURSOS COLETADOS!%s\n", COR_VERDE, COR_RESET);

        if (nivel <= 1) {
            int quantidade = 2 + rand() % 2;
            printf("Coletou %d %s:\n\n", quantidade, (quantidade > 1) ? "itens" : "item");
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"Espada", "Espada", "Espada", "Escudo", "Escudo"};
                const char *rec = recursos[rand() % 5];
                printf("%s+ %s%s\n", COR_VERDE, rec, COR_RESET);
                adicionarItem(jogador->inventario, rec);
            }
        } else if (nivel == 2) {
            int quantidade = 1 + rand() % 2;
            printf("Coletou %d %s:\n\n", quantidade, (quantidade > 1) ? "itens" : "item");
            for (int i = 0; i < quantidade; i++) {
                const char *recursos[] = {"Espada", "Escudo", "Escudo"};
                const char *rec = recursos[rand() % 3];
                printf("%s+ %s%s\n", COR_VERDE, rec, COR_RESET);
                adicionarItem(jogador->inventario, rec);
            }
        } else {
            const char *recursos[] = {"Espada", "Escudo"};
            const char *rec = recursos[rand() % 2];
            printf("Coletou 1 item:\n\n");
            printf("%s+ %s%s\n", COR_VERDE, rec, COR_RESET);
            adicionarItem(jogador->inventario, rec);
        }

        destino->tipo = NORMAL;
        printf("\n%sO território agora é NORMAL.%s\n\n", COR_CIANO, COR_RESET);
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

        // Cabecalho do turno
        printf("%sTURNO %d - %s (J%d)%s\n", COR_AZUL, turno, jogador->nome, jogador->id, COR_RESET);

        // Informacoes do territorio atual
        const char *tipoStr = jogador->atual->tipo == NORMAL    ? "NORMAL"
                              : jogador->atual->tipo == RECURSO ? "RECURSO"
                              : jogador->atual->tipo == CHAVE   ? "CHAVE"
                                                                : "BATALHA";
        const char *corTipo = jogador->atual->tipo == NORMAL    ? COR_RESET
                              : jogador->atual->tipo == RECURSO ? COR_VERDE
                              : jogador->atual->tipo == CHAVE   ? COR_AMARELO
                                                                : COR_VERMELHO;

        printf("%sTerritório:%s %s%s%s [%s%s%s | Defesa: %d]\n",
               COR_CIANO,
               COR_RESET,
               COR_MAGENTA,
               jogador->atual->nome,
               COR_RESET,
               corTipo,
               tipoStr,
               COR_RESET,
               jogador->atual->escudo);

        printf("%sAtaque:%s %d  %s|%s  %sEscudos:%s %d  %s|%s  %sChaves:%s %d/2\n\n",
               COR_VERMELHO,
               COR_RESET,
               ataque(jogador),
               COR_RESET,
               COR_RESET,
               COR_AZUL,
               COR_RESET,
               escudos(jogador),
               COR_RESET,
               COR_RESET,
               COR_AMARELO,
               COR_RESET,
               jogador->chave1Dominada + jogador->chave2Dominada);

        // Menu de movimentacao
        printf("%s┌─ MOVIMENTAÇÃO ────────────────────┐%s\n", COR_CIANO, COR_RESET);
        printf("%s│%s 1 - CIMA (%s%s%s)\n",
               COR_CIANO,
               COR_RESET,
               COR_MAGENTA,
               jogador->atual->pai ? jogador->atual->pai->nome : "N/A",
               COR_RESET);

        if (jogador->atual->esquerda)
            printf("%s│%s 2 - BAIXO ESQUERDA (%s%s%s)\n", COR_CIANO, COR_RESET, COR_MAGENTA, jogador->atual->esquerda->nome, COR_RESET);
        if (jogador->atual->direita)
            printf("%s│%s 3 - BAIXO DIREITA (%s%s%s)\n", COR_CIANO, COR_RESET, COR_MAGENTA, jogador->atual->direita->nome, COR_RESET);

        Territorio *lateral_esq = buscarIrmaoEsquerda(raiz, jogador->atual);
        Territorio *lateral_dir = buscarIrmaoDireita(raiz, jogador->atual);

        if (lateral_esq)
            printf("%s│%s 4 - LATERAL ESQUERDA (%s%s%s)\n", COR_CIANO, COR_RESET, COR_MAGENTA, lateral_esq->nome, COR_RESET);
        if (lateral_dir)
            printf("%s│%s 5 - LATERAL DIREITA (%s%s%s)\n", COR_CIANO, COR_RESET, COR_MAGENTA, lateral_dir->nome, COR_RESET);

        printf("%s└───────────────────────────────────┘%s\n\n", COR_CIANO, COR_RESET);

        // Menu de acoes
        printf("%s┌─ AÇÕES ───────────────────────────┐%s\n", COR_VERDE, COR_RESET);

        if (escudos(jogador) > 0)
            printf("%s│%s 6 - Colocar ESCUDO\n", COR_VERDE, COR_RESET);

        printf("%s│%s 7 - Ver MAPA\n", COR_VERDE, COR_RESET);
        printf("%s│%s 8 - STATUS adversário\n", COR_VERDE, COR_RESET);
        printf("%s│%s 9 - PASSAR turno\n", COR_VERDE, COR_RESET);
        printf("%s└───────────────────────────────────┘%s\n\n", COR_VERDE, COR_RESET);

        printf("%s>%s ", COR_BRANCO, COR_RESET);

        int escolha = 0;
        if (scanf("%d", &escolha) != 1 || (escolha == 2 && !jogador->atual->esquerda) || (escolha == 3 && !jogador->atual->direita) ||
            (escolha == 4 && !lateral_esq) || (escolha == 5 && !lateral_dir)) {
            printf("\n");
            limparBuffer();
            printf("%sOPÇÃO INVÁLIDA!%s\n\n", COR_VERMELHO, COR_RESET);
            prompt();
            continue;
        }

        printf("\n");
        limparBuffer();

        switch (escolha) {
        case 1: // Mover para pai
            if (jogador->atual->pai) {
                if (tentarMovimento(jogador->atual->pai, jogador, adversario)) {
                    turnoConcluido = 1;
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

        case 4: // Movimento lateral esquerda
            if (lateral_esq) {
                if (tentarMovimento(lateral_esq, jogador, adversario)) {
                    turnoConcluido = 1;
                }
            }
            prompt();
            break;

        case 5: // Movimento lateral direita
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
                jogador->atual->tipo = BATALHA;
                printf("%sESCUDO COLOCADO!%s\n", COR_AZUL, COR_RESET);
                printf("Defesa em %s%s%s: %s%d%s\n\n",
                       COR_MAGENTA,
                       jogador->atual->nome,
                       COR_RESET,
                       COR_VERDE,
                       jogador->atual->escudo,
                       COR_RESET);
                printf("%sO território agora é do tipo BATALHA.%s\n\n", COR_VERMELHO, COR_RESET);
                turnoConcluido = 1;
            }
            prompt();
            break;

        case 7: // Ver mapa
            printf("%sMAPA%s\n", COR_AZUL, COR_RESET);
            imprimirMundo(raiz, 0);
            printf("\n");
            prompt();
            break;

        case 8: // Ver status adversario
            printf("%sSTATUS ADVERSÁRIO%s\n", COR_AMARELO, COR_RESET);
            printf("Nome: %s%s%s (J%d)\n", COR_MAGENTA, adversario->nome, COR_RESET, adversario->id);
            printf("Território: %s%s%s\n", COR_MAGENTA, adversario->atual->nome, COR_RESET);
            printf("Chaves: %s%d/2%s\n", COR_AMARELO, adversario->chave1Dominada + adversario->chave2Dominada, COR_RESET);
            printf("Ataque: %s%d%s | Escudos: %s%d%s\n\n",
                   COR_VERMELHO,
                   ataque(adversario),
                   COR_RESET,
                   COR_AZUL,
                   escudos(adversario),
                   COR_RESET);
            prompt();
            break;

        case 9: // Passar turno
            printf("%s%s passou o turno.%s\n\n", COR_CIANO, jogador->nome, COR_RESET);
            turnoConcluido = 1;
            prompt();
            break;

        default:
            printf("%sOPÇÃO INVÁLIDA!%s\n\n", COR_VERMELHO, COR_RESET);
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

    // Tela de titulo
    printf("%sCODE BATTLE%s\n\n", COR_MAGENTA, COR_RESET);
    printf("%sObjetivo:%s Dominar %sAMBAS as CHAVES%s e conquistar o %sNúcleo-X%s!\n\n",
           COR_BRANCO,
           COR_RESET,
           COR_AMARELO,
           COR_RESET,
           COR_CIANO,
           COR_RESET);

    Territorio *raiz = criarMundo();
    raizGlobal = raiz;

    chave1Global = buscarChave(raiz, 1);
    chave2Global = buscarChave(raiz, 2);

    printf("%sTerritorios CHAVE:%s\n\n", COR_AMARELO, COR_RESET);
    printf("CHAVE 1: %s%s%s\n", COR_MAGENTA, (chave1Global != NULL) ? chave1Global->nome : "N/A", COR_RESET);
    printf("CHAVE 2: %s%s%s\n\n", COR_MAGENTA, (chave2Global != NULL) ? chave2Global->nome : "N/A", COR_RESET);

    Territorio *spawn1 = buscarBaseEsquerda(raiz);
    Territorio *spawn2 = buscarBaseDireita(raiz);

    char nomeJogador1[32];
    char nomeJogador2[32];

    printf("%sDigite o nome do Jogador 1:%s ", COR_VERDE, COR_RESET);
    fgets(nomeJogador1, sizeof(nomeJogador1), stdin);
    nomeJogador1[strcspn(nomeJogador1, "\n")] = 0;

    printf("%sDigite o nome do Jogador 2:%s ", COR_VERDE, COR_RESET);
    fgets(nomeJogador2, sizeof(nomeJogador2), stdin);
    nomeJogador2[strcspn(nomeJogador2, "\n")] = 0;

    Jogador *j1 = criarJogador(nomeJogador1, 1, spawn1);
    Jogador *j2 = criarJogador(nomeJogador2, 2, spawn2);

    limparTela();
    printf("%sJOGO INICIADO!%s\n\n", COR_VERDE, COR_RESET);

    printf("%s%s%s nasce em: %s%s%s\n", COR_VERDE, j1->nome, COR_RESET, COR_MAGENTA, spawn1->nome, COR_RESET);
    printf("%s%s%s nasce em: %s%s%s\n\n", COR_VERDE, j2->nome, COR_RESET, COR_MAGENTA, spawn2->nome, COR_RESET);

    printf("%sMAPA INICIAL:%s\n\n", COR_AZUL, COR_RESET);
    imprimirMundo(raiz, 0);
    printf("\n");
    prompt();

    int turno = 1;
    int vencedor = 0;

    while (!vencedor) {
        Jogador *atual = (turno % 2 == 1) ? j1 : j2;
        Jogador *oponente = (turno % 2 == 1) ? j2 : j1;

        turnoJogador(raiz, atual, oponente, turno);

        if (verificarVitoria(atual, raiz)) {
            limparTela();
            printf("%s%s VENCEU!%s\n\n", COR_VERDE, atual->nome, COR_RESET);
            printf("%sAmbas CHAVES dominadas%s\n", COR_AMARELO, COR_RESET);
            printf("%sNúcleo-X conquistado%s\n\n", COR_CIANO, COR_RESET);
            vencedor = atual->id;
            break;
        }

        if (turno >= LIMITE_TURNOS) {
            limparTela();
            printf("%sLIMITE DE TURNOS ATINGIDO!%s\n\n", COR_AMARELO, COR_RESET);

            int territoriosJogador1 = contarTerritorios(raiz, 1);
            int territoriosJogador2 = contarTerritorios(raiz, 2);

            printf("%sContagem de territórios:%s\n\n", COR_BRANCO, COR_RESET);
            printf("%s%s:%s %s%d%s territórios\n", COR_VERDE, j1->nome, COR_RESET, COR_CIANO, territoriosJogador1, COR_RESET);
            printf("%s%s:%s %s%d%s territórios\n\n", COR_VERDE, j2->nome, COR_RESET, COR_CIANO, territoriosJogador2, COR_RESET);

            if (territoriosJogador1 > territoriosJogador2) {
                printf("%s%s VENCE POR TERRITÓRIOS!%s\n", COR_VERDE, j1->nome, COR_RESET);
                vencedor = 1;
            } else if (territoriosJogador2 > territoriosJogador1) {
                printf("%s%s VENCE POR TERRITÓRIOS!%s\n", COR_VERDE, j2->nome, COR_RESET);
                vencedor = 2;
            } else {
                printf("%sEMPATE! Ambos dominaram %d territórios.%s\n", COR_AMARELO, territoriosJogador1, COR_RESET);
            }
            break;
        }

        turno++;
    }

    destruirJogador(j1);
    destruirJogador(j2);
    destruirMundo(raiz);

    printf("\n%sObrigado por jogar Code Battle!%s\n\n", COR_MAGENTA, COR_RESET);
    return 0;
}
