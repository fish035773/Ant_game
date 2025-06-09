#ifndef BOSS_FIGHT_H_INCLUDED
#define BOSS_FIGHT_H_INCLUDED
#include "scene.h"
#include "element_type.h"

/*
   [game scene object]
*/
typedef enum{
    MENU_NONE,
    MENU_SELECTING,
    MENU_ATTACK,
    MENU_DEFEND,
    MENU_ATTACK_BAR,
    MENU_ATTACK_JUDGE
}BossMenu;

typedef struct _Boss_Fight
{
    ALLEGRO_BITMAP *background;
    BossMenu menu_state;
    int menu_index;

    ALLEGRO_BITMAP *attack_button;
    ALLEGRO_BITMAP *defend_button;

    float bar_position;
    float bar_speed;
    bool bar_direction;
    bool bar_active;
    ALLEGRO_BITMAP *slide_bar;
    ALLEGRO_BITMAP *slide_bar_pointer;
} Boss_Fight;


Scene *New_Boss_Fight(int label);

void boss_fight_update(Scene *self);
void boss_fight_draw(Scene *self);
void boss_fight_destroy(Scene *self);

void boss_fight_menu_control(Scene *self);

#endif
