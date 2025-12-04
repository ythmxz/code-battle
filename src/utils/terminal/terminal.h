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

// Funções para terminal
void limpar_tela(void);
void limpar_buffer(void);
void pressionar_enter();