#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 10    // Tamanho do tabuleiro 10x10
#define SHIP_SIZE  3     // Cada navio ocupa 3 posições
#define WATER      0     // Representa água
#define SHIP_MARK  3     // Representa navio

typedef enum { HORIZONTAL = 0, VERTICAL = 1 } Orientation;

/*
   A ideia principal:
   - O tabuleiro é uma matriz onde 0 representa água e 3 representa navio.
   - Antes de posicionar um navio, verifico se ele cabe e se não há sobreposição.
   - Usei duas funções auxiliares pequenas para deixar o código mais organizado.
*/

// Inicializa o tabuleiro preenchendo tudo com 0 (água)
void initBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            board[r][c] = WATER;
        }
    }
}

// Verifica se é possível colocar o navio começando em (row, col)
int canPlaceShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, Orientation o) {
    for (int i = 0; i < SHIP_SIZE; i++) {
        int r = row + (o == VERTICAL ? i : 0);
        int c = col + (o == HORIZONTAL ? i : 0);

        // Saiu do tabuleiro ou encontrou outro navio
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != WATER) {
            return 0;
        }
    }
    return 1;
}

// Posiciona o navio na matriz, assumindo que já foi validado
void placeShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, Orientation o) {
    int ship[SHIP_SIZE] = {SHIP_MARK, SHIP_MARK, SHIP_MARK};
    for (int i = 0; i < SHIP_SIZE; i++) {
        int r = row + (o == VERTICAL ? i : 0);
        int c = col + (o == HORIZONTAL ? i : 0);
        board[r][c] = ship[i];
    }
}

// Exibe o tabuleiro completo no console
void printBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    printf("    ");
    for (int c = 0; c < BOARD_SIZE; c++) printf("%d ", c);
    printf("\n");

    printf("   ");
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

    // Coordenadas iniciais dos navios (definidas direto no código)
    int hRow = 2, hCol = 1; // Navio horizontal (2,1) até (2,3)
    int vRow = 5, vCol = 7; // Navio vertical (5,7) até (7,7)

    if (!canPlaceShip(board, hRow, hCol, HORIZONTAL)) {
        printf("Erro: navio horizontal nao cabe ou sobrepoe outro.\n");
        return 1;
    }
    placeShip(board, hRow, hCol, HORIZONTAL);

    if (!canPlaceShip(board, vRow, vCol, VERTICAL)) {
        printf("Erro: navio vertical nao cabe ou sobrepoe outro.\n");
        return 1;
    }
    placeShip(board, vRow, vCol, VERTICAL);

    printf("\nTabuleiro final (0 = agua, 3 = navio):\n\n");
    printBoard(board);

    return 0;
}