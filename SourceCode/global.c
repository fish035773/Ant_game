#include "global.h"
#include "shapes/Shape.h"
#include <stdbool.h>
// variables for global usage
const double FPS = 60.0;
const int GAME_WIDTH = 1920;
const int GAME_HEIGHT = 1080;
const int WIDTH = 640;
const int HEIGHT = 360;
int window = 0;

ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER *fps = NULL;
ALLEGRO_FONT *clock_font = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES] = {false};
Point mouse;

bool f_key_released = true;
bool m_key_released = true;

bool debug_mode = true;

GameClock game_clock;
Resources resources;
FoodState food_states[100]; 
int alert_level;