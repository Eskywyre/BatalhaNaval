// BatalhaNaval_Aventureiro.c
// Nível Aventureiro: tabuleiro 10x10, 4 navios (2 ortogonais e 2 diagonais)

#include <stdio.h>

#define BOARD_SIZE 10
#define SHIP_SIZE  3
#define WATER      0
#define SHIP_MARK  3

// Usei quatro orientações: horizontal, vertical, diagonal ↘ (DR) e diagonal ↗ (UR).
typedef enum { HORIZONTAL = 0, VERTICAL = 1, DIAG_DR = 2, DIAG_UR = 3 } Orientation;

void initBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            board[r][c] = WATER;
        }
    }
}

// Cálculo do próximo par (r,c) conforme a orientação
static void step(int baseR, int baseC, int i, Orientation o, int *outR, int *outC) {
    int r = baseR, c = baseC;
    if (o == HORIZONTAL) c += i;
    else if (o == VERTICAL) r += i;
    else if (o == DIAG_DR) { r += i; c += i; }   // ↘
    else /* DIAG_UR */     { r -= i; c += i; }   // ↗
    *outR = r; *outC = c;
}

// Verifico se cabe e se não há sobreposição
int canPlaceShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, Orientation o) {
    for (int i = 0; i < SHIP_SIZE; i++) {
        int r, c;
        step(row, col, i, o, &r, &c);

        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) return 0;
        if (board[r][c] != WATER) return 0;
    }
    return 1;
}

// Posiciono o navio já validado
void placeShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, Orientation o) {
    for (int i = 0; i < SHIP_SIZE; i++) {
        int r, c;
        step(row, col, i, o, &r, &c);
        board[r][c] = SHIP_MARK;
    }
}

// Imprimo o tabuleiro de forma enxuta e legível
void printBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    printf("    ");
    for (int c = 0; c < BOARD_SIZE; c++) printf("%d ", c);
    printf("\n   ");
    for (int c = 0; c < BOARD_SIZE; c++) printf("--");
    printf("-\n");

    for (int r = 0; r < BOARD_SIZE; r++) {
        printf("%2d | ", r);
        for (int c = 0; c < BOARD_SIZE; c++) {
            printf("%d ", board[r][c]);
        }
        printf("\n");
    }
}

int main(void) {
    int board[BOARD_SIZE][BOARD_SIZE];
    initBoard(board);

    // Escolhi coordenadas fixas, sem entrada do usuário (como pede o desafio).
    // 2 ortogonais:
    int hRow = 0, hCol = 0;                 // HORIZONTAL: (0,0) (0,1) (0,2)
    int vRow = 2, vCol = 5;                 // VERTICAL:   (2,5) (3,5) (4,5)

    // 2 diagonais:
    int d1Row = 6, d1Col = 1;               // DIAG_DR ↘:  (6,1) (7,2) (8,3)
    int d2Row = 9, d2Col = 7;               // DIAG_UR ↗:  (9,7) (8,8) (7,9)

    // Valido e posiciono um a um. Se algum falhar, aviso e saio.
    if (!canPlaceShip(board, hRow, hCol, HORIZONTAL)) { printf("Falha no navio HORIZONTAL.\n"); return 1; }
    placeShip(board, hRow, hCol, HORIZONTAL);

    if (!canPlaceShip(board, vRow, vCol, VERTICAL))   { printf("Falha no navio VERTICAL.\n"); return 1; }
    placeShip(board, vRow, vCol, VERTICAL);

    if (!canPlaceShip(board, d1Row, d1Col, DIAG_DR))  { printf("Falha no navio DIAG_DR.\n"); return 1; }
    placeShip(board, d1Row, d1Col, DIAG_DR);

    if (!canPlaceShip(board, d2Row, d2Col, DIAG_UR))  { printf("Falha no navio DIAG_UR.\n"); return 1; }
    placeShip(board, d2Row, d2Col, DIAG_UR);

    printf("\nTabuleiro (0 = agua, 3 = navio):\n\n");
    printBoard(board);
    return 0;
}
