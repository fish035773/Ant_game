#include <allegro5/allegro_audio.h>
#include "boss_fight.h"
#include "sceneManager.h"
#include "../element/boss.h"
bool can_press = true;

Scene *New_Boss_Fight(int label)
{
    Boss_Fight *pDerivedObj = (Boss_Fight *)malloc(sizeof(Boss_Fight));
    Scene *pObj = New_Scene(label);
    
    // setting derived object member
    pDerivedObj->menu_state = MENU_SELECTING;
    pDerivedObj->background = al_load_bitmap("assets/image/background_layer_1.png");
    pDerivedObj->attack_button = al_load_bitmap("assets/image/attack_button.png");
    pDerivedObj->defend_button = al_load_bitmap("assets/image/defend_button.png");
    pDerivedObj->slide_bar = al_load_bitmap("assets/image/slide_bar.png");
    pDerivedObj->slide_bar_pointer = al_load_bitmap("assets/image/slide_bar_pointer.png");
    pObj->pDerivedObj = pDerivedObj;

    Elements *boss = New_Boss(BOSS_L);
    _Register_elements(pObj, boss);
    pDerivedObj->boss = boss;
    
    // setting derived object function
    pObj->Update = boss_fight_update;
    pObj->Draw = boss_fight_draw;
    pObj->Destroy = boss_fight_destroy;
    return pObj;
}

void boss_fight_update(Scene *self)
{   
    Boss_Fight *bf = (Boss_Fight*)self->pDerivedObj;
    Boss *boss = (Boss*)(bf->boss->pDerivedObj);

    ALLEGRO_KEYBOARD_STATE key;
    al_get_keyboard_state(&key);

    if(bf->menu_state == MENU_SELECTING){
        if(can_press){
            if(key_state[ALLEGRO_KEY_W]){
                bf->menu_index = (bf->menu_index - 1 + 2) % 2;
                can_press = false;
            }else if(key_state[ALLEGRO_KEY_S]){
                bf->menu_index = (bf->menu_index + 1) % 2;
                can_press = false;
            }else if(key_state[ALLEGRO_KEY_ENTER]){
                if(bf->menu_index == 0){
                    bf->menu_state = MENU_ATTACK_BAR;
                    bf->bar_position = 0.0f;
                    bf->bar_speed = 0.015f;
                    bf->bar_direction = true;
                    bf->bar_active = true;
                }else if(bf->menu_index == 1){
                    bf->menu_state = MENU_DEFEND;
                }
                can_press = false;
            }
        }
    }

    if(!key_state[ALLEGRO_KEY_W] && !key_state[ALLEGRO_KEY_S] && !key_state[ALLEGRO_KEY_ENTER]){
        can_press = true;
    }

    if(bf->menu_state == MENU_ATTACK_WAIT){
        if((bf->attack_return_timer-- <= 0)){
            bf->menu_state = MENU_SELECTING;
        }
    }
    if(bf->bar_active && bf->menu_state == MENU_ATTACK_BAR){
        
        if(bf->bar_direction){
            bf->bar_position += bf->bar_speed;
        }else {
            bf->bar_position -= bf->bar_speed;
        }

        if(bf->bar_position >= 1.0f){
            bf->bar_position = 1.0f;
            bf->bar_direction = false;
        }else if(bf->bar_position <= 0.0f){
            bf->bar_position = 0.0f;
            bf->bar_direction = true;
        }
        //WIDTH = 87
        //RED = 20, YELLOW = 18, GREEN = 7
        

        if(al_key_down(&key, ALLEGRO_KEY_SPACE)){
            float damage = 0.0f;
            bf->bar_active = false;
            bf->menu_state = MENU_ATTACK_JUDGE;

            if(bf->bar_position > 0.44f && bf->bar_position < 0.52f){
                damage += 20;
            }else if(bf->bar_position > 0.22f && bf->bar_position < 0.72f){
                damage += 10;
            }else{
                damage = 0;
            }
            boss->hp -= damage;
            bf->bar_active = false;
            bf->attack_return_timer = 80;
            bf->menu_state = MENU_ATTACK_WAIT;
            can_press = false;
        }
    }

    // update every element
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Update(ele);
    }
    // run interact for every element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Interact(ele);
    }
    
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(self, ele);
    }

    //switch to the second map
    if (key_state[ALLEGRO_KEY_P])
    {
        self->scene_end = true;
        window = 1;
        return;
    }
    
}
void boss_fight_draw(Scene *self)
{
    Boss_Fight *bf = (Boss_Fight*)self->pDerivedObj;

    al_clear_to_color(al_map_rgb(0, 0, 0));
    Boss_Fight *gs = ((Boss_Fight *)(self->pDerivedObj));
    al_draw_bitmap(gs->background, 0, 0, 0);

    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }

    int x = 30;
    int y_attack = 240;
    int y_defend = 290;

    if(bf->menu_state == MENU_SELECTING){
        if(bf->menu_index == 0){
            al_draw_bitmap(bf->attack_button, x, y_attack, 0);
            al_draw_tinted_bitmap(bf->defend_button, al_map_rgba_f(0.5, 0.5, 0.5, 1.0), x, y_defend, 0);
        }else {
            al_draw_tinted_bitmap(bf->attack_button, al_map_rgba_f(0.5, 0.5, 0.5, 1.0), x, y_attack, 0);
            al_draw_bitmap(bf->defend_button, x, y_defend, 0);
        }
    }

    if(bf->menu_state == MENU_ATTACK_BAR || bf->menu_state == MENU_ATTACK_JUDGE || bf->menu_state == MENU_ATTACK_WAIT){
        int bar_width = 219;
        //int bar_height = al_get_bitmap_height(bf->slide_bar);
        int bar_x = (WIDTH - bar_width) / 2;
        int bar_y = 250;

        al_draw_bitmap(bf->slide_bar, bar_x, bar_y, 0);

        float bar_position = bf->bar_position;
        int pointer_x = bar_x + (int)(bar_position * bar_width) + 10;
        int pointer_y = bar_y + 32;

        int pointer_width = al_get_bitmap_width(bf->slide_bar_pointer);
        //int pointer_height = al_get_bitmap_height(bf->slide_bar_pointer);
        al_draw_bitmap(bf->slide_bar_pointer, pointer_x - pointer_width / 2, pointer_y, 0);
    }
}
void boss_fight_destroy(Scene *self)
{
    Boss_Fight *Obj = (Boss_Fight *)(self->pDerivedObj);
    if (!Obj) {
        //printf("[ERROR] self->pDerivedObj is NULL\n");
        return;
    }

    //printf("[DEBUG] Freeing background...\n");
    if (Obj->background)
        al_destroy_bitmap(Obj->background);

    //printf("[DEBUG] Getting all elements...\n");
    ElementVec allEle = _Get_all_elements(self);
    //printf("[DEBUG] Total elements: %d\n", allEle.len);

    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        //printf("[DEBUG] element %d label = %d\n", i, ele->label);

        //printf("[DEBUG] Destroying element %d at %p\n", i, ele);
        if (ele) {
            if (ele->Destroy) {
                ele->Destroy(ele);
                //printf("[DEBUG] Destroyed element %d successfully\n", i);
            } else {
                //printf("[WARN] ele->Destroy is NULL at index %d\n", i);
            }
        } else {
            //printf("[WARN] Element %d is NULL\n", i);
        }
    }

    al_destroy_bitmap(Obj->attack_button);
    al_destroy_bitmap(Obj->defend_button);
    al_destroy_bitmap(Obj->slide_bar_pointer);
    al_destroy_bitmap(Obj->slide_bar);

    free(Obj);

    //printf("[DEBUG] Freeing Scene object...\n");
    free(self);

    //printf("[DEBUG] kitchen_destroy() finished\n");
}
