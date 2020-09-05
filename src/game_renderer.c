#include "game_renderer.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include "guiconst.h"

#define TILE_SIZE 32

GameRenderer *game_renderer_create(Gui *gui) {
    GameRenderer *renderer = calloc(1, sizeof(GameRenderer));
    int win_w;
    int win_h;
    SDL_GetWindowSize(gui->window, &win_w, &win_h);

    renderer->w = win_w/gui->scale;

    // Allow two tiles of extra screen display to allow for varying resolutions
    int flexibility = 2 * TILE_SIZE;
    if (renderer->w > SCRW+flexibility) {
        renderer->w = SCRW+flexibility;
    }
    renderer->h = win_h/gui->scale;
    if (renderer->h > SCRH+flexibility) {
        renderer->h = SCRH+flexibility;
    }
    renderer->viewport.w = renderer->w * gui->scale;
    renderer->viewport.h = renderer->h * gui->scale;
    renderer->viewport.x = (win_w-renderer->viewport.w)/2;
    renderer->viewport.y = (win_h-renderer->viewport.h)/2;

    if (NULL == (renderer->screen = SDL_CreateTexture(
        gui->renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        renderer->w + 2 * TILE_SIZE, renderer->h + 2 * TILE_SIZE
    )) ||
        NULL == (renderer->background1 = SDL_CreateTexture(
            gui->renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            renderer->w + 2 * TILE_SIZE, renderer->h + 2 * TILE_SIZE
        )) ||
        NULL == (renderer->background2 = SDL_CreateTexture(
            gui->renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            renderer->w + 2 * TILE_SIZE, renderer->h + 2 * TILE_SIZE
        )) ||
        NULL == (renderer->camera_view = SDL_CreateTexture(
            gui->renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            renderer->viewport.w,
            renderer->viewport.h
        ))
    ) {
        fprintf(stderr, "Failed to initialize game renderer: %s\n", SDL_GetError());
        game_renderer_destroy(renderer);
        return NULL;
    }
    fprintf(stderr, "Viewport %d x %d\n", renderer->viewport.w, renderer->viewport.h);

    return renderer;
}

void draw_grid(GameRenderer *game_renderer, Gui *gui, Vector2 offset, SDL_Texture *texture, int r, int g, int b) {
    int startx = (int)offset.x;
    int starty = (int)offset.y;
    int tiles_x = 2 + game_renderer->w/TILE_SIZE;
    int tiles_y = 2 + game_renderer->h/TILE_SIZE;

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
    SDL_SetRenderTarget(gui->renderer, game_renderer->camera_view);
    double tile_to_world = TILE_SIZE * gui->scale;
    SDL_Rect target_rect = {
        .x = tile_to_world * (floor(offset.x)-offset.x),
        .y = tile_to_world * (floor(offset.y)-offset.y),
        .w = (game_renderer->w + 2 * TILE_SIZE) * gui->scale,
        .h = (game_renderer->h + 2 * TILE_SIZE) * gui->scale
    };
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(gui->renderer, texture, NULL, &target_rect);

}

void game_renderer_render(GameRenderer *game_renderer, Gui *gui, Vector2 camera) {
    Vector2 parallax1 = {
        .x = camera.x * 0.9,
        .y = camera.y * 0.9
    };
    Vector2 parallax2 = {
        .x = camera.x * 0.3,
        .y = camera.y * 0.3,
    };

    SDL_SetRenderTarget(gui->renderer, game_renderer->camera_view);
    SDL_SetRenderDrawColor(gui->renderer,0,0,0,0);
    SDL_RenderClear(gui->renderer);

    draw_grid(game_renderer, gui, parallax2, game_renderer->background2, 30,0,30);
    draw_grid(game_renderer, gui, parallax1, game_renderer->background1, 90,30,80);
    draw_grid(game_renderer, gui, camera, game_renderer->screen, 120, 60, 100);

    SDL_SetRenderTarget(gui->renderer, NULL);
    SDL_RenderCopy(gui->renderer, game_renderer->camera_view, NULL, &game_renderer->viewport);

}

void game_renderer_destroy(GameRenderer *renderer) {
    if (NULL == renderer) {
        return;
    }
    if (NULL != renderer->camera_view) {
        SDL_DestroyTexture(renderer->camera_view);
    }
    if (NULL != renderer->background1) {
        SDL_DestroyTexture(renderer->background1);
    }
    if (NULL != renderer->background2) {
        SDL_DestroyTexture(renderer->background2);
    }
    if (NULL != renderer->screen) {
        SDL_DestroyTexture(renderer->screen);
    }
    free(renderer);
}
