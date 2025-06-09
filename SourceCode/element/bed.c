#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "bed.h"
#include "collision.h" 
#include "charater.h"
#include "../shapes/Rectangle.h"
#include "../scene/scene.h"
#include "../scene/element_type.h"
#include "../scene/sceneManager.h"
#include "../scene/nest.h"
#include <stdio.h>
#include <stdbool.h>
extern Scene *scene;
extern GameClock game_clock;
extern bool f_key_released;
/*
   [Bed function]
*/
Elements *New_Bed(int label)
{
    al_init();
    Bed *pDerivedObj = (Bed *)malloc(sizeof(Bed));
    Elements *pObj = New_Elements(label);
    pDerivedObj->bed_img = al_load_bitmap("assets/image/bed.png");
    pDerivedObj->acc_img = al_load_bitmap("assets/image/accelerate.png");
    pDerivedObj->x = 130;
    pDerivedObj->y = HEIGHT - 78;

    Shape *shape = New_Rectangle(pDerivedObj->x, pDerivedObj->y, pDerivedObj->x + 78, pDerivedObj->y + 52);
    pDerivedObj->hitbox = shape;
    pDerivedObj->jumped = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Bed_draw;
    pObj->Update = Bed_update;
    pObj->Destroy = Bed_destroy;
    pObj->Interact = Bed_interact;
    f_key_released = true;
    return pObj;
}
void Bed_interact(Elements *self){
    return;
}
void Bed_update(Elements *self) {
    Bed *bed = (Bed *)(self->pDerivedObj);

    ElementVec char_vec = _Get_label_elements(scene, Character_L);
    bool colliding = false;
    if (char_vec.len > 0) {
        Elements *char_ele = char_vec.arr[0];
        Character *character = (Character *)(char_ele->pDerivedObj);

        colliding = check_collision(bed->hitbox, character->hitbox);
    }

    if (colliding) {
        if (key_state[ALLEGRO_KEY_F] && f_key_released) {
            game_clock.day += 1;
            game_clock.hour = 0;
            game_clock.min = 0;
            f_key_released = false;
            bed->jumped = true;
            bed->jump_img_start_time = al_get_time();
        }
    }
    if (!key_state[ALLEGRO_KEY_F]) f_key_released = true;
}
void Bed_draw(Elements *self)
{
    Bed *bed = ((Bed *)(self->pDerivedObj));
    al_draw_bitmap(bed->bed_img, bed->x, bed->y, 0);
    if(bed->jumped){
        double now = al_get_time();
        if(now - bed->jump_img_start_time < 1.5)
            al_draw_bitmap(bed->acc_img, (WIDTH - 276) / 2, (HEIGHT - 99) / 2, 0);
        else {
            bed->jumped = false;
        }
    }
    
}
void Bed_destroy(Elements* self)
{
    if (!self) return;
    Bed* c = (Bed*)(self->pDerivedObj);
    if (!c) {
        printf("[WARN] bed_destroy: pDerivedObj is NULL\n");
        return;
    }

    free(c);
    free(self);
}
