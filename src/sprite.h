#ifndef SRC_SPRITE_H_
#define SRC_SPRITE_H_

#include "vector.h"

typedef struct {
    Vector2 pos;
    int image;
    int flip_x;
} Sprite;

#endif /* SRC_SPRITE_H_ */
