#ifndef BED_H_INCLUDED
#define BED_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "../scene/sceneManager.h"
#include "../algif5/algif.h"
#include "../scene/nest.h"
#include "element.h"
#include "../shapes/Shape.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int x, y;
    Shape *hitbox;
    ALLEGRO_BITMAP *bed_img;
    ALLEGRO_BITMAP *acc_img;
    bool is_active;
    bool jumped;
    double jump_img_start_time;
} Bed;

Elements *New_Bed(int label);
void Bed_update(Elements *self);
void Bed_draw(Elements *self);
void Bed_destroy(Elements *self);
void Bed_interact(Elements *self);

#endif
