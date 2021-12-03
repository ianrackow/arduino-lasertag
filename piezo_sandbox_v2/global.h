#pragma once

#include "SimpleTimer.h"
/* SimpleTimer is necessary for the sound timer, which allows
sounds to play asynchronously alongside the FSM code. */

typedef enum {
  GAME_STARTING = 1,
  PEW = 2,
  GAME_OVER = 3,
  HIT = 4,
  REVIVED = 5,
} game_sound;

// ———————————————
// Sound
// ———————————————

/** Timer object for sounds. */
extern SimpleTimer sound_player;

/** Whether a sound is currently playing. */
extern bool is_playing_sound;

/** The last type of sound that _began_ playing. */
extern game_sound last_played_sound;
