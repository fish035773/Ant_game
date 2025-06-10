#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#define GAME_TERMINATE -1
#include "shapes/Point.h"
#include "./GameClock.h"
#include "./Resources.h"
#include "shapes/Point.h"
#define debug(x)                                                            \
    if (x)                                                                  \
    {                                                                       \
        printf("check at line number %d in file %s\n", __LINE__, __FILE__); \
    }
#define MAX_ELEMENT 100
#define min(x, y) ((x) > (y) ? (y) : (x))
#define max(x, y) ((x) < (y) ? (y) : (x))

// Add near other typedefs or structs
typedef struct {
    bool is_collected;
    int day_collected;
} FoodState;

// note that you can't assign initial value for extern variable!
extern const double FPS;
extern const int WIDTH;
extern const int HEIGHT;
extern const int GAME_WIDTH;
extern const int GAME_HEIGHT;
extern int window;
extern bool key_state[ALLEGRO_KEY_MAX];
extern bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES];
extern Point mouse;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_EVENT event;
extern ALLEGRO_TIMER *fps;
extern bool debug_mode;

extern bool f_key_released;
extern bool m_key_released;

extern int spawn_timer;
ALLEGRO_FONT* count_font;

// extern int spawn_timer;
extern ALLEGRO_FONT* count_font;
extern GameClock game_clock;
extern Resources resources;
extern FoodState food_states[100];
extern int alert_level;
#endif
