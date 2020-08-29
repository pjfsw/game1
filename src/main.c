#include <SDL2/SDL.h>
#include <unistd.h>
#include <stdio.h>

#define SCRW 320
#define SCRH 180

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *screen;
    int w;
    int h;
    SDL_Rect viewport;
} Gui;

void destroy_gui(Gui *gui) {
    if (NULL == gui) {
        return;
    }
    if (NULL != gui->screen) {
        SDL_DestroyTexture(gui->screen);
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

    int scale = height/SCRH;
    if (width/SCRW < scale) {
        scale = width/SCRW;
    }
    gui->w = SCRW;
    gui->h = SCRH;
    gui->viewport.w = gui->w * scale;
    gui->viewport.h = gui->h * scale;
    gui->viewport.x = (width-gui->viewport.w)/2;
    gui->viewport.y = (height-gui->viewport.h)/2;
    fprintf(stderr, "Viewport %d x %d\n", gui->viewport.w, gui->viewport.h);

    if (NULL == (gui->window = SDL_CreateWindow(
        "Game1",
        windowPosX, windowPosY,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
    )) ||
        NULL == (gui->renderer = SDL_CreateRenderer(gui->window, -1, 0)) ||
        NULL == (gui->screen = SDL_CreateTexture(gui->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, gui->w, gui->h)
        )) {
        destroy_gui(gui);
        return NULL;
    }
    return gui;
}

void render_screen(Gui *gui) {
    SDL_SetRenderDrawColor(gui->renderer, 255,0,0,255);
    SDL_RenderDrawLine(gui->renderer, 0,0,gui->w, gui->h);
}

void game_loop(Gui *gui) {
    while(1) {
        SDL_Event e;
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT) {
            break;
        }
        SDL_SetRenderTarget(gui->renderer, gui->screen);
        SDL_SetRenderDrawColor(gui->renderer, 10,10,10,255);
        SDL_RenderClear(gui->renderer);
        render_screen(gui);

        SDL_SetRenderTarget(gui->renderer, NULL);
        SDL_SetRenderDrawColor(gui->renderer, 0,0,0,0);
        SDL_RenderClear(gui->renderer);
        SDL_RenderCopy(gui->renderer, gui->screen, NULL, &gui->viewport);
        SDL_RenderPresent(gui->renderer);
        SDL_Delay(2);
    }
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
