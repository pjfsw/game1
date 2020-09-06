#ifndef _GAME_RENDERER_H_
#define _GAME_RENDERER_H_

#include "vector.h"
#include "gui.h"
#include "sprite.h"
#include "tilemap.h"

#define TILE_SIZE 32

typedef struct {
    SDL_Texture* layers[TILE_LAYERS];
    SDL_Texture *camera_view;
    SDL_Rect viewport;
    int w;
    int h;
    int x_tiles;
    int y_tiles;
} GameRenderer;

GameRenderer *game_renderer_create(Gui *gui);

void game_renderer_render(GameRenderer *game_renderer, Gui *gui, Vector2 *camera,
    Tilemap *tilemap, int sprite_count, Sprite **sprites);

void game_renderer_destroy(GameRenderer *tilemap);

#endif /* _GAME_RENDERER_H_ */
