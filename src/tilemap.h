#ifndef _TILEMAP_H_
#define _TILEMAP_H_

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
} Tilemap;

Tilemap *tilemap_create(Gui *gui);

void tilemap_render(Tilemap *tilemap, Gui *gui, Vector2 camera);

void tilemap_destroy(Tilemap *tilemap);

#endif /* _TILEMAP_RENDERER_H_ */
