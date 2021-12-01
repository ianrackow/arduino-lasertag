#include "laser_tag_utils.h"

#include <Arduino.h>

#include "laser_tag.h"

// PIN ASSIGNMENTS
static int LASER = 3;
static int TRIGGER = 5;
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
 * Display "CALIBRATING" as a scroll on the LCD
 */
void calibrate() {
  if (CURRENT_STATE != sGAME_NOT_STARTED) {
    return;
  }
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
  vest_threshold = max_read + 20;
  Serial.print("Done calibrating, threshold: ");
  Serial.println(vest_threshold);
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
  // play desired sound
  switch (sound) {
    case PEW:
      tone(PIEZO, 262, 1000);
      break;
    case HIT:
      tone(PIEZO, 100, 1000);
      break;
    default:
      break;
  }
}

void report_hit() {
  Serial.println("Sending hit");
  connect_to_webpage();
}

#endif
