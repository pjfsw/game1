#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *args[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
  } 
  printf("Initialization succeeeded\n");
  SDL_Quit();
}

