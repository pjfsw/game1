#include <SDL2/SDL.h>
#include <unistd.h>
#include <stdio.h>
#include "gui.h"
#include "game.h"
#include "guiconst.h"
#include "deltatime.h"
#include "input.h"

void destroy_gui(Gui *gui) {
    if (NULL == gui) {
        return;
    }
    if (NULL != gui->renderer) {
        SDL_DestroyRenderer(gui->renderer);
    }
    if (NULL != gui->window) {
        SDL_DestroyWindow(gui->window);
    }
    SDL_Quit();
    free(gui);
}

Gui *create_gui(int display) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Initialization failure: %s\n", SDL_GetError());
        return NULL;
    }

    int windowPosX = 0;
    int windowPosY = 0;
    int width = 0;
    int height = 0;
    for (int i = 0; i < SDL_GetNumVideoDisplays(); i++) {
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(i, &dm);
        SDL_Rect bounds;
        SDL_GetDisplayBounds(i, &bounds);
        fprintf(stderr, "%c%d: Display name: %s, Desktop size: %d x %d, [%d,%d-%d,%d]\n",
            i == display ? '*' : ' ',
                i, SDL_GetDisplayName(i), dm.w, dm.h,
                bounds.x, bounds.y, bounds.w, bounds.h);
        if (i == display) {
            windowPosX = bounds.x;
            windowPosY = bounds.y;
            width = bounds.w;
            height = bounds.h;
        }
    }

    Gui *gui = calloc(1, sizeof(Gui));

    if (NULL == (gui->window = SDL_CreateWindow(
        "Game1",
        windowPosX, windowPosY,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
    )) ||
        NULL == (gui->renderer = SDL_CreateRenderer(gui->window, -1, 0))) {
        fprintf(stderr, "Failed to initialize gui: %s\n", SDL_GetError());
        destroy_gui(gui);
        return NULL;
    }
    gui->scale = height/SCRH;
    if (width/SCRW < gui->scale) {
        gui->scale = width/SCRW;
    }

    return gui;
}

void render_screen(Gui *gui) {
}

void game_loop(Gui *gui) {
    int now = SDL_GetTicks();
    Game *game = game_create(gui);
    if (game == NULL) {
        fprintf(stderr, "Failed to create game!\n");
        return;
    }
    while(1) {
        SDL_Event e;
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
            break;
        }
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            input_handle_event(e);
        }

        SDL_SetRenderTarget(gui->renderer, NULL);
        SDL_SetRenderDrawColor(gui->renderer, 0,0,0,0);
        SDL_RenderClear(gui->renderer);

        int then = now;
        now = SDL_GetTicks();
        deltatime_set((double)(now-then)/1000.0);

        game_update(game, gui);

        SDL_RenderPresent(gui->renderer);
        SDL_Delay(2);
    }
    game_destroy(game);
}

int main(int argc, char *argv[]) {
    int opt;
    int display = 0;
    while ((opt = getopt(argc, argv, "d:")) != -1) {
         switch (opt) {
         case 'd':
             sscanf(optarg, "%d", &display);
             break;
         default:
             fprintf(stderr, "Usage: %s [-d displayNo]\n", argv[0]);
             exit(EXIT_FAILURE);
         }
    }

    Gui *gui = create_gui(display);
    if (NULL == gui) {
        exit(EXIT_FAILURE);
    }
    game_loop(gui);
    destroy_gui(gui);
    return EXIT_SUCCESS;
}
