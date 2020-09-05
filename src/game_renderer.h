#ifndef _GAME_RENDERER_H_
#define _GAME_RENDERER_H_

#include "vector.h"
#include "gui.h"

typedef struct {
    SDL_Texture *background1;
    SDL_Texture *background2;
    SDL_Texture *screen;
    SDL_Texture *camera_view;
    int w;
    int h;
    SDL_Rect viewport;
} GameRenderer;

GameRenderer *game_renderer_create(Gui *gui);

void game_renderer_render(GameRenderer *tilemap, Gui *gui, Vector2 camera);

void game_renderer_destroy(GameRenderer *tilemap);

#endif /* _GAME_RENDERER_H_ */
