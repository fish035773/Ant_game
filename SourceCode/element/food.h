#ifndef FOOD_H_INCLUDED
#define FOOD_H_INCLUDED
#include "element.h"
#include "../shapes/Shape.h"
#include "charater.h"
#include <stdbool.h>

#define MAX_COLLECTION_PROGRESS 100.0
#define COLLECTION_RATE 0.85 

typedef struct _Food {
    int x, y;
    int width, height;
    ALLEGRO_BITMAP *img;
    ALLEGRO_BITMAP *slide_bar;
    ALLEGRO_BITMAP *slide_bar_pointer;
    
    ALLEGRO_BITMAP *alert_bar_full;
    ALLEGRO_BITMAP *alert_bar_yellow;
    ALLEGRO_BITMAP *alert_bar_red;
    ALLEGRO_BITMAP *alert_bar_empty;

    Shape *hitbox;
    bool is_collected;
    int day_collected;
    double collection_progress; 
    // bool character_is_colliding;
    bool alert_bar_active;
    float alert_bar_position;
    bool alert_bar_direction;
    float alert_bar_speed;
    int alert_level;

    Character *player;
    int id;
} Food;


Elements *New_Food(int label, int x, int y, int scene_label, int id);
void Food_update(Elements *self);
void Food_draw(Elements *self);
void Food_destroy(Elements *self);
void Food_interact(Elements *self);

#endif