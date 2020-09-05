#include <stdlib.h>
#include "gui.h"
#include "game.h"
#include "deltatime.h"
#include "math.h"
#include "game_renderer.h"

Game *game_create(Gui *gui) {
    Game *game = calloc(1, sizeof(Game));
    if (NULL == (game->game_renderer = game_renderer_create(gui))) {
        game_destroy(game);
        return NULL;
    }
    game->dx = 2.0;
    game->dy = 0.5;

    return game;
}

void game_update(Game *game, Gui *gui) {
    game->camera.x += game->dx * deltatime();
    game->camera.y += game->dy * deltatime();

    if (game->camera.x < 0.0) {
        game->camera.x = 0.0;
        game->dx = fabs(game->dx);
    }
    if (game->camera.x > 10.0) {
        game->camera.x = 10.0;
        game->dx = -fabs(game->dx);
    }
    if (game->camera.y < 0.0) {
        game->camera.y = 0.0;
        game->dy = fabs(game->dy);
    }
    if (game->camera.y > 10.0) {
        game->camera.y = 10.0;
        game->dy = -fabs(game->dy);
    }

    game_renderer_render(game->game_renderer, gui, game->camera);
}

void game_destroy(Game *game) {
    if (NULL == game) {
        return;
    }
    if (NULL != game->game_renderer) {
        game_renderer_destroy(game->game_renderer);
    }
    free(game);
}
