#ifndef SRC_GUI_H_
#define SRC_GUI_H_

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int scale;
} Gui;

#endif /* SRC_GUI_H_ */
