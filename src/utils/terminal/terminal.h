#ifndef TERMINAL_H
#define TERMINAL_H

#ifdef _WIN32
#define LIMPAR_TELA "cls"
#else
#define LIMPAR_TELA "clear"
#endif

#define COR_RESET "\033[0m"
#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_AMARELO "\033[1;33m"
#define COR_AZUL "\033[1;34m"
#define COR_MAGENTA "\033[1;35m"
#define COR_CIANO "\033[1;36m"
#define COR_BRANCO "\033[1;37m"

/// @brief Limpa a tela do terminal.
void limpar_tela(void);

/// @brief Limpa o buffer de entrada.
void limpar_buffer(void);

/// @brief Aguarda o usuário pressionar a tecla Enter.
void pressionar_enter(void);

#endif // TERMINAL_H
