#ifndef SRC_TILEMAP_H_
#define SRC_TILEMAP_H_

#define TILE_LAYERS 3
#define TILE_COLUMNS 256
#define TILE_ROWS 256

typedef struct {
    int tiles[TILE_ROWS][TILE_COLUMNS];
    double parallax;    // 1.0 = no parallax, > 1.0 = more foreground, < 1.0 = more background
} Tilelayer;

typedef struct {
    Tilelayer layers[TILE_LAYERS];  // layer 0 = interactive layer
} Tilemap;

Tilemap *tilemap_load(char *filename);

void tilemap_save(Tilemap *tilemap, char *filename);

void tilemap_destroy(Tilemap *tilemap);


#endif /* SRC_TILEMAP_H_ */
