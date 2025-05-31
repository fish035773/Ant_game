#include "GameClock.h"

void Update_Game_Clock(GameClock *clock, double delta){
    double game_minute = delta * (540 / 300);

    clock->timer_accum += game_minute;

    while(clock->timer_accum >= 1.0){
        clock->min++;
        clock->timer_accum -= 1.0;

        if(clock->min >= 60){
            clock->hour++;
            clock->min = 0;

            if(clock->hour > 17){
                clock->hour = 8;
                clock->day++;
            }
        }
    }
}