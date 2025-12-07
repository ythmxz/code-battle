#include <stdio.h>
#include <stdlib.h>

#include "terminal.h"

void limparTela() {
    system(LIMPAR_TELA);
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void prompt() {
    printf("\n%s[Pressione ENTER para continuar]%s\n", COR_CIANO, COR_RESET);
    getchar();
}
