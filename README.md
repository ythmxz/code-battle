gcc src\*.c -o code_battle.exe
# Code Battle v2.1

Jogo **Code Battle** implementado em C com mecânicas avançadas de combate e conquista territorial.

## Compilação e Execução

### Windows (PowerShell com gcc/MinGW)

```powershell
gcc -I src src\*.c src\tads\*.c -o CodeBattle.exe
.\CodeBattle.exe
```

### Linux/Unix

```bash
gcc -I src src/*.c src/tads/*.c -o CodeBattle
./CodeBattle
```

**Nota**: O código detecta automaticamente a plataforma e ajusta comportamento (limpeza de tela, cores ANSI).

## Descrição do Jogo

Dois jogadores começam em extremos opostos de uma árvore binária completa (15 nós) e competem para:
1. **Dominar ambos os nós CHAVE** (posicionados aleatoriamente)
2. **Conquistar o Núcleo-X** (raiz da árvore)

### Mecânicas Principais

#### Movimentação
- Os jogadores podem se mover **em todas as direções**:
  - **Subir** (para o nó pai)
  - **Descer** (para nós filhos esquerdo/direito)
  - **Lateral verdadeiro** (para nós adjacentes no mesmo nível, não circular)
    - Exemplo: de H pode ir para G ou I
    - Mas de G não pode ir para N (não são adjacentes)
- Jogadores podem revisitar nós já dominados
- Cada movimento registra histórico (TAD Pilha)
- **Núcleo-X bloqueado**: só pode ser acessado após dominar AMBAS as chaves

#### Sistema de Combate
- Nós ocupados pelo jogador possuem **escudos** (proteção)
- Para conquistar nó inimigo: **ataque ≥ escudo do nó**
  - **Ataque** = quantidade de espadas no inventário
  - **Escudo do nó** = escudos colocados pelo adversário
- Se escudo = 0, nó é conquistado automaticamente
- Spawn inicial de cada jogador começa com 1 escudo

**IMPORTANTE**:
- Jogador **só domina** nó se estiver vazio OU conquistá-lo em combate
- Apenas **passar** por nó do adversário com escudo NÃO muda a ocupação
- Nós abandonados (sem escudo) voltam a ficar vazios

#### Recursos
- **Espada**: aumenta poder de ataque
- **Escudo**: pode ser colocado em nós para protegê-los
- **Energia**: recurso extra (uso futuro)
- Coletados ao entrar em nós do tipo **RECURSO** (apenas uma vez por nó)
- **Recursos escalam com o nível**:
  - Nível 0-1 (próximo à raiz): 2-3 recursos, prioridade espadas/escudos
  - Nível 2: 1-2 recursos aleatórios
  - Nível 3 (folhas): 1 recurso aleatório
- Após coleta, nó RECURSO vira NORMAL (recurso consumido)

#### Tipos de Nó
- **NORMAL**: passagem livre
- **RECURSO**: coleta automática de item aleatório
- **CHAVE**: deve ser dominado para vitória (2 nós no mapa)

#### Condição de Vitória

**Vitória Normal** - Jogador vence ao atender **todas** as condições:
1. Dominar a **CHAVE 1** (ocupar o nó CHAVE)
2. Dominar a **CHAVE 2** (ocupar o nó CHAVE)
3. Estar no **Núcleo-X** (raiz) - **só acessível após ter ambas as chaves**

**Vitória por Limite de Turnos**:
- Se a partida chegar a **100 turnos** sem vencedor
- O jogador com **mais territórios dominados** vence
- Em caso de empate, é declarado empate

## Estrutura do Código

### Interface e Usabilidade
- ✨ **Cores ANSI**: Interface colorida (Windows 10+ e Linux)
  - Vermelho: combate e ataque
  - Verde: recursos e vitória
  - Amarelo: chaves e alertas
  - Azul/Ciano: informações gerais
- 📢 **Tela limpa**: A cada turno a tela é limpa para melhor visualização
- 🔄 **Cross-platform**: Funciona em Windows e Linux (detecção automática)

### Arquivos Principais

- `src/tree.h/c` - Estrutura da árvore binária (nós com ponteiros pai/irmão)
- `src/player.h/c` - Estrutura do jogador e controle de inventário
- `src/tads/list.h/c` - TAD Lista Encadeada (inventário)
- `src/tads/stack.h/c` - TAD Pilha (histórico de movimentos)
- `src/main.c` - Loop principal do jogo e lógica de turnos

## Conceitos Implementados

- ✅ Alocação dinâmica de memória
- ✅ Ponteiros para estruturas (pai, filhos, irmão)
- ✅ TAD Lista Encadeada (inventário)
- ✅ TAD Pilha (histórico)
- ✅ Árvore binária completa (15 nós, altura 3)
- ✅ Recursividade (busca de nós, configuração de ponteiros)
- ✅ Modularização (headers e sources separados)
- ✅ Código documentado em português
