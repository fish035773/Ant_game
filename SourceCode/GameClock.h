#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H

typedef struct{
    int hour;
    int min;
    int day;
    double timer_accum;
} GameClock;

void Update_Game_Clock(GameClock *clock, double delta);

#endif