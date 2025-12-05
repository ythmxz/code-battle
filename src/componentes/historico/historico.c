#include <stdio.h>
#include <stdlib.h>

#include "historico.h"

Historico *inicializarHistorico() {
    Historico *historico = (Historico *)malloc(sizeof(Historico));

    if (historico == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o histórico.\n");
        exit(1);
    }

    historico->topo = NULL;

    return historico;
}

void empilharHistorico(Historico *historico, Territorio *territorio) {
    Acao *acao = (Acao *)malloc(sizeof(Acao));

    if (acao == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a ação.\n");
        exit(1);
    }

    acao->territorio = territorio;
    acao->proximo = historico->topo;
    historico->topo = acao;
}

Territorio *desempilharHistorico(Historico *historico) {
    if (historico->topo == NULL) {
        return NULL;
    }

    Acao *acao = historico->topo;
    Territorio *territorio = acao->territorio;

    historico->topo = acao->proximo;

    free(acao);

    return territorio;
}

Territorio *topoHistorico(Historico *historico) {
    if (historico->topo == NULL) {
        return NULL;
    }

    return historico->topo->territorio;
}

int historicoVazio(Historico *historico) {
    return historico->topo == NULL;
}

void destruirHistorico(Historico *historico) {
    while (historico->topo != NULL) {
        Acao *acao = historico->topo;

        historico->topo = acao->proximo;
        free(acao);
    }

    free(historico);
}
