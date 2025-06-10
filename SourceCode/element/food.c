#include "food.h"
#include "../global.h"
#include "../scene/scene.h"
#include <allegro5/allegro.h>
#include "charater.h"
#include "../shapes/Rectangle.h"
#include "../scene/element_type.h" //  <-- 新增
#include "../scene/sceneManager.h"   //  <-- 新增
#include "collision.h"             //  <-- 新增 (為了 check_collision)
extern Resources resources;
//bool f_key_released = true;
bool space_key_released = true;
extern int alert_level;
Elements *New_Food(int label, int x, int y, int scene_label, int id) {

    Food *pDerivedObj = (Food *)malloc(sizeof(Food));
    Elements *pObj = New_Elements(label);

    pDerivedObj->img = al_load_bitmap("assets/image/icon.png");
    pDerivedObj->slide_bar = al_load_bitmap("assets/image/slide_bar.png");
    pDerivedObj->slide_bar_pointer = al_load_bitmap("assets/image/slide_bar_pointer.png");


    if (!pDerivedObj->img) {
        fprintf(stderr, "failed to load food image\n");
        return NULL;
    }

    pDerivedObj->width = 32;
    pDerivedObj->height = 32;
    pDerivedObj->x = x;
    pDerivedObj->y = y;

    int half_w = pDerivedObj->width / 2;
    int half_h = pDerivedObj->height / 2;
    pDerivedObj->hitbox = New_Rectangle(
        pDerivedObj->x - half_w,
        pDerivedObj->y - half_h,
        pDerivedObj->x + half_w,
        pDerivedObj->y + half_h
    );

    pDerivedObj->id = id;

    pDerivedObj->collection_progress = 0;
    // pDerivedObj->character_is_colliding = false;
    pDerivedObj->alert_bar_active = false;
    pDerivedObj->alert_bar_direction = true;
    pDerivedObj->alert_bar_position = 0.0f;
    pDerivedObj->alert_bar_speed = 0.02f;

    if (food_states[pDerivedObj->id].is_collected && food_states[pDerivedObj->id].day_collected == game_clock.day) {
        pDerivedObj->is_collected = true;
        pDerivedObj->day_collected = game_clock.day;
    } else {
        pDerivedObj->is_collected = false;
        pDerivedObj->day_collected = -1;
    }

    pObj->pDerivedObj = pDerivedObj;
    
    pObj->inter_obj[pObj->inter_len++] = Character_L;

    pObj->Update = Food_update;
    pObj->Draw = Food_draw;
    pObj->Destroy = Food_destroy;
    pObj->Interact = Food_interact;

    return pObj;
}

void Food_update(Elements *self) {
    Food *food = (Food *)(self->pDerivedObj);

    //printf("food->is_collected: %d, game_day: %d, day_collected: %d\n", food->is_collected, game_clock.day, food->day_collected);
    //printf("[food %d] collected=%d, day_collected=%d, current_day=%d\n",
    // Respawn logic
    if (food->is_collected && game_clock.day >= food->day_collected + 1) {
        food->is_collected = false;
        food->day_collected = -1;
        
        Rectangle *rect = (Rectangle *)(food->hitbox->pDerivedObj);
        int w = rect->x2 - rect->x1;
        int h = rect->y2 - rect->y1;

        rect->x1 = food->x - w / 2;
        rect->x2 = food->x + w / 2;
        rect->y1 = food->y - h / 2;
        rect->y2 = food->y + h / 2;
    }

    // Collection logic
     ElementVec char_vec = _Get_label_elements(scene, Character_L);
    bool colliding = false;
    if (char_vec.len > 0) {
        Elements *char_ele = char_vec.arr[0];
        Character *character = (Character *)(char_ele->pDerivedObj);

        colliding = check_collision(food->hitbox, character->hitbox);
        //printf("%d\n", food->id);
    }

    int now_collecting=0;
    if (colliding||now_collecting==1) {
        
        if (!food->alert_bar_active && key_state[ALLEGRO_KEY_F] && f_key_released) {
            food->alert_bar_active = true;
            food->alert_bar_position = 0.0f;
            food->alert_bar_direction = true;
            food->alert_bar_speed = 0.02f;
            f_key_released = false;
        }

        if (food->alert_bar_active) {
            now_collecting=1;
            if (food->alert_bar_direction)
                food->alert_bar_position += food->alert_bar_speed;
            else
                food->alert_bar_position -= food->alert_bar_speed;

            if (food->alert_bar_position >= 1.0f) {
                food->alert_bar_position = 1.0f;
                food->alert_bar_direction = false;
            } else if (food->alert_bar_position <= 0.0f) {
                food->alert_bar_position = 0.0f;
                food->alert_bar_direction = true;
            }

            if (key_state[ALLEGRO_KEY_M] && space_key_released) {
                space_key_released = false;
                food->alert_bar_active = false;
                //printf("%d\n", alert_level);
                if (food->alert_bar_position > 0.44f && food->alert_bar_position < 0.52f) {
                    resources.food += 5 * resources.ants;
                    // Update global state
                    food_states[food->id].is_collected = true;
                    food_states[food->id].day_collected = game_clock.day;
                    now_collecting=0;
                }else if(food->alert_bar_position > 0.22f && food->alert_bar_position < 0.72f){
                    resources.food += 2 * resources.ants;
                    food_states[food->id].is_collected = true;
                    food_states[food->id].day_collected = game_clock.day;
                    now_collecting=0;
                }else {
                    //printf("%d\n", food->id);
                    alert_level -= 1;
                    food->is_collected = true;
                    if (alert_level <= 0) {
                        //printf("YASSS\n");
                        if(game_clock.hour < 17) game_clock.day += 1;
                        game_clock.hour = 8;
                        game_clock.min = 0;
                        resources.food = 0;
                        scene->scene_end = true;
                        window = 1;
                        return;
                    }
                }
                food->is_collected = true;
                food->day_collected = game_clock.day;

                al_rest(0.5);
            }
        }
    }

    if (!key_state[ALLEGRO_KEY_F]) f_key_released = true;
    if (!key_state[ALLEGRO_KEY_SPACE]) space_key_released = true;
}

void Food_interact(Elements *self) {
    // Food *food = (Food *)(self->pDerivedObj);

    // if (food->is_collected) return;

    // ElementVec char_vec = _Get_label_elements(scene, Character_L);
    // if (char_vec.len > 0) {
    //     Elements *char_ele = char_vec.arr[0];
    //     Character *character = (Character *)(char_ele->pDerivedObj);
        
    //     if (check_collision(food->hitbox, character->hitbox)) {
    //         food->character_is_colliding = true;
    //     }
    // }
    return;
}

void Food_draw(Elements *self) {
    Food *food = (Food *)(self->pDerivedObj);
    
    if (food->is_collected) {
        return;
    }else if (!food->is_collected) {
        al_draw_scaled_bitmap(
            food->img, 0, 0,
            al_get_bitmap_width(food->img),
            al_get_bitmap_height(food->img),
            food->x - food->width / 2,
            food->y - food->height / 2,
            food->width,
            food->height,
            0
        );
        //Rectangle *f = (Rectangle *)food->hitbox->pDerivedObj;
        //al_draw_rectangle(f->x1, f->y1, f->x2, f->y2, al_map_rgb(255, 0, 0), 2);
        if(food->alert_bar_active){
            int bar_width = 219;
            //int bar_height = al_get_bitmap_height(food->slide_bar);
            
            int bar_x = (WIDTH - bar_width) / 2;
            int bar_y = 250;

            al_draw_bitmap(food->slide_bar, bar_x, bar_y, 0);
            float bar_position = food->alert_bar_position;
            int pointer_x = bar_x + (int)(bar_position * bar_width) + 10;
            int pointer_y = bar_y + 32;
            
            int pointer_width = al_get_bitmap_width(food->slide_bar_pointer);
            //int pointer_height = al_get_bitmap_height(food->slide_bar_pointer);
            al_draw_bitmap(food->slide_bar_pointer, pointer_x - pointer_width / 2, pointer_y, 0);
        }
    }

}

void Food_destroy(Elements *self) {
    Food *food = (Food *)(self->pDerivedObj);
    al_destroy_bitmap(food->img);
    free(food->hitbox->pDerivedObj);
    free(food->hitbox);
    free(food);
    free(self);
}