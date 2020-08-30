#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include "tilemap.h"
#include "guiconst.h"

#define TILE_SIZE 32

Tilemap *tilemap_create(Gui *gui) {
    Tilemap *tilemap = calloc(1, sizeof(Tilemap));
    int win_w;
    int win_h;
    SDL_GetWindowSize(gui->window, &win_w, &win_h);

    tilemap->w = win_w/gui->scale;

    // Allow two tiles of extra screen display to allow for varying resolutions
    int flexibility = 2 * TILE_SIZE;
    if (tilemap->w > SCRW+flexibility) {
        tilemap->w = SCRW+flexibility;
    }
    tilemap->h = win_h/gui->scale;
    if (tilemap->h > SCRH+flexibility) {
        tilemap->h = SCRH+flexibility;
    }
    tilemap->viewport.w = tilemap->w * gui->scale;
    tilemap->viewport.h = tilemap->h * gui->scale;
    tilemap->viewport.x = (win_w-tilemap->viewport.w)/2;
    tilemap->viewport.y = (win_h-tilemap->viewport.h)/2;

    if (NULL == (tilemap->screen = SDL_CreateTexture(
        gui->renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        tilemap->w + 2 * TILE_SIZE, tilemap->h + 2 * TILE_SIZE
    )) ||
        NULL == (tilemap->background = SDL_CreateTexture(
            gui->renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            tilemap->w + 2 * TILE_SIZE, tilemap->h + 2 * TILE_SIZE
        )) ||
        NULL == (tilemap->camera_view = SDL_CreateTexture(
            gui->renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            tilemap->viewport.w,
            tilemap->viewport.h
        ))
    ) {
        fprintf(stderr, "Failed to initialize tilemap: %s\n", SDL_GetError());
        tilemap_destroy(tilemap);
        return NULL;
    }
    fprintf(stderr, "Viewport %d x %d\n", tilemap->viewport.w, tilemap->viewport.h);

    return tilemap;
}

void draw_grid(Tilemap *tilemap, Gui *gui, Vector2 offset, SDL_Texture *texture, int r, int g, int b) {
    int startx = (int)offset.x;
    int starty = (int)offset.y;
    int tiles_x = 2 + tilemap->w/TILE_SIZE;
    int tiles_y = 2 + tilemap->h/TILE_SIZE;

    SDL_SetRenderTarget(gui->renderer, texture);
    SDL_SetRenderDrawColor(gui->renderer, 0,0,0,0);
    SDL_RenderClear(gui->renderer);
    SDL_SetRenderDrawColor(gui->renderer, r,g,b,255);

    for (int x = 0; x < tiles_x; x++) {
        for (int y = 0; y < tiles_y; y++) {
            int c = (startx+starty+x+y)%2;
            if (c) {
                SDL_Rect rect = {
                    .x=x*TILE_SIZE,
                    .y=y*TILE_SIZE,
                    .w=TILE_SIZE-1,
                    .h=TILE_SIZE-1
                };
                SDL_RenderFillRect(gui->renderer, &rect);
            }
        }
    }
    SDL_SetRenderTarget(gui->renderer, tilemap->camera_view);
    double tile_to_world = TILE_SIZE * gui->scale;
    SDL_Rect target_rect = {
        .x = tile_to_world * (floor(offset.x)-offset.x),
        .y = tile_to_world * (floor(offset.y)-offset.y),
        .w = (tilemap->w + 2 * TILE_SIZE) * gui->scale,
        .h = (tilemap->h + 2 * TILE_SIZE) * gui->scale
    };
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(gui->renderer, texture, NULL, &target_rect);

}

void tilemap_render(Tilemap *tilemap, Gui *gui, Vector2 camera) {
    Vector2 parallax = {
        .x = camera.x * 0.8,
        .y = camera.y * 0.8
    };

    SDL_SetRenderTarget(gui->renderer, tilemap->camera_view);
    SDL_SetRenderDrawColor(gui->renderer,0,0,0,0);
    SDL_RenderClear(gui->renderer);

    draw_grid(tilemap, gui, parallax, tilemap->background, 200,0,200);
    draw_grid(tilemap, gui, camera, tilemap->screen, 255, 255, 255);

    SDL_SetRenderTarget(gui->renderer, NULL);
    SDL_RenderCopy(gui->renderer, tilemap->camera_view, NULL, &tilemap->viewport);

}

void tilemap_destroy(Tilemap *tilemap) {
    if (NULL == tilemap) {
        return;
    }
    if (NULL != tilemap->camera_view) {
        SDL_DestroyTexture(tilemap->camera_view);
    }
    if (NULL != tilemap->background) {
        SDL_DestroyTexture(tilemap->background);
    }
    if (NULL != tilemap->screen) {
        SDL_DestroyTexture(tilemap->screen);
    }
    free(tilemap);
}
