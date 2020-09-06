#include <SDL2/SDL.h>

#include "tile_editor.h"
#include "input.h"
#include "guiconst.h"
#include "deltatime.h"
#include "tilemap.h"

void tile_editor_update_camera(TileEditorData *ted, Gui *gui, Vector2 *camera, int x, int y, int middle_button) {
    if (middle_button) {
        double scale = gui->scale * TILE_SIZE;
        double dx = (double)(x - ted->mx) / scale;
        double dy = (double)(y - ted->my) / scale;

        camera->x -= dx;
        camera->y -= dy;
    }
    if (camera->x < 0) {
        camera->x = 0;
    }
    if (camera->y < 0) {
        camera->y = 0;
    }

    ted->mx = x;
    ted->my = y;
}

void tile_editor_update(Tilemap *tilemap, TileEditorData *ted, Vector2 *camera, Gui *gui) {
    if (input_get()->save) {
        tilemap_save(tilemap, "tilemap.txt");
    }
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    int mouse = SDL_GetGlobalMouseState(NULL, NULL);
    double wx = (double)x / (double)(gui->scale * TILE_SIZE) + camera->x;
    double wy = (double)y / (double)(gui->scale * TILE_SIZE) + camera->y;

    if (mouse & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
        ted->cursor.pos.x = -1 ;
        ted->cursor.pos.y = -1 ;
    } else {
        ted->cursor.pos.x = floor(wx);
        ted->cursor.pos.y = floor(wy);

    }
    tile_editor_update_camera(ted, gui, camera, x, y, mouse & SDL_BUTTON(SDL_BUTTON_MIDDLE));

    int tx = ted->cursor.pos.x;
    int ty = ted->cursor.pos.y;
    if (mouse & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        tilemap->layers[0].tiles[ty][tx] = 1;
    } else if (mouse & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        tilemap->layers[0].tiles[ty][tx] = 0;
    }

}
