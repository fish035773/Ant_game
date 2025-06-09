#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "boss.h"
#include "../scene/sceneManager.h"
#include "../algif5/algif.h"
#include "../scene/boss_fight.h"
#include <stdio.h>
#include <stdbool.h>
extern Scene *scene;
/*
   [Boss function]
*/
Elements *New_Boss(int label)
{
    Boss *pDerivedObj = (Boss *)malloc(sizeof(Boss));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load boss images
    //char state_string[3][10] = {"stop", "move", "attack"};
    pDerivedObj->anime_gif = algif_load_animation("assets/image/boss.gif");
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());
    pDerivedObj->bar_background = al_load_bitmap("assets/image/boss_hp_bar.png");
    //98
    pDerivedObj->width = 168;
    pDerivedObj->height = 221;
    
    pDerivedObj->x = (WIDTH - pDerivedObj->width) / 2;
    pDerivedObj->y = (HEIGHT - pDerivedObj->height) / 2 - 10;
    pDerivedObj->max_hp = 100;
    pDerivedObj->hp = 100;
    pDerivedObj->display_hp = 100;
    pDerivedObj->bar_width = 260;
    pDerivedObj->bar_height = 42;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Boss_draw;
    pObj->Update = Boss_update;
    pObj->Destroy = Boss_destroy;
    pObj->Interact = Boss_interact;
    return pObj;
}
void Boss_interact(Elements *self){
    return;
}
void Boss_update(Elements *self)
{
    Boss *chara = (Boss*)self->pDerivedObj;
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
void Boss_draw(Elements *self)
{
    // with the state, draw corresponding image
    Boss *chara = ((Boss *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->anime_gif, al_get_time());
    if(frame){
        al_draw_bitmap(frame, chara->x, chara->y, 0);
    }

    float bar_x = (WIDTH - chara->bar_width) / 2;
    float bar_y = chara->y - chara->bar_height - 5;
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
                            al_map_rgb(255, 61, 31));
}
void Boss_destroy(Elements* self)
{
    if (!self) return;
    Boss* c = (Boss*)(self->pDerivedObj);
    if (!c) {
        printf("[WARN] boss_destroy: pDerivedObj is NULL\n");
        return;
    }

    if(c->anime_gif){
        algif_destroy_animation(c->anime_gif);
        c->anime_gif = NULL;
    }

    free(c);
    free(self);
}
