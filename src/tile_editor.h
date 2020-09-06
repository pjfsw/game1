#ifndef SRC_TILE_EDITOR_H_
#define SRC_TILE_EDITOR_H_

#include "sprite.h"
#include "vector.h"
#include "gui.h"
#include "tilemap.h"

typedef struct {
    Sprite cursor;
    int mx;
    int my;
} TileEditorData;


void tile_editor_update(Tilemap *tilemap, TileEditorData *ted, Vector2 *camera, Gui *gui);


#endif /* SRC_TILE_EDITOR_H_ */
