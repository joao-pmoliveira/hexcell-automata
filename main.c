#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>


int main(void)
{
    const int ROWS = 10;
    const int COLUMNS = 10;
    const int MAX_VALUE = 1;

    int cells[ROWS][COLUMNS];
    memset(cells, 0, sizeof cells);

    int state[ROWS][COLUMNS];


    // Initial state

    for (int c = 0; c < COLUMNS; c++) {
        for (int r = 0; r < ROWS; r++) {
            cells[c][r] = rand() % (MAX_VALUE + 1);
        }
    }

    while(1) {
        // save current cells' state

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                state[c][r] = cells[c][r];
            }
        }

        // Calculate new cell state

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                int n = 0;

                // Conway rules
                // alive & n<2 = dies
                // alive & (n=2 || n=3) = lives
                // alive & n>3 = dies
                // dead & n=3 = born 

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


                /* is alive */
                if (state[c][r]) {
                    if (n<2 || n>3) cells[c][r] = 0;
                    if (n == 2 || n == 3) cells[c][r] = 1;
                } else if (n == 3) cells[c][r] = 1;

            }
        }

        // Display
        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                if (cells[c][r]) {
                    printf("%s", "|0");
                } else {
                    printf("%s", "| ");
                }
            }
            printf("|\n");
        }

        Sleep(500);
        system("cls");
    }

    return 0;
}
