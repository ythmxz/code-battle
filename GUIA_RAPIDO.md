# Guia Rápido - Code Battle

## Como Jogar

### Compilar e Executar

**Windows (PowerShell):**
```powershell
gcc -I src src\*.c src\tads\*.c -o CodeBattle.exe
.\CodeBattle.exe
```

**Linux/Unix:**
```bash
gcc -I src src/*.c src/tads/*.c -o CodeBattle
./CodeBattle
```

### Objetivo
1. Dominar **ambos os nós CHAVE** (marcados como [CHAVE])
2. Conquistar o **Núcleo-X** (raiz da árvore)

### Ações Disponíveis por Turno

1. **Subir (mover para PAI)** - Subir na árvore (em direção à raiz)
2. **Descer para FILHO ESQUERDO** - Ir para o filho esquerdo (mostra nome do nó)
3. **Descer para FILHO DIREITO** - Ir para o filho direito (mostra nome do nó)
4. **Movimento LATERAL para ESQUERDA** - Mover para nó adjacente à esquerda no mesmo nível (mostra nome)
5. **Movimento LATERAL para DIREITA** - Mover para nó adjacente à direita no mesmo nível (mostra nome)
6. **Colocar ESCUDO** - Proteger o nó atual (consome 1 escudo do inventário)
7. **Ver mapa** - Visualizar árvore completa
8. **Ver status** - Inventário detalhado, ataque, escudos
9. **Passar turno** - Não fazer nada

**Notas importantes**:
- O jogo mostra apenas as opções de movimento disponíveis
- **Movimento lateral verdadeiro**: permite mover entre nós adjacentes no mesmo nível (ex: estando em H, pode ir para G ou I)
- **Não é circular**: um jogador em G não pode ir para N (pois não são adjacentes)
- **Núcleo-X bloqueado**: só pode ser acessado após dominar AMBAS as chaves!

### Combate

Quando tentar entrar em nó ocupado pelo adversário:
- Se **escudo do nó = 0**: conquista automática
- Se **seu ataque ≥ escudo do nó**: conquista bem-sucedida
- Se **seu ataque < escudo do nó**: movimento bloqueado

**Ataque** = número de espadas no inventário
**Escudo do nó** = escudos colocados pelo jogador que domina o nó

### Recursos

- **Espada** 🗡️ - Aumenta ataque
- **Escudo** 🛡️ - Pode ser colocado em nós para proteção
- **Energia** ⚡ - Recurso extra

**Como coletar**: Recursos são coletados automaticamente ao entrar em nós **[REC]**.

**IMPORTANTE**:
- Cada nó de recurso dá itens **apenas uma vez**
- Após coleta, o nó vira **NORMAL** (recurso consumido)
- Nós mais próximos da raiz (níveis 0-1) dão **2-3 recursos** (espadas/escudos prioritários)
- Nós de nível 2 dão **1-2 recursos**
- Nós de nível 3 (folhas) dão **1 recurso**

### Tipos de Nó

- **NORM** - Nó normal, sem efeitos especiais
- **REC** - Recurso: coleta item aleatório ao entrar
- **CHAVE** - Necessário para vitória (existem 2 no mapa)

### Dicas Estratégicas

1. **Proteja sua rota**: coloque escudos em nós importantes
2. **Colete recursos**: espadas e escudos são essenciais
3. **Planeje ataques**: veja o escudo do nó inimigo antes de atacar
4. **Movimente-se lateralmente**: use nós irmãos para flanquear
5. **Dominação das CHAVES**: priorize encontrar e dominar ambas

### Exemplo de Partida

```
Turno 1 (J1): Mover para PAI → chegar em nó C
Turno 2 (J2): Mover para PAI → chegar em nó F
Turno 1 (J1): Colocar ESCUDO → C agora tem 1 escudo
Turno 2 (J2): Mover para IRMÃO → chegar em nó M
...
```

### Vitória

Para vencer, você precisa:
- ✅ Dominar CHAVE 1
- ✅ Dominar CHAVE 2
- ✅ Estar no Núcleo-X

**OU**

- ⏱️ Se a partida chegar a **100 turnos**: vence quem tiver mais territórios!

### Regras Importantes

1. **Ocupação de território**:
   - Você só domina um nó se ele estiver vazio OU se conquistá-lo em combate
   - Apenas passar por território inimigo protegido NÃO muda a ocupação

2. **Núcleo-X bloqueado**:
   - Só pode acessar após ter AMBAS as chaves

3. **Recursos únicos**:
   - Cada nó de recurso dá itens apenas uma vez
   - Depois vira nó NORMAL

Boa sorte! 🎮
