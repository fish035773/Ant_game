#include "GameClock.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include "global.h"
extern ALLEGRO_FONT* count_font;
extern Resources resources;
extern int alert_level;
void Init_Game_Clock(GameClock *clock){
    clock->day = 1;
    clock->hour = 8;
    clock->min = 0;
    clock->timer_accum = 0.0;
}

void Update_Game_Clock(GameClock *clock, double delta){
        if(clock->day >= 4) return;
    double game_minute = delta * (540 / 300*10);

    clock->timer_accum += game_minute;
    while(clock->timer_accum >= 1.0){
        clock->min++;
        clock->timer_accum -= 1.0;

        if(clock->min >= 60){
            clock->hour++;
            clock->min = 0;

            if(clock->hour > 17){

                
                resources.food_add = 0;
                window = 1;
                clock->hour = 8;
                clock->day++;
            }
        }
    }
}

void Clock_Draw(GameClock *clock, int x, int y){
    char buf[32];
    if(clock->day >= 4) return;
    if(clock->hour == 17 && clock->min >= 0){
        snprintf(buf, sizeof(buf), "Day %d %02d:%02d", clock->day, 17, 0);
        al_draw_text(count_font, al_map_rgb(255, 255, 255), x, y, 0, buf);
    }else {
        snprintf(buf, sizeof(buf), "Day %d %02d:%02d", clock->day, clock->hour, clock->min);
        al_draw_text(count_font, al_map_rgb(255, 255, 255), x, y, 0, buf);
    }

}