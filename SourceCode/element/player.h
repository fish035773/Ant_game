#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../algif5/algif.h"
#include <stdbool.h>


typedef struct _Player
{
    int hp;
    int max_hp;
    int display_hp;
    int damage;
    int bar_width;
    int bar_height;
    ALLEGRO_BITMAP *bar_background;
} Player;

Elements *New_Player(int label);
void Player_update(Elements *self);
void Player_draw(Elements *self);
void Player_destroy(Elements *self);
void Player_interact(Elements *self);
#endif
