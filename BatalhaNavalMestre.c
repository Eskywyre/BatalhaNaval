// BatalhaNaval_Mestre.c
// Nível Mestre: tabuleiro 10x10 + habilidades (cone, cruz, octaedro) sobrepostas ao tabuleiro.
// Minha ideia: gerar as 3 matrizes (0/1) com loops/ifs e "carimbar" no tabuleiro


#include <stdio.h>

#define BOARD_SIZE 10
#define SHIP_SIZE  3
#define WATER      0
#define SHIP_MARK  3
#define SKILL_MARK 5

typedef enum { HORIZONTAL = 0, VERTICAL = 1 } Orientation;

/* --- Base do tabuleiro e navios (reaproveito a lógica dos níveis anteriores) --- */

void initBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            board[r][c] = WATER;
}

int canPlaceShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, Orientation o) {
    for (int i = 0; i < SHIP_SIZE; i++) {
        int r = row + (o == VERTICAL ? i : 0);
        int c = col + (o == HORIZONTAL ? i : 0);
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) return 0;
        if (board[r][c] != WATER) return 0;
    }
    return 1;
}

void placeShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, Orientation o) {
    for (int i = 0; i < SHIP_SIZE; i++) {
        int r = row + (o == VERTICAL ? i : 0);
        int c = col + (o == HORIZONTAL ? i : 0);
        board[r][c] = SHIP_MARK;
    }
}

/* --- Construção dinâmica das matrizes de habilidade (0/1) --- */
/* Usei tamanho fixo 5x5 para ficar visual mas a lógica tb funciona para ímpar >=3. */

#define SKILL_N 5

void buildCone(int m[SKILL_N][SKILL_N]) {
    // Cone "apontando para baixo": topo é a ponta, cada linha alarga 1 pra cada lado.
    int mid = SKILL_N / 2; // coluna central
    for (int r = 0; r < SKILL_N; r++) {
        for (int c = 0; c < SKILL_N; c++) {
            // distância horizontal até o centro tem que caber na "largura" do cone (r)
            int dx = c - mid;
            m[r][c] = (dx >= -r && dx <= r) ? 1 : 0;
        }
    }
}

void buildCross(int m[SKILL_N][SKILL_N]) {
    int mid = SKILL_N / 2;
    for (int r = 0; r < SKILL_N; r++) {
        for (int c = 0; c < SKILL_N; c++) {
            // marca a linha central e a coluna central
            m[r][c] = (r == mid || c == mid) ? 1 : 0;
        }
    }
}

void buildOctahedron(int m[SKILL_N][SKILL_N]) {
    // “Octaedro de frente” vira um losango (diamante): |dr| + |dc| <= raio
    int mid = SKILL_N / 2;
    for (int r = 0; r < SKILL_N; r++) {
        for (int c = 0; c < SKILL_N; c++) {
            int dr = r - mid;
            int dc = c - mid;
            m[r][c] = ((dr < 0 ? -dr : dr) + (dc < 0 ? -dc : dc) <= mid) ? 1 : 0;
        }
    }
}

/* --- Sobreposição das habilidades no tabuleiro --- */
/* Obs importante: para CONE, a "origem" é a PONTA (topo do cone), então alinhei a primeira linha da matriz nesse ponto.
   Para CRUZ e OCTAEDRO, a origem é o CENTRO da matriz. Isso bate com a descrição do enunciado. */

void overlayCone(int board[BOARD_SIZE][BOARD_SIZE], int skill[SKILL_N][SKILL_N], int apexRow, int apexCol) {
    int mid = SKILL_N / 2;
    for (int r = 0; r < SKILL_N; r++) {
        for (int c = 0; c < SKILL_N; c++) {
            if (skill[r][c] == 0) continue;
            int br = apexRow + r;       // topo da matriz fica na origem
            int bc = apexCol + (c - mid);
            if (br < 0 || br >= BOARD_SIZE || bc < 0 || bc >= BOARD_SIZE) continue;
            if (board[br][bc] != SHIP_MARK) board[br][bc] = SKILL_MARK; // não apago navio
        }
    }
}

void overlayCentered(int board[BOARD_SIZE][BOARD_SIZE], int skill[SKILL_N][SKILL_N], int centerRow, int centerCol) {
    int mid = SKILL_N / 2;
    for (int r = 0; r < SKILL_N; r++) {
        for (int c = 0; c < SKILL_N; c++) {
            if (skill[r][c] == 0) continue;
            int br = centerRow + (r - mid);
            int bc = centerCol + (c - mid);
            if (br < 0 || br >= BOARD_SIZE || bc < 0 || bc >= BOARD_SIZE) continue;
            if (board[br][bc] != SHIP_MARK) board[br][bc] = SKILL_MARK;
        }
    }
}

/* --- Impressão --- */

void printBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    // Cabeçalho
    printf("    ");
    for (int c = 0; c < BOARD_SIZE; c++) printf("%d ", c);
    printf("\n   ");
    for (int c = 0; c < BOARD_SIZE; c++) printf("--");
    printf("-\n");

    for (int r = 0; r < BOARD_SIZE; r++) {
        printf("%2d | ", r);
        for (int c = 0; c < BOARD_SIZE; c++) {
            // Visual numérico como pedido (0 agua, 3 navio, 5 skill)
            printf("%d ", board[r][c]);
        }
        printf("\n");
    }
}

int main(void) {
    int board[BOARD_SIZE][BOARD_SIZE];
    initBoard(board);

    // Posicionei dois navios simples só para dar contexto visual (como nos níveis anteriores).
    int hRow = 2, hCol = 1; // horizontal: (2,1)-(2,3)
    int vRow = 6, vCol = 7; // vertical:   (6,7)-(8,7)

    if (!canPlaceShip(board, hRow, hCol, HORIZONTAL)) { printf("Falha navio horizontal.\n"); return 1; }
    placeShip(board, hRow, hCol, HORIZONTAL);

    if (!canPlaceShip(board, vRow, vCol, VERTICAL))   { printf("Falha navio vertical.\n"); return 1; }
    placeShip(board, vRow, vCol, VERTICAL);

    // Construo as 3 habilidades (5x5) com loops/ifs
    int cone[SKILL_N][SKILL_N], cross[SKILL_N][SKILL_N], octa[SKILL_N][SKILL_N];
    buildCone(cone);
    buildCross(cross);
    buildOctahedron(octa);

    // Origem das habilidades no tabuleiro (fixo no código, como pede o nível):
    // CONE: origem é a PONTA; CRUZ/ OCTAEDRO: origem é o CENTRO.
    int coneApexRow = 0, coneApexCol = 5;     // ponta no topo, descendo
    int crossCenterRow = 4, crossCenterCol = 3;
    int octaCenterRow  = 7, octaCenterCol  = 2;

    // Sobreposição respeitando limites e sem apagar navio
    overlayCone(board,   cone, coneApexRow, coneApexCol);
    overlayCentered(board, cross, crossCenterRow, crossCenterCol);
    overlayCentered(board,  octa,  octaCenterRow,  octaCenterCol);

    printf("\nLegenda: 0=agua  3=navio  5=area de habilidade\n\n");
    printBoard(board);
    return 0;
}
