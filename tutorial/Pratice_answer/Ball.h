#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED
#include "element.h"
#include "../shapes/Shape.h"
/*
   [Ball object]
*/
typedef struct _Ball
{
    int x, y; // the position of image
    int r;    // the radius
    Shape *hitbox; // the hitbox of object
    ALLEGRO_COLOR color;
} Ball;

Elements *New_Ball(int label);
void Ball_update(Elements *self);
void Ball_interact(Elements *self);
void Ball_draw(Elements *self);
void Ball_destory(Elements *self);
#endif
