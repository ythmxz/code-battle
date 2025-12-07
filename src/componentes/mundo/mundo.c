#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mundo.h"

Territorio *criarTerritorio(const char *nome, TipoTerritorio tipo) {
    Territorio *territorio = (Territorio *)malloc(sizeof(Territorio));

    if (territorio == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o território.\n");
        exit(1);
    }

    strncpy(territorio->nome, nome, TAMANHO_NOME - 1);
    territorio->nome[TAMANHO_NOME - 1] = '\0';
    territorio->tipo = tipo;
    territorio->ocupado = 0;
    territorio->escudo = 0;
    territorio->pai = NULL;
    territorio->esquerda = NULL;
    territorio->direita = NULL;

    return territorio;
}

Territorio *criarMundo() {
    // Nível 0

    Territorio *nucleoX = criarTerritorio("Nucleo-X", NORMAL);

    // Nível 1

    Territorio *moonad = criarTerritorio("Moonad", NORMAL);
    Territorio *sunad = criarTerritorio("Sunad", NORMAL);

    // Nível 2

    Territorio *ounica = criarTerritorio("Ounica", NORMAL);
    Territorio *pharia = criarTerritorio("Pharia", NORMAL);
    Territorio *iphido = criarTerritorio("Iphido", NORMAL);
    Territorio *leles = criarTerritorio("Leles", NORMAL);

    // Nível 3

    Territorio *vivus = criarTerritorio("Vivus", NORMAL);
    Territorio *irilon = criarTerritorio("Irilon", NORMAL);
    Territorio *zealan = criarTerritorio("Zealan", NORMAL);
    Territorio *yoria = criarTerritorio("Yoria", NORMAL);
    Territorio *muwen = criarTerritorio("Muwen", NORMAL);
    Territorio *ozidian = criarTerritorio("Ozidian", NORMAL);
    Territorio *gukar = criarTerritorio("Gukar", NORMAL);
    Territorio *vitalis = criarTerritorio("Vitalis", NORMAL);

    // Conectar territórios

    nucleoX->esquerda = moonad;
    nucleoX->direita = sunad;
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

    configurarPonteirosMundo(nucleoX, NULL);

    Territorio *candidatos[] = {
        moonad, sunad, ounica, pharia, iphido, leles, irilon, zealan, yoria, muwen, ozidian, gukar}; // 12 candidatos
    int tamanhoCandidatos = sizeof(candidatos) / sizeof(candidatos[0]);

    int chave1 = rand() % tamanhoCandidatos;
    int chave2 = 0;

    do {
        chave2 = rand() % tamanhoCandidatos;
    } while (chave2 == chave1);

    candidatos[chave1]->tipo = CHAVE;
    candidatos[chave2]->tipo = CHAVE;

    int quantidadeRecursos = 7 + rand() % 3;
    for (int i = 0; i < quantidadeRecursos; i++) {
        int recurso = rand() % tamanhoCandidatos;

        if (candidatos[recurso]->tipo == NORMAL) {
            candidatos[recurso]->tipo = RECURSO;
        }
    }

    return nucleoX;
}

void configurarPonteirosMundo(Territorio *territorio, Territorio *territorioPai) {
    if (territorio == NULL)
        return;
    territorio->pai = territorioPai;

    configurarPonteirosMundo(territorio->esquerda, territorio);
    configurarPonteirosMundo(territorio->direita, territorio);
}

Territorio *buscarBaseEsquerda(Territorio *territorio) {
    if (territorio == NULL) {
        return NULL;
    }

    if (territorio->esquerda == NULL && territorio->direita == NULL) {
        return territorio;
    }

    return buscarBaseEsquerda(territorio->esquerda);
}

Territorio *buscarBaseDireita(Territorio *territorio) {
    if (territorio == NULL) {
        return NULL;
    }

    if (territorio->esquerda == NULL && territorio->direita == NULL) {
        return territorio;
    }

    return buscarBaseDireita(territorio->direita);
}

static void buscarChavesRecursivo(Territorio *territorio, Territorio **chave1, Territorio **chave2) {
    if (territorio == NULL) {
        return;
    }

    if (territorio->tipo == CHAVE) {
        if (*chave1 == NULL) {
            *chave1 = territorio;
        }

        else if (*chave2 == NULL) {
            *chave2 = territorio;
        }
    }

    buscarChavesRecursivo(territorio->esquerda, chave1, chave2);
    buscarChavesRecursivo(territorio->direita, chave1, chave2);
}

Territorio *buscarChave(Territorio *territorio, int chave) {
    Territorio *chave1 = NULL;
    Territorio *chave2 = NULL;

    buscarChavesRecursivo(territorio, &chave1, &chave2);

    if (chave == 1) {
        return chave1;
    }

    else if (chave == 2) {
        return chave2;
    }

    return NULL;
}

int nivelTerritorio(Territorio *territorio, Territorio *nucleo) {
    if (territorio == NULL) {
        return -1;
    }

    if (territorio == nucleo) {
        return 0;
    }

    int nivelEsquerda = nivelTerritorio(territorio->esquerda, nucleo);

    if (nivelEsquerda >= 0) {
        return nivelEsquerda + 1;
    }

    int nivelDireita = nivelTerritorio(territorio->direita, nucleo);

    if (nivelDireita >= 0) {
        return nivelDireita + 1;
    }

    return -1;
}

static void contarTerritoriosNivel(Territorio *territorio, int nivelAlvo, int nivelAtual, Territorio **lista, int *contador) {
    if (territorio == NULL || *contador >= 15) {
        return;
    }

    if (nivelAtual == nivelAlvo) {
        lista[*contador] = territorio;
        (*contador)++;

        return;
    }

    contarTerritoriosNivel(territorio->esquerda, nivelAlvo, nivelAtual + 1, lista, contador);
    contarTerritoriosNivel(territorio->direita, nivelAlvo, nivelAtual + 1, lista, contador);
}

Territorio *buscarIrmaoEsquerda(Territorio *territorio, Territorio *atual) {
    if (territorio == NULL || atual == NULL) {
        return NULL;
    }

    int nivel = nivelTerritorio(territorio, atual);

    if (nivel < 0) {
        return NULL;
    }

    // Coletar todos os nós do mesmo nível

    Territorio *territoriosNivel[15];
    int contador = 0;

    contarTerritoriosNivel(territorio, nivel, 0, territoriosNivel, &contador);

    for (int i = 0; i < contador; i++) {
        if (territoriosNivel[i] == atual) {
            if (i > 0) {
                return territoriosNivel[i - 1];
            }

            return NULL;
        }
    }

    return NULL;
}

Territorio *buscarIrmaoDireita(Territorio *territorio, Territorio *atual) {
    if (territorio == NULL || atual == NULL) {
        return NULL;
    }

    int nivel = nivelTerritorio(territorio, atual);
    if (nivel < 0) {
        return NULL;
    }

    Territorio *territoriosNivel[15];
    int contador = 0;

    contarTerritoriosNivel(territorio, nivel, 0, territoriosNivel, &contador);

    for (int i = 0; i < contador; i++) {
        if (territoriosNivel[i] == atual) {
            if (i < contador - 1) {
                return territoriosNivel[i + 1];
            }
            return NULL;
        }
    }

    return NULL;
}

void imprimirMundo(Territorio *territorio, int nivel) {
    if (territorio == NULL) {
        return;
    }

    for (int i = 0; i < nivel; i++) {
        printf("|--");
    }

    const char *tipoNome[] = {"NORMAL", "RECURSO", "CHAVE"};
    printf("'%s' [%s | OCUPAÇÃO: %d | ESCUDO(S): %d]\n",
           territorio->nome,
           tipoNome[territorio->tipo],
           territorio->ocupado,
           territorio->escudo);

    imprimirMundo(territorio->esquerda, nivel + 1);
    imprimirMundo(territorio->direita, nivel + 1);
}

void destruirMundo(Territorio *territorio) {
    if (territorio == NULL) {
        return;
    }

    destruirMundo(territorio->esquerda);
    destruirMundo(territorio->direita);
    free(territorio);
}
