#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include "element_type.h"

/*
   [game scene object]
*/

typedef struct _GameScene
{
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *times_up;
    bool timeup;
    double time_up_img_start_time;
} GameScene;
Scene *New_Road(int label);
void  road_update(Scene *self);
void  road_draw(Scene *self);
void  road_destroy(Scene *self);

#endif
