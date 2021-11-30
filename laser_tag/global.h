#pragma once

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
  int game_start_timestamp;
  int saved_clock;
  int countdown;
} state_vars;

// FSM variables
extern int deaths;
extern int game_start_timestamp;
extern int saved_clock;
extern int countdown;
extern server_packet received_packet;

// FSM inputs
extern int trigger_pressed;
extern int sensor_value;
