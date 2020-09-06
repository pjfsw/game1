#ifndef _GAME_H_
#define _GAME_H_

#include "game_renderer.h"
#include "gui.h"
#include "vector.h"

typedef struct {
    Sprite player;
    GameRenderer *game_renderer;
    Vector2 camera;
    Vector2 camera_bounds_top_left;
    Vector2 camera_bounds_bottom_right;
    Tilemap *tilemap;
    double dx;
    double dy;
} Game;

Game *game_create();

void game_update(Game *game, Gui *gui);

void game_destroy(Game *game);

#endif /* _GAME_H_ */
