#pragma once

#include "SimpleTimer.h"
/* SimpleTimer is necessary for the sound timer, which allows
sounds to play asynchronously alongside the FSM code. */

typedef enum {
  sGAME_NOT_STARTED = 0,
  sCOUNTDOWN_TILL_START = 1,
  sNEUTRAL = 2,
  sJUST_FIRED = 3,
  sHIT = 4,
  sGAME_OVER = 5,
  sGUN_COOLDOWN = 6,
} state;

typedef enum {
  INVALID_PACKET = 0,
  GAME_START_NOT_SET = -1,
  GAME_IDLE = 1,
  GAME_START = 2,
} server_packet;

typedef enum {
  ON = 1,
  OFF = 2,
} light_status;

typedef enum {
  GAME_STARTING = 1,
  PEW = 2,
  GAME_OVER = 3,
  HIT = 4,
  REVIVED = 5,
} game_sound;

/*
 * A struct to keep all four state inputs in one place
 */
typedef struct {
  long mils;
  int trigger_pressed;
  int sensor_value;
  server_packet received_packet;
} state_inputs;

/*
 * A struct to keep all four state variables in one place
 */
typedef struct {
  int deaths;
  int game_start_time;
  int saved_clock;
  int countdown;
  int curr_time;
} state_vars;

// Global game variables
extern int shot_duration;
extern int shot_cooldown;
extern int hit_cooldown;
extern int game_duration;
extern int poll_game_start_interval;
extern int vest_threshold;  // We need to calibrate this

// FSM variables
extern int deaths;
extern int game_start_time;
extern int saved_clock;
extern server_packet received_packet;
extern int curr_time;

// FSM inputs
extern int trigger_pressed;
extern int sensor_value;

// FSM state
extern state CURRENT_STATE;

// ———————————————
// Sound
// ———————————————

/** Timer object for sounds. */
extern SimpleTimer sound_player;

/** Whether a sound is currently playing. */
extern bool is_playing_sound;

/** The last type of sound that _began_ playing. */
extern game_sound last_played_sound;
