#include "utils.h"

#include <Arduino.h>

#include "piezo_sandbox_v2.h"
#include "sounds.h"

// ———————————————————
// Pin Assignments
// ———————————————————

static int PIEZO = 4;
static int BUTTON = 5;

// ———————————————————
// Functions
// ———————————————————

void initialize_system() {
  pinMode(PIEZO, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void make_sound(game_sound sound) {
  // Play desried sound.
  switch (sound) {
    case PEW:
      start_sound(sound);
      sound_player.setTimer(PEW_TIMER_INTERVAL_MS, play_next_pew_note, PEW_SAMPLES);
      sound_player.setTimeout(PEW_TIMER_INTERVAL_MS * PEW_SAMPLES, end_sound);
      break;
    case HIT:
      start_sound(sound);
      sound_player.setTimer(HIT_TIMER_INTERVAL_MS, play_next_hit_note, HIT_SAMPLES);
      sound_player.setTimeout(HIT_TIMER_INTERVAL_MS * HIT_SAMPLES, end_sound);
      break;
    case REVIVED:
    case GAME_STARTING:
      start_sound(sound);
      sound_player.setTimer(REVIV_TIMER_INTERVAL_MS, play_next_reviv_note, REVIV_SAMPLES);
      sound_player.setTimeout(REVIV_TIMER_INTERVAL_MS * REVIV_SAMPLES, end_sound);
      break;
    case GAME_OVER:
      start_sound(sound);
      // Opening jingle:
      sound_player.setTimer(GAME_OVER_JINGLE_INTERVAL_MS, play_game_over_jingle, GAME_OVER_JINGLE_SIZE);
      // Finishing note:
      sound_player.setTimeout(GAME_OVER_JINGLE_INTERVAL_MS * GAME_OVER_JINGLE_SIZE, play_game_over_finisher);
      sound_player.setTimeout(
        GAME_OVER_JINGLE_INTERVAL_MS * GAME_OVER_JINGLE_SIZE + GAME_OVER_FINISHER_DURATION_MS,
        end_sound
      );
      break;
  }
}
