#ifndef KITCHEN_H_INCLUDED
#define KITCHEN_H_INCLUDED
#include "scene.h"
#include "element_type.h"

/*
   [game scene object]
*/

typedef struct _Kitchen
{
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *alert_bar_full;
    ALLEGRO_BITMAP *alert_bar_yellow;
    ALLEGRO_BITMAP *alert_bar_red;
    ALLEGRO_BITMAP *alert_bar_empty;

} Kitchen;
Scene *New_Kitchen(int label);
void kitchen_update(Scene *self);
void kitchen_draw(Scene *self);
void kitchen_destroy(Scene *self);

#endif
