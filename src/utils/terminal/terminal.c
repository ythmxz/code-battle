#include <stdio.h>
#include <stdlib.h>

#include "terminal.h"

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
