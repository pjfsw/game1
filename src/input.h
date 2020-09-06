#ifndef SRC_INPUT_H_
#define SRC_INPUT_H_

#include <SDL2/SDL.h>

typedef struct {
    int up;
    int down;
    int left;
    int right;
    int jump;
    int edit;
    int save;
} Input;

void input_handle_event(SDL_Event event);

Input *input_get();

#endif /* SRC_INPUT_H_ */
