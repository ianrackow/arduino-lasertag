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

void loop() {
  /* Handle the button press. */ 
  if (millis() - last_button_check_ms >= BUTTON_CHECK_INTERVAL_MS) {
//    Serial.print(millis());
//    Serial.print(" // ");
//    Serial.println(last_button_check_ms);
    bool curr_button_read = digitalRead(BUTTON_PIN);
    Serial.println(digitalRead(BUTTON_PIN));

    if (millis() - last_button_press_ms > 120
      && curr_button_read == HIGH
      && last_button_read == LOW
    ) {
      handle_button_press();
      last_button_press_ms = millis();
    }
    
    last_button_read = curr_button_read;
    last_button_check_ms = millis();
  } else {
      Serial.print("-");
  }

  /* Run sound. */
  sound_player.run();
  delay(7);
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
