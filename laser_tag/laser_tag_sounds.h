#include "global.h"

// Note: see https://stackoverflow.com/questions/2190919/mixing-extern-and-const.

/*
 * Constants
 */

extern const int PEW_TIMER_INTERVAL_MS;
extern const int PEW_SAMPLES;

extern const int HIT_TIMER_INTERVAL_MS;
extern const int HIT_SAMPLES;

extern const int REVIV_TIMER_INTERVAL_MS;
extern const int REVIV_SAMPLES;

extern const int GAME_OVER_JINGLE_SIZE;
extern const int GAME_OVER_JINGLE_INTERVAL_MS;
extern const int GAME_OVER_FINISHER_DURATION_MS;

/*
 * Functions
 */

int convert_note_to_hz(float note);

void play_next_pew_note();
void play_next_hit_note();
void play_next_reviv_note();
void play_game_over_jingle();
void play_game_over_finisher();

void start_sound(game_sound sound);
void end_sound();
