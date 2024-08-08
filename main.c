#include <stdlib.h>
#include "include/raylib.h"

#define IVORY       (Color) {246, 247, 235, 255}
#define ONYX        (Color) {57, 62, 65, 255}

int main(void)
{
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 360;
    const int CELL_SIZE = 16;
    const int ROWS = SCREEN_WIDTH / CELL_SIZE;
    const int COLUMNS = SCREEN_HEIGHT / CELL_SIZE;
    const int MAX_VALUE = 1;

    int cells[COLUMNS][ROWS];
    int state[COLUMNS][ROWS];

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hexcell Automata");
    SetTargetFPS(5);

    // set initial state
    for (int c = 0; c < COLUMNS; c++) {
        for (int r = 0; r < ROWS; r++) {
            cells[c][r] = rand() % (MAX_VALUE + 1);
        }
    }

    while (!WindowShouldClose()) {
        
        // save current state
        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                state[c][r] = cells[c][r];
            }
        }

        // calculate new state

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                // CONWAY'S RULES
                // Alive & n<2 -> cell dies
                // Alive & (n=2 | n=3) -> cell lives
                // Alive & n>3 -> cell dies
                // Dead & n=3 -> new alive cell

                int n = 0;

                if (c-1 >= 0) {
                    n += state[c-1][r];

                    if (r-1 >= 0) n += state[c-1][r-1];
                    if (r+1 < ROWS) n += state[c-1][r+1];
                }

                if (c+1 < COLUMNS) {
                    n += state[c+1][r];

                    if (r-1 >= 0) n += state[c+1][r-1];
                    if (r+1 < ROWS) n += state[c+1][r+1];
                }

                if (r-1 >= 0) n += state[c][r-1];
                if (r+1 < ROWS) n += state[c][r+1];

                if (state[c][r]) {
                    if (n < 2 || n > 3) cells[c][r] = 0;
                    if (n == 2 || n == 3) cells[c][r] = 1;
                } else if (n == 3) cells [c][r] = 1;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // draw cells
        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                int x = r * CELL_SIZE;
                int y = c * CELL_SIZE;

                if (cells[c][r]) {
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, ONYX);
                } else {
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, IVORY);
                }

                DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, IVORY);

            }
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
