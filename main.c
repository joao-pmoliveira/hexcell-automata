#include <stdbool.h>
#include <stdlib.h>
#include "include/raylib.h"
#include "include/raymath.h"


#define IVORY       (Color) {246, 247, 235, 255}
#define ONYX        (Color) {57, 62, 65, 255}
#define CPINK       (Color) {255, 175, 204, 255}
#define SORANGE     (Color) {227, 100, 20, 255}
#define CARMINE     (Color) {154, 3, 30, 255}

static bool use_accent_color = false;
static int palette_index = 1;
//Palettes from Lospect
static const Color palettes[][3] = {
    //Dream-Based Videogame
    { {250,230,205,255}, {151,155,199,255}, {243,192,206,255}, },
    //Bumblebit
    { {39,41,70,255}, {237,160,49,255}, {231,255,238,255}, },
    //1Bit-Styx
    { {44,44,42,255}, {255,243,243,255}, {160,124,255,255}, },
    //Mystique 3
    { {80,42,69,255}, {129,110,160,255}, {135,186,197,255}, },
    //Sangria
    { {33,6,19,255}, {246,48,144,255}, {255,255,245,255}, },
    //Cold Light
    { {8,0,31,255}, {68,77,132,255}, {178,213,209,255}, },
};

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
    
    int palette_timer = 0;

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
        Color background_color = palettes[palette_index][0];
        Color decaying_color = palettes[palette_index][1];
        Color accent_color = use_accent_color ? palettes[palette_index][2] : decaying_color;

        ClearBackground(background_color);


        // draw cells
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLUMNS; col++) {
                int x = col * CELL_SIZE;
                int y = row * CELL_SIZE;

                if (cells[row][col].alive) {
                    // draw alive cell
                    DrawRectangle(x, y,
                                  CELL_SIZE, CELL_SIZE,
                                  accent_color);
                } else {
                    // draw empty cell
                    int alpha = Remap(cells[row][col].health, MIN_HEALTH, MAX_HEALTH, 0, 255);
                    DrawRectangle(x, y,
                                  CELL_SIZE, CELL_SIZE,
                                  (Color){decaying_color.r, decaying_color.g, decaying_color.b, alpha});
                }

            }
        }

        palette_timer += 1;
        if (palette_timer > (30 * 3)) {
            int palettes_count = sizeof(palettes) / sizeof(palettes[0]);
            palette_index++;
            palette_index = palette_index % palettes_count;
            palette_timer = 0;
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
