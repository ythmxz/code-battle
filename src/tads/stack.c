#include "stack.h"
#include <stdlib.h>

Pilha *pilha_criar() {
    Pilha *p = malloc(sizeof(Pilha));
    p->topo = NULL;
    return p;
}

void pilha_empilhar(Pilha *p, struct No *v) {
    NoPilha *n = malloc(sizeof(NoPilha));
    n->valor = v;
    n->proximo = p->topo;
    p->topo = n;
}

struct No *pilha_desempilhar(Pilha *p) {
    if (!p->topo)
        return NULL;
    NoPilha *n = p->topo;
    struct No *v = n->valor;
    p->topo = n->proximo;
    free(n);
    return v;
}

struct No *pilha_topo(Pilha *p) {
    if (!p->topo)
        return NULL;
    return p->topo->valor;
}

int pilha_vazia(Pilha *p) {
    return p->topo == NULL;
}

void pilha_destruir(Pilha *p) {
    while (p->topo) {
        NoPilha *n = p->topo;
        p->topo = n->proximo;
        free(n);
    }
    free(p);
}
