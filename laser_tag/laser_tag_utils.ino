#include "laser_tag_utils.h"

#include <Arduino.h>

#include "laser_tag.h"
#include "laser_tag_sounds.h"

// PIN ASSIGNMENTS
static int LASER = 3;
static int TRIGGER = 2;
static int LIGHT_SENSOR = A1;
static int VEST_LIGHTS = 7;
static int PIEZO = 4;
static int CALIBRATE_BUTTON = 6;

/*
 * Initialize system: set random, initialize arrow characters, set up LCD
 */
void initialize_system() {
  pinMode(LASER, OUTPUT);
  pinMode(VEST_LIGHTS, OUTPUT);
  pinMode(PIEZO, OUTPUT);
  pinMode(LIGHT_SENSOR, INPUT);
  pinMode(TRIGGER, INPUT);
  pinMode(CALIBRATE_BUTTON, INPUT);

  attachInterrupt(CALIBRATE_BUTTON, calibrate, RISING);
}

/*
 * Calibrate photo sensors
 */
void calibrate() {
  int max_read = 0;
  Serial.println("Calibrating to current light");
  for (int i = 0; i < 15; i++) {
    int sensor_value = analogRead(LIGHT_SENSOR);
    Serial.println(sensor_value);
    if (sensor_value > max_read) {
      max_read = sensor_value;
    }
    delay(200);
  }
  vest_threshold = max_read + 40;
  Serial.print("Done calibrating, threshold: ");
  Serial.println(vest_threshold);

  if (CURRENT_STATE == sGAME_OVER) {
    while (true)
      ;
  }
}

/*
 * Display which sensor has been pressed on the gamepad, assuming labels[] and thresholds[]
 * have been set up correctly
 */
void test_calibration() {
}

/*
 * Read in gamepad inputs and update num_buttons_pressed and last_button_pressed
 * We only increase "num_buttons_pressed" if the current button pressed is greater than the previous button pressed
 */
void update_inputs() {
  trigger_pressed = digitalRead(TRIGGER);
  // Serial.println(trigger_pressed);
  sensor_value = analogRead(LIGHT_SENSOR);
  // Serial.println(sensor_value);
}

void set_vest_lights(light_status level) {
  if (level == ON) {
    // Serial.println("Vest on");
    digitalWrite(VEST_LIGHTS, HIGH);
  } else {
    // Serial.println("Vest off");
    digitalWrite(VEST_LIGHTS, LOW);
  }
}

#ifdef TESTING

// Don't actually turn the laser on for testing so we don't accidentally blind anyone
// We just assume that the hit worked
void set_laser(int level) {
  if (level == ON) {
    Serial.println("Laser on");
    sensor_value = vest_threshold + 1000;
  } else {
    Serial.println("Laser off");
  }
}

// Don't actually play any sounds
void make_sound(game_sound sound) {
}

// Don't try to connect to actual webpage, this is for later integration testing
void report_hit() {
  Serial.println("Sending hit");
}

#else

void set_laser(int level) {
  if (level == ON) {
    Serial.println("Laser on");
    digitalWrite(LASER, HIGH);
  } else {
    Serial.println("Laser off");
    digitalWrite(LASER, LOW);
  }
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

  // switch (sound) {
  //   case PEW:
  //     tone(PIEZO, 262, shot_duration);
  //     break;
  //   case HIT:
  //     tone(PIEZO, 100, 1000);
  //     break;
  //   case GAME_OVER:
  //     tone(PIEZO, 440, 100);
  //     delay(100);
  //     tone(PIEZO, 277, 100);
  //     delay(100);
  //     tone(PIEZO, 220, 100);
  //     delay(100);
  //     tone(PIEZO, 165, 200);
  //     delay(200);
  //     tone(PIEZO, 110, 200);
  //     break;
  //   case GAME_STARTING:
  //     tone(PIEZO, 110, 100);
  //     delay(100);
  //     tone(PIEZO, 165, 100);
  //     delay(100);
  //     tone(PIEZO, 220, 100);
  //     delay(100);
  //     tone(PIEZO, 277, 100);
  //     delay(100);
  //     tone(PIEZO, 440, 100);
  //     break;
  //   case REVIVED:
  //     tone(PIEZO, 110, 100);
  //     delay(100);
  //     tone(PIEZO, 440, 300);
  //   default:
  //     break;
  // }
}

void report_hit() {
  Serial.println("Sending hit");
  connect_to_webpage();
}

#endif
