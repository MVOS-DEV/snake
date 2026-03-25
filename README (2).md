# 🐍 Snake — Jogo ASCII no Terminal

Um clássico jogo Snake feito inteiramente com a **biblioteca padrão do C++**, rodando no terminal com gráficos ASCII. Sem dependências externas, sem frameworks — apenas C++ puro.

---

## 📋 Índice

- [Sobre](#sobre)
- [Gameplay](#gameplay)
- [Requisitos](#requisitos)
- [Instalação](#instalação)
- [Como Executar](#como-executar)
- [Controles](#controles)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Como Funciona](#como-funciona)
- [Licença](#licença)

---

## Sobre

Este projeto foi criado como exercício de estudo para demonstrar como um jogo 2D pode ser construído usando apenas C++ e renderização no terminal — sem bibliotecas gráficas, sem dependências externas.

Ele aborda conceitos fundamentais de desenvolvimento de jogos como:
- Loop de jogo
- Leitura de input sem bloqueio
- Detecção de colisão
- Gerenciamento de estado
- Dificuldade dinâmica

---

## Gameplay

```
+------------------------------+
|                              |
|        @ooo                  |
|           o                  |
|           o    *             |
|                              |
|                              |
+------------------------------+
 Score: 3   |   WASD = mover   |   Q = sair
```

- `@` — Cabeça da cobra  
- `o` — Corpo da cobra  
- `*` — Comida  

Coma a comida para crescer. O jogo fica mais rápido conforme o score aumenta. Evite as paredes e o próprio corpo.

---

## Requisitos

| Ferramenta | Versão | Observação |
|------------|--------|------------|
| Compilador C++ | C++11 ou superior | g++, clang++ ou MSVC |
| Terminal | Qualquer | CMD, PowerShell, Bash, Zsh |

Nenhuma biblioteca externa é necessária.

---

## Instalação

### 1. Clone o repositório

```bash
git clone https://github.com/seu-usuario/snake-ascii.git
cd snake-ascii
```

Ou simplesmente baixe o arquivo `snake.cpp` diretamente.

### 2. Compile

**Linux / macOS:**
```bash
g++ snake.cpp -o snake
```

**Windows (MinGW):**
```cmd
g++ snake.cpp -o snake.exe
```

**Windows (MSVC):**
```cmd
cl snake.cpp /Fe:snake.exe
```

---

## Como Executar

**Linux / macOS:**
```bash
./snake
```

**Windows:**
```cmd
snake.exe
```

> 💡 **Dica (Windows):** Abra o Explorador de Arquivos, navegue até a pasta onde está o `snake.cpp`, clique na barra de endereço, digite `cmd` e pressione Enter. O terminal já abrirá direto na pasta certa.

---

## Controles

| Tecla | Ação |
|-------|------|
| `W` | Mover para cima |
| `S` | Mover para baixo |
| `A` | Mover para esquerda |
| `D` | Mover para direita |
| `Q` | Sair |

---

## Estrutura do Projeto

```
snake-ascii/
│
├── snake.cpp       # Código-fonte completo do jogo (arquivo único)
└── README.md       # Este arquivo
```

O jogo inteiro está contido em um único arquivo `.cpp`, mantido simples intencionalmente para facilitar a leitura e o aprendizado.

---

## Como Funciona

### Loop do Jogo

Cada frame segue esta sequência:

```
Ler Input → Atualizar Lógica → Renderizar → Aguardar (delay)
```

### Movimento da Cobra

A cobra é representada como uma `std::deque<Pos>` — uma fila dupla de posições.

- A cada frame, uma nova cabeça é inserida na frente
- O rabo é removido do final
- Se a comida for comida, o rabo **não** é removido — fazendo a cobra crescer

```cpp
snake.push_front(novaCabeca);       // sempre avança
if (!comeuComida) snake.pop_back(); // só remove o rabo se não comeu
```

### Detecção de Colisão

Duas verificações por frame:
1. **Colisão com parede** — se a cabeça saiu dos limites do campo
2. **Colisão consigo mesmo** — se a cabeça sobrepõe algum segmento do corpo

### Dificuldade Dinâmica

O delay entre os frames diminui conforme o score aumenta:

```cpp
int delay = 150 - (score * 3);
if (delay < 60) delay = 60; // velocidade máxima limitada
```

### Compatibilidade entre Sistemas

A leitura de input sem bloqueio (`kbhit` + `getch`) é tratada de forma diferente por sistema operacional:
- **Windows** — usa `<conio.h>` nativamente
- **Linux / macOS** — implementação própria usando `termios` e `fcntl`

---

## Licença

Este projeto está licenciado sob a **Licença MIT** — sinta-se livre para usar, modificar e distribuir.

---

<p align="center">Feito com C++ e ❤️</p>
