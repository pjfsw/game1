#include <stdlib.h>
#include "gui.h"
#include "game.h"
#include "tilemap.h"
#include "deltatime.h"
#include "math.h"

Game *game_create(Gui *gui) {
    Game *game = calloc(1, sizeof(Game));
    if (NULL == (game->tilemap = tilemap_create(gui))) {
        game_destroy(game);
        return NULL;
    }

    return game;
}

void game_update(Game *game, Gui *gui) {
    game->camera.x += 2.0 * deltatime();
    game->camera.y += 0.5 * deltatime();

    if (game->camera.x > 10.0) {
        game->camera.x -= 10.0;
    }
    if (game->camera.y > 10.0) {
        game->camera.y -= 10.0;
    }

    tilemap_render(game->tilemap, gui, game->camera);
}

void game_destroy(Game *game) {
    if (NULL == game) {
        return;
    }
    free(game);
}
