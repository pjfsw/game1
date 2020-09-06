#include <stdio.h>
#include <stdlib.h>

#include "tilemap.h"

#define LAYER_ID -1
#define PARALLAX_SCALE 1000.0

Tilemap *tilemap_load(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }
    Tilemap *tilemap = calloc(1, sizeof(Tilemap));
    int layer = 0;

    tilemap->layers[0].parallax = 1.0;

    while (!feof(f)) {
        int row;
        int col;
        int tile;

        if (3 != fscanf(f, "%04x %04x %04x\n", &row, &col, &tile)) {
            continue;
        }
        if (row == LAYER_ID && col >= 0 && col < TILE_LAYERS) {
            layer = col;
            tilemap->layers[layer].parallax = (double)tile/PARALLAX_SCALE;
        } else if (col >= 0 && col < TILE_COLUMNS && row >= 0 && row < TILE_ROWS) {
            tilemap->layers[layer].tiles[row][col] = tile;
        }
    }
    fclose(f);

    return tilemap;
}

void tilemap_save(Tilemap *tilemap, char *filename) {
    FILE *f = fopen(filename, "w");
    if (f  == NULL) {
        fprintf(stderr, "Failed to save tilemap with filename %s\n", filename);
        return;
    }
    for (int i = 0; i < TILE_LAYERS; i++) {
        fprintf(f, "%04x %04x %04x\n", LAYER_ID, i, (int)(tilemap->layers[i].parallax * PARALLAX_SCALE));
        for (int row = 0; row < TILE_ROWS; row++) {
            for (int col = 0; col < TILE_COLUMNS; col++) {
                int tile = tilemap->layers[i].tiles[col][row];
                if (tile != 0) {
                    fprintf(f, "%04x %04x %04x\n", row, col, tile);
                }
            }
        }
    }
    fclose(f);
}

void tilemap_destroy(Tilemap *tilemap) {
    if (NULL == tilemap) {
        return;
    }
    free(tilemap);
}
