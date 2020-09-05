#include "input.h"

Input input_system_input;

void input_handle_event(SDL_Event event) {
    if (event.key.keysym.scancode == SDL_SCANCODE_W || event.key.keysym.scancode == SDL_SCANCODE_UP) {
        input_system_input.up = event.key.type == SDL_KEYDOWN;
    }
    if (event.key.keysym.scancode == SDL_SCANCODE_S || event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
        input_system_input.down = event.key.type == SDL_KEYDOWN;
    }
    if (event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
        input_system_input.left = event.key.type == SDL_KEYDOWN;
    }
    if (event.key.keysym.scancode == SDL_SCANCODE_D || event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
        input_system_input.right = event.key.type == SDL_KEYDOWN;
    }
}

Input *input_get() {
    return &input_system_input;
}
