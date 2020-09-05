#ifndef _GAME_RENDERER_H_
#define _GAME_RENDERER_H_

#include "vector.h"
#include "gui.h"
#include "sprite.h"

typedef struct {
    SDL_Texture *background1;
    SDL_Texture *background2;
    SDL_Texture *screen;
    SDL_Texture *camera_view;
    SDL_Rect viewport;
    int w;
    int h;
    int x_tiles;
    int y_tiles;
} GameRenderer;

GameRenderer *game_renderer_create(Gui *gui);

void game_renderer_render(GameRenderer *game_renderer, Gui *gui, Vector2 camera,
    int sprite_count, Sprite **sprites);

void game_renderer_destroy(GameRenderer *tilemap);

#endif /* _GAME_RENDERER_H_ */
