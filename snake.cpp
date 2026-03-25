#include <iostream>
#include <deque>
#include <string>
#include <cstdlib>
#include <ctime>

// ─── Compatibilidade Windows / Linux ─────────────────────────────────────────
#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
  #define CLEAR "cls"
  #define SLEEP(ms) Sleep(ms)
#else
  #include <termios.h>
  #include <unistd.h>
  #include <fcntl.h>
  #define CLEAR "clear"
  #define SLEEP(ms) usleep((ms) * 1000)

  // kbhit e getch não existem no Linux — implementamos manualmente
  int kbhit() {
      struct termios oldt, newt;
      int oldf, ch;
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
      fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
      ch = getchar();
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      fcntl(STDIN_FILENO, F_SETFL, oldf);
      if (ch != EOF) { ungetc(ch, stdin); return 1; }
      return 0;
  }

  int getch() {
      struct termios oldt, newt;
      int ch;
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      ch = getchar();
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      return ch;
  }
#endif
// ─────────────────────────────────────────────────────────────────────────────

const int W = 30; // largura do campo
const int H = 20; // altura do campo

struct Pos { int x, y; };
enum  Dir { UP, DOWN, LEFT, RIGHT };

// Gera comida em posição aleatória
Pos novaComida() {
    return { rand() % W, rand() % H };
}

// Verifica se uma posição está no corpo da cobra
bool naoCobra(const std::deque<Pos>& snake, int x, int y) {
    for (const auto& p : snake)
        if (p.x == x && p.y == y) return true;
    return false;
}

void renderizar(const std::deque<Pos>& snake, const Pos& food, int score) {
    system(CLEAR);

    // Borda superior
    std::cout << "+" << std::string(W, '-') << "+\n";

    for (int y = 0; y < H; y++) {
        std::cout << '|';
        for (int x = 0; x < W; x++) {
            // Cabeça da cobra
            if (snake.front().x == x && snake.front().y == y)
                std::cout << '@';
            // Corpo da cobra
            else if (naoCobra(snake, x, y))
                std::cout << 'o';
            // Comida
            else if (food.x == x && food.y == y)
                std::cout << '*';
            // Vazio
            else
                std::cout << ' ';
        }
        std::cout << "|\n";
    }

    // Borda inferior
    std::cout << "+" << std::string(W, '-') << "+\n";
    std::cout << " Score: " << score
              << "   |   WASD = mover   |   Q = sair\n";
}

int main() {
    srand(time(0));

    // Cobra começa com 3 segmentos no centro
    std::deque<Pos> snake = {
        { W/2,     H/2 },
        { W/2 - 1, H/2 },
        { W/2 - 2, H/2 }
    };

    Dir dir   = RIGHT;
    Pos food  = novaComida();
    int score = 0;

    while (true) {
        // ── Leitura de input (não bloqueia o jogo) ────────────────────────
        if (kbhit()) {
            char c = (char)getch();
            switch (c) {
                case 'w': case 'W': if (dir != DOWN)  dir = UP;    break;
                case 's': case 'S': if (dir != UP)    dir = DOWN;  break;
                case 'a': case 'A': if (dir != RIGHT) dir = LEFT;  break;
                case 'd': case 'D': if (dir != LEFT)  dir = RIGHT; break;
                case 'q': case 'Q':
                    std::cout << "\nVocê saiu. Score: " << score << "\n";
                    return 0;
            }
        }

        // ── Calcula nova posição da cabeça ────────────────────────────────
        Pos cabeca = snake.front();
        if      (dir == UP)    cabeca.y--;
        else if (dir == DOWN)  cabeca.y++;
        else if (dir == LEFT)  cabeca.x--;
        else if (dir == RIGHT) cabeca.x++;

        // ── Colisão com parede ────────────────────────────────────────────
        if (cabeca.x < 0 || cabeca.x >= W || cabeca.y < 0 || cabeca.y >= H)
            break;

        // ── Colisão consigo mesmo ─────────────────────────────────────────
        if (naoCobra(snake, cabeca.x, cabeca.y))
            break;

        // ── Move a cobra ──────────────────────────────────────────────────
        snake.push_front(cabeca);

        if (cabeca.x == food.x && cabeca.y == food.y) {
            // Comeu: cresce e gera nova comida
            score++;
            food = novaComida();
        } else {
            // Não comeu: remove o rabo (mantém tamanho)
            snake.pop_back();
        }

        // ── Desenha ───────────────────────────────────────────────────────
        renderizar(snake, food, score);

        // Velocidade: reduz delay conforme score aumenta (fica mais rápido)
        int delay = 150 - (score * 3);
        if (delay < 60) delay = 60;
        SLEEP(delay);
    }

    // Game over
    system(CLEAR);
    std::cout << "\n";
    std::cout << "  ╔══════════════════════╗\n";
    std::cout << "  ║      GAME  OVER      ║\n";
    std::cout << "  ╚══════════════════════╝\n\n";
    std::cout << "  Pontuação final: " << score << "\n\n";
    std::cout << "  Para jogar novamente, execute o programa outra vez.\n\n";

    
    return 0;
}