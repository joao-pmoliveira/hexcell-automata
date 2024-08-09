#include <stdbool.h>
#include <stdlib.h>
#include "include/raylib.h"
#include "include/raymath.h"

#define IVORY       (Color) {246, 247, 235, 255}
#define ONYX        (Color) {57, 62, 65, 255}
#define CPINK       (Color) {255, 175, 204, 255}

typedef struct Cell {
    bool alive;
    int health;
} Cell;

int main(void)
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const int CELL_SIZE = 4; 
    const int COLUMNS = SCREEN_WIDTH/CELL_SIZE;
    const int ROWS = SCREEN_HEIGHT/CELL_SIZE;
    const int MAX_HEALTH = 100;
    const int MIN_HEALTH = 0;

    Cell cells[ROWS][COLUMNS];
    Cell state[ROWS][COLUMNS];

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hexcell Automata");
    SetTargetFPS(30);

    // set initial state
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (rand() % 2) {
                cells[row][col].alive = true;
                cells[row][col].health = MAX_HEALTH;
            } else {
                cells[row][col].alive = false;
                cells[row][col].health = MIN_HEALTH;
            }
        }
    }

    while (!WindowShouldClose()) {
        // save current state
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLUMNS; col++) {
                state[row][col].alive = cells[row][col].alive;
                state[row][col].health = cells[row][col].health;
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
                    n += state[row-1][col].alive;

                    if (col-1 >= 0) n += state[row-1][col-1].alive;
                    if (col+1 < COLUMNS) n += state[row-1][col+1].alive;
                }

                if (row+1 < ROWS) {
                    n += state[row+1][col].alive;

                    if (col-1 >= 0) n += state[row+1][col-1].alive;
                    if (col+1 < COLUMNS) n += state[row+1][col+1].alive;
                }

                if (col-1 >= 0) n += state[row][col-1].alive;
                if (col+1 < COLUMNS) n += state[row][col+1].alive;
                
                if (state[row][col].alive) {
                    // cell is alive
                    if (n < 2 || n > 3) {
                        cells[row][col].alive = false;
                        cells[row][col].health = Clamp(cells[row][col].health - 1, MIN_HEALTH, MAX_HEALTH);
                    } else if (n == 2 || n == 3) {
                        cells[row][col].alive = true;
                        cells[row][col].health = MAX_HEALTH;
                    }
                } else {
                    if (n == 3) {
                        cells[row][col].alive = true;
                        cells[row][col].health = MAX_HEALTH;
                    } else {
                        cells[row][col].alive = false;
                        cells[row][col].health = Clamp(cells[row][col].health - 1, MIN_HEALTH, MAX_HEALTH);
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLUE);


        // draw cells
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLUMNS; col++) {
                int x = col * CELL_SIZE;
                int y = row * CELL_SIZE;

                if (cells[row][col].alive) {
                    // draw alive cell
                    DrawRectangle(x, y,
                                  CELL_SIZE, CELL_SIZE,
                                  (Color){246, 247, 235, Remap(cells[row][col].health, MIN_HEALTH, MAX_HEALTH, 0, 255)});
                } else {
                    // draw empty cell
                    DrawRectangle(x, y,
                                  CELL_SIZE, CELL_SIZE,
                                  (Color){246, 247, 235, Remap(cells[row][col].health, MIN_HEALTH, MAX_HEALTH, 0, 255)});
                }

                // draw dividers
                // DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, BLUE);
            }
        }

        DrawFPS(0, SCREEN_HEIGHT - 32);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
