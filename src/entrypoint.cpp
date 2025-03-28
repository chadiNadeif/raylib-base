#include "entrypoint.h"
#include "raylib.h"

#include <time.h>

#define RECT_SIZE 8
#define MAX_COL 128
#define MAX_ROW 128
#define WIN_W MAX_COL * RECT_SIZE
#define WIN_H MAX_ROW * RECT_SIZE

bool zombies[MAX_ROW][MAX_COL] = {0};

int CountNeighbors(int y, int x) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) continue;

            int ny = y + dy;
            int nx = x + dx;

            if (ny >= 0 && ny < MAX_ROW && nx >= 0 && nx < MAX_COL) {
                count += zombies[ny][nx];
            }
        }
    }

    return count;
}

void UpdateGrid() {
    bool newZombies[MAX_ROW][MAX_COL] = {0};

    for (int y = 0; y < MAX_ROW; y++) {
        for (int x = 0; x < MAX_COL; x++) {
            int neighbors = CountNeighbors(y, x);

            if (zombies[y][x]) {
                newZombies[y][x] = (neighbors == 2 || neighbors == 3);
            } else {
                newZombies[y][x] = (neighbors == 3);
            }
        }
    }

    for (int y = 0; y < MAX_ROW; y++) {
        for (int x = 0; x < MAX_COL; x++) {
            zombies[y][x] = newZombies[y][x];
        }
    }
}

void raylib_start(void) {
    InitWindow(WIN_W, WIN_H, "Conway's Game of Life");
    SetRandomSeed(time(NULL));

    for (int i = 0; i < MAX_ROW; ++i) {
        for (int z = 0; z < MAX_COL; ++z) {
            zombies[i][z] = GetRandomValue(0, 1);
        }
    }

    while (!WindowShouldClose()) {
        UpdateGrid();

        BeginDrawing();
        ClearBackground(BLACK);
        for (int y = 0; y < MAX_ROW; y++) {
            for (int x = 0; x < MAX_COL; ++x) {
                float offset = (y % 2 == 0) * RECT_SIZE;
                if (x % 2 == 0) {
                    Rectangle get_rekt = CLITERAL(Rectangle){
                        .x = (float)(x * RECT_SIZE + offset),
                        .y = (float)(y * RECT_SIZE),
                        .width = RECT_SIZE,
                        .height = RECT_SIZE
                    };
                    DrawRectangleLinesEx(get_rekt, 4, RED);
                } else {
                    Rectangle get_rekt = CLITERAL(Rectangle){
                        .x = (float)(x * RECT_SIZE - offset),
                        .y = (float)(y * RECT_SIZE),
                        .width = RECT_SIZE,
                        .height = RECT_SIZE
                    };
                    DrawRectangleLinesEx(get_rekt, 4, BLUE);
                }

                if (zombies[y][x]) {
                    DrawRectangle(x * RECT_SIZE, y * RECT_SIZE, RECT_SIZE, RECT_SIZE, WHITE);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
}
