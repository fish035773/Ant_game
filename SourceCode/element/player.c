#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"
#include "../scene/sceneManager.h"
#include "../algif5/algif.h"
#include "../scene/boss_fight.h"
#include <stdio.h>
#include <stdbool.h>
extern Scene *scene;
/*
   [Player function]
*/
Elements *New_Player(int label)
{
    Player *pDerivedObj = (Player *)malloc(sizeof(Player));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load player images
    //char state_string[3][10] = {"stop", "move", "attack"};
    pDerivedObj->bar_background = al_load_bitmap("assets/image/player_hp_bar.png");

    pDerivedObj->max_hp = 100;
    pDerivedObj->hp = 100;
    pDerivedObj->display_hp = 100;
    pDerivedObj->bar_width = 260;
    pDerivedObj->bar_height = 42;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Player_draw;
    pObj->Update = Player_update;
    pObj->Destroy = Player_destroy;
    pObj->Interact = Player_interact;
    return pObj;
}
void Player_interact(Elements *self){
    return;
}
void Player_update(Elements *self)
{
    Player *chara = (Player*)self->pDerivedObj;
    float speed = 0.005;
    if(chara->display_hp > chara->hp){
        chara->display_hp -= speed;
        if(chara->display_hp < chara->hp){
            chara->display_hp = chara->hp;
        }
    }else if(chara->display_hp < chara->hp){
        chara->display_hp += speed;
        if(chara->display_hp > chara->hp){
            chara->display_hp = chara->hp;
        }
    }

    return;
}
void Player_draw(Elements *self)
{
    // with the state, draw corresponding image
    Player *chara = ((Player *)(self->pDerivedObj));

    float bar_x = (WIDTH - chara->bar_width) / 2;
    float bar_y = HEIGHT - 60;
    al_draw_bitmap(chara->bar_background, bar_x, bar_y, 0);

    float padding_x = 7;
    float padding_y = 5;
    float inner_x = bar_x + padding_x;
    float inner_y = bar_y + padding_y + 15;
    float inner_width = chara->bar_width - 2 * padding_x;
    float inner_height = chara->bar_height - 2 * padding_y;
    
    float hp_ratio = (float)chara->hp / chara->max_hp;
    float display_ratio = (float)chara->display_hp / chara->max_hp;
    
    al_draw_filled_rectangle(inner_x, inner_y,
                            inner_x + inner_width * display_ratio, inner_y + inner_height - 15,
                            al_map_rgb(255, 106, 69));
    al_draw_filled_rectangle(inner_x, inner_y,
                            inner_x + inner_width * hp_ratio, inner_y + inner_height - 15,
                            al_map_rgb(175, 227, 70));
}
void Player_destroy(Elements* self)
{
    if (!self) return;
    Player* c = (Player*)(self->pDerivedObj);
    if (!c) {
        printf("[WARN] player_destroy: pDerivedObj is NULL\n");
        return;
    }

    free(c);
    free(self);
}
