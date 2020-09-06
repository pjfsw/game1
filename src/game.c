#include <stdlib.h>
#include "gui.h"
#include "game.h"
#include "deltatime.h"
#include "math.h"
#include "game_renderer.h"
#include "sprite.h"
#include "input.h"
#include "tilemap.h"

Game *game_create(Gui *gui) {
    Game *game = calloc(1, sizeof(Game));
    if (NULL == (game->game_renderer = game_renderer_create(gui)) ||
        NULL == (game->tilemap = tilemap_load("tilemap.txt"))) {
        game_destroy(game);
        return NULL;
    }
    double w = game->game_renderer->w / TILE_SIZE;
    double h = game->game_renderer->h / TILE_SIZE;

    game->camera_bounds_top_left.x = w/2 - 0.75;
    game->camera_bounds_top_left.y = h/2 - 0.75;
    game->camera_bounds_bottom_right.x = w/2 + 0.25;
    game->camera_bounds_bottom_right.y = h/2 + 0.25;

    return game;
}

void game_update_player(Game *game) {
    double speed = 3.0;

    if (input_get()->left) {
        game->player.pos.x -= speed * deltatime();
    } else if (input_get()->right) {
        game->player.pos.x += speed * deltatime();
    }
    if (input_get()->up) {
        game->player.pos.y -= speed * deltatime();
    } else if (input_get()->down) {
        game->player.pos.y += speed * deltatime();
    }
}

void game_update_camera(Game *game) {
    Vector2 *camera = &game->camera;
    Vector2 *player = &game->player.pos;

    if (player->x - camera->x < game->camera_bounds_top_left.x) {
        camera->x = player->x - game->camera_bounds_top_left.x;
    } else if (player->x - camera->x > game->camera_bounds_bottom_right.x) {
        camera->x = player->x - game->camera_bounds_bottom_right.x;
    }
    if (player->y - camera->y < game->camera_bounds_top_left.y) {
        camera->y = player->y - game->camera_bounds_top_left.y;
    } else if (player->y - camera->y > game->camera_bounds_bottom_right.y) {
        camera->y = player->y - game->camera_bounds_bottom_right.y;
    }

    if (camera->x < 0) {
        camera->x = 0.0;
    }
    if (camera->y < 0) {
        camera->y = 0.0;
    }
}

void game_update(Game *game, Gui *gui) {
    game_update_player(game);
    game_update_camera(game);

    Sprite* sprites[] = { &game->player };

    game_renderer_render(game->game_renderer, gui, &game->camera, game->tilemap, 1, sprites);
}

void game_destroy(Game *game) {
    if (NULL == game) {
        return;
    }
    if (NULL != game->tilemap) {
        tilemap_destroy(game->tilemap);
    }
    if (NULL != game->game_renderer) {
        game_renderer_destroy(game->game_renderer);
    }
    free(game);
}
