#include <stdlib.h>
#include "include/raylib.h"

#define IVORY       (Color) {246, 247, 235, 255}
#define ONYX        (Color) {57, 62, 65, 255}
#define CPINK       (Color) {255, 175, 204, 255}

int main(void)
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const int CELL_SIZE = 8; 
    const int COLUMNS = SCREEN_WIDTH/CELL_SIZE;
    const int ROWS = SCREEN_HEIGHT/CELL_SIZE;
    const int MAX_VALUE = 100;

    int cells[ROWS][COLUMNS];
    int state[ROWS][COLUMNS];

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hexcell Automata");
    SetTargetFPS(30);

    // set initial state
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            cells[row][col] = 0;
            cells[row][col] = (rand() % 2);
        }
    }

    while (!WindowShouldClose()) {
        // save current state
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLUMNS; col++) {
                state[row][col] = cells[row][col];
            }
        }
        
        // calculate new state
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLUMNS; col++) {
                // Conway Rules
                // alive && n<2 : cell dies
                // alive && n>3 : cell dies
                // alive && n=2 : cell lives
                // alive && n=3 : cell lives
                // dead && n=3 : cell reborn
                
                int n = 0;

                if (row-1 >= 0) {
                    n += state[row-1][col];

                    if (col-1 >= 0) n += state[row-1][col-1];
                    if (col+1 < COLUMNS) n += state[row-1][col+1];
                }

                if (row+1 < ROWS) {
                    n += state[row+1][col];

                    if (col-1 >= 0) n += state[row+1][col-1];
                    if (col+1 < COLUMNS) n += state[row+1][col+1];
                }

                if (col-1 >= 0) n += state[row][col-1];
                if (col+1 < COLUMNS) n += state[row][col+1];
                
                if (state[row][col]) {
                    // cell is alive
                    if (n < 2 || n > 3) cells[row][col] = 0;
                    else if (n == 2 || n == 3) cells[row][col] = 1;
                } else {
                    if (n == 3) cells[row][col] = 1;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // draw cells
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLUMNS; col++) {
                int x = col * CELL_SIZE;
                int y = row * CELL_SIZE;

                if (cells[row][col]) {
                    // draw alive cell
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, IVORY);
                } else {
                    // draw empty cell
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, BLUE);
                }

                // draw dividers
                DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, BLUE);
            }
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
