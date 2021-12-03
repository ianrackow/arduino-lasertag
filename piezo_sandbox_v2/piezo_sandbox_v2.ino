#include "piezo_sandbox_v2.h"

#include <Arduino.h>

#include "sounds.h"

// ———————————
// Variables
// ———————————

SimpleTimer sound_player;

// ———————————
// Button-related
// ———————————

const uint8_t BUTTON_CHECK_INTERVAL_MS = 30;
const uint8_t BUTTON_PRESS_INTERVAL_MS = 120;

unsigned long last_button_check_ms = 0;
unsigned long last_button_press_ms = 0;
bool last_button_read = LOW;

// ———————————
// Inputs
// ———————————

//static int PIEZO_PIN = 4;
static int BUTTON_PIN = 5;

// ##################################

void setup() {
  Serial.begin(9600);
  while (!Serial);
}

unsigned long last_tested_sound_ms = 0;
int sound_test_counter = 1;

void loop() {
  if (sound_test_counter <= 5 && millis() - last_tested_sound_ms > 2500) {
    Serial.println("Now playing sounds");
    make_sound((game_sound)sound_test_counter);
    sound_test_counter++;
    last_tested_sound_ms = millis();
  }
  
  /* Run sound. */
  sound_player.run();
  delay(12);
}

// ####################################

game_sound current_sound = PEW;

 void handle_button_press() {
   Serial.println("Pressed button");
   make_sound(current_sound);

   /* Cycle the next sound. */
   if (current_sound == PEW) {
     Serial.println("PEW");
     current_sound = HIT;
    
   } else if (current_sound == HIT) {
     Serial.println("HIT");
     current_sound = REVIVED;
    
   } else if (current_sound == REVIVED) {
     Serial.println("REVIVED");
     current_sound = GAME_OVER;
    
   } else if (current_sound == GAME_OVER) {
     Serial.println("GAME_OVER");
     current_sound = PEW;
    
   } else {
     Serial.print("ERROR: Couldn't find the next sound after ");
     Serial.println(current_sound);
   }
 }
