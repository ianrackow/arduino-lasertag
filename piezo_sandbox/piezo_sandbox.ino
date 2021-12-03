#include "SimpleTimer.h"

typedef enum {
  GAME_STARTING = 1,
  PEW = 2,
  GAME_OVER = 3,
  HIT = 4,
  REVIVED = 5,
} game_sound;

game_sound current_sound = PEW;

const uint8_t BUTTON_CHECK_INTERVAL_MS = 30;
const uint8_t BUTTON_PRESS_INTERVAL_MS = 120;

unsigned long last_button_check_ms = 0;
unsigned long last_button_press_ms = 0;
bool last_button_read = LOW;

// ———

const int A4_NOTE = 69; // A4 is the 69th note, where A0 is 21 and C1 is 24.
const double A4_FREQ_HZ = 440; // Data type 'double' for efficiency.

const float NOTE_TO_HZ_COEFF = 1.059463094359;

typedef enum {
  TONE = 1,
  OSC = 2,
} sound_function;

typedef enum {
  oscCOS = 1,
  oscSIN = 2,
} osc_waveform;

sound_function PEW_SOUND_FUNCTION = OSC;
sound_function HIT_SOUND_FUNCTION = TONE;

// NOTE: A sample rate of 1000 Hz might be okay, but better is 10,000 Hz, which
// means we play a sound once every 0.1 milliseconds (100 microseconds).
/*
 * We want our PEW sound to sound like an oscillation from B-N to B+N, where
 * B and N are note values and B = A5 = 71.
 * 
 * I'm guessing N will be about 11.
 * 
 * I don't know what time is going to be, but the cycle should play 4 complete
 * times in a half-second, which means 8 times per second. That's a frequency
 * of 8.
 * 
 * So, our equation can look like:
 * 
 * 
 */

// the timer object
SimpleTimer timer;

// ————————————————
// Inputs
// ————————————————

static int PIEZO = 4;
static int BUTTON = 5;

// ————————————————

void setup() {
    Serial.begin(9600);
    while (!Serial); // Wait for Serial to initialize
  
    pinMode(PIEZO, OUTPUT);
    pinMode(BUTTON, INPUT);
}


void loop() {
  
    /* Handle the button press. */ 
    if (millis() - last_button_check_ms >= BUTTON_CHECK_INTERVAL_MS) {
//        Serial.print(millis());
//        Serial.print(" // ");
//        Serial.println(last_button_check_ms);
        bool curr_button_read = digitalRead(BUTTON);
        Serial.println(digitalRead(BUTTON));
    
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
//      Serial.print("-");
    }
//    delay(1);

  timer.run();
}

// ————————————————
// Sound Functions
// ————————————————

/**
 * @param {float} note – Input note.
 *    Must be between 21 (A0) and 108 (C8), inclusive.
 * @returns The frequency of the note in Hz, rounded to the nearest Hz.
 */
int convert_note_to_hz(float note) {
  float half_step_distance = note - A4_NOTE;
  double hz = A4_FREQ_HZ * pow(NOTE_TO_HZ_COEFF, half_step_distance);
  return (int)hz;
}

/**
 * This represents an oscilliation function.
 * 
 * @param {float} speed_hz – Another way of saying "oscillation frequency."
 *    The higher this is, the faster the cycle (and the more times the cycle
 *    completes in 1 second.)
 * 
 * @param {float} amount_notes – Another way of saying "amplitude."
 *    The higher this is, the more extreme the wobble.
 *    
 * @param {long} time_us – Time in microseconds.
 *    
 * @returns {int} Audio frequency value at the specified time, in Hz.
 */
float osc_to_note(
    osc_waveform waveform, float speed_hz, float amount_notes, float base_note, double time_us
) {
  // 1 second = 1,000,000 ms
  float time_s = time_us / (float)1000000;
  double value_note;
  if (waveform == oscCOS) {
    value_note = base_note + amount_notes * cos(TWO_PI * speed_hz * time_s);
  } else {
    value_note = base_note + amount_notes * sin(TWO_PI * speed_hz * time_s);
  }
  return (float)value_note;
}


/**
 * This represents a sweep function. This sweep's equation is x^curve_factor.
 * 
 * @param {float} curve_factor – How dire you want the curve to be. A good default is 2.
 */
float sweep_to_note(
  int duration_ms,
  int timeshift_ms,
  float start_note,
  float end_note,
  float curve_factor,
  long time_us
) {
  float note_diff = end_note - start_note;
  float time_ms = (double)time_us / 1000.f;

  float mult_coeff = (float)note_diff / pow(duration_ms + timeshift_ms, curve_factor);

  // Call equation
  float value_note = mult_coeff * pow(time_ms + timeshift_ms, curve_factor) + start_note;
  return value_note;
}

// ————————————————

int timer_counter = 0;
bool piezo_mutex_lock = false;

const int PEW_TIMER_INTERVAL_MS = 4;
const int PEW_SAMPLES = 120;

const float PEW_BASE_NOTE = 71;
const float PEW_AMOUNT_NOTES = 11;
const float PEW_SPEED_HZ = 8.4;

/** Timer function. Plays next PEW note. */
void play_next_pew_note() {
  noTone(PIEZO);
  
  float note_to_play = osc_to_note(
      oscCOS,
      PEW_SPEED_HZ,
      PEW_AMOUNT_NOTES,
      PEW_BASE_NOTE,
      PEW_TIMER_INTERVAL_MS * 1000 * timer_counter
    );
  int hz_to_play = convert_note_to_hz(note_to_play);

  
  if (timer_counter % 10 == 0) {
    Serial.print(timer_counter);
    Serial.print(" // Note: ");
    Serial.print(note_to_play);
    Serial.print(" // Hz: ");
    Serial.println(hz_to_play);
  }
  
  tone(PIEZO, hz_to_play, PEW_TIMER_INTERVAL_MS);

  timer_counter += 1;
}

const int HIT_TIMER_INTERVAL_MS = 8;
const int HIT_SAMPLES = 180;

const float HIT_BASE_NOTE = 60;
const float HIT_AMOUNT_NOTES = 8;
const float HIT_SPEED_HZ = 3.3;

/** Timer function. Plays next HIT note. */
void play_next_hit_note() {
  noTone(PIEZO);

  float note_to_play;
  int hz_to_play;
  if (timer_counter <= 110) {
      // Phase 1: Oscillate.
      note_to_play = osc_to_note(
          oscSIN,
          HIT_SPEED_HZ,
          HIT_AMOUNT_NOTES,
          HIT_BASE_NOTE, 
          HIT_TIMER_INTERVAL_MS * 1000 * timer_counter
        );
      hz_to_play = convert_note_to_hz(note_to_play);
    
      
  } else {
      // Phase 2: Low note.
      note_to_play = HIT_BASE_NOTE - HIT_AMOUNT_NOTES + 5;
      hz_to_play = convert_note_to_hz(note_to_play);
  }
  
  if (timer_counter % 10 == 0) {
    Serial.print(timer_counter);
    Serial.print(" // Note: ");
    Serial.print(note_to_play);
    Serial.print(" // Hz: ");
    Serial.println(hz_to_play);
  }
  
  tone(PIEZO, hz_to_play, HIT_TIMER_INTERVAL_MS);

  timer_counter += 1;
}

const int REVIV_TIMER_INTERVAL_MS = 8;
const int REVIV_SAMPLES = 125;

const int REVIV_DURATION_MS = 1000;
const int REVIV_TIMESHIFT_MS = 300;
const float REVIV_START_NOTE = 36;
//const float REVIV_END_NOTE = 112;
const float REVIV_END_NOTE = 108;
const float REVIV_CURVE_FACTOR = 1.5;

void play_next_reviv_note() {
  noTone(PIEZO);
  
  float note_to_play = sweep_to_note(
      REVIV_DURATION_MS,
      REVIV_TIMESHIFT_MS,
      REVIV_START_NOTE,
      REVIV_END_NOTE,
      REVIV_CURVE_FACTOR,
      REVIV_TIMER_INTERVAL_MS * 1000 * timer_counter // takes us
    );
  int hz_to_play = convert_note_to_hz(note_to_play);

  
  if (timer_counter % 10 == 0) {
    Serial.print(timer_counter);
    Serial.print(" // Note: ");
    Serial.print(note_to_play);
    Serial.print(" // Hz: ");
    Serial.println(hz_to_play);
  }
  
  tone(PIEZO, hz_to_play, REVIV_TIMER_INTERVAL_MS);

  timer_counter += 1;
}

const int GAME_OVER_JINGLE[] = {
  36, 43, 48, 52, 55, 60, 64, 67, // Arp 1
  38, 45, 50, 54, 57, 62, 66, 69, // Arp 2
  40, 45, 47, 52, 56, 59, 64, 68, 71 // Arp 3
};
const int GAME_OVER_JINGLE_SIZE = 25;
const int GAME_OVER_JINGLE_INTERVAL_MS = 70;

const int GAME_OVER_FINISHER_NOTE = 76;
const int GAME_OVER_FINISHER_DURATION_MS = 300;

/** Timer function. Plays next GAME_OVER_JINGLE note. */
void play_game_over_jingle() {
  noTone(PIEZO);
  if (timer_counter >= GAME_OVER_JINGLE_SIZE) {
    Serial.print("Error: tried to play another note in Game Over Jingle. Index:");
    Serial.println(timer_counter);
    return;
  }

  float hz_to_play = convert_note_to_hz(GAME_OVER_JINGLE[timer_counter] + 14);
  tone(PIEZO, hz_to_play, GAME_OVER_JINGLE_INTERVAL_MS);

  timer_counter += 1;
}

void play_game_over_finisher() {
  noTone(PIEZO);
  float hz_to_play = convert_note_to_hz(GAME_OVER_FINISHER_NOTE + 14);
  tone(PIEZO, hz_to_play, GAME_OVER_FINISHER_DURATION_MS);
}

void lock_sound() {
  piezo_mutex_lock = true;
  timer_counter = 0;
}

void unlock_sound() {
  piezo_mutex_lock = false;
}

// ————————————————

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

void make_sound(game_sound sound) {

    float tone_note;
    int tone_hz;
  
    switch (sound) {
        case PEW:
            if (piezo_mutex_lock == false) {
                lock_sound();
                timer.setTimer(PEW_TIMER_INTERVAL_MS, play_next_pew_note, PEW_SAMPLES);
                timer.setTimeout(PEW_TIMER_INTERVAL_MS * PEW_SAMPLES, unlock_sound);
            }
            break;
        case HIT:
            if (piezo_mutex_lock == false) {
//                tone(PIEZO, 150, 1000);
                lock_sound();
                timer.setTimer(HIT_TIMER_INTERVAL_MS, play_next_hit_note, HIT_SAMPLES);
                timer.setTimeout(HIT_TIMER_INTERVAL_MS * HIT_SAMPLES, unlock_sound);
                
            }
            break;
        case REVIVED:
        case GAME_STARTING:
            if (piezo_mutex_lock == false) {
                lock_sound();
                timer.setTimer(REVIV_TIMER_INTERVAL_MS, play_next_reviv_note, REVIV_SAMPLES);
                timer.setTimeout(REVIV_TIMER_INTERVAL_MS * REVIV_SAMPLES, unlock_sound);
            }
            break;
        case GAME_OVER:
            if (piezo_mutex_lock == false) {
                lock_sound();
                int FULL_JINGLE_DURATION =
                    GAME_OVER_JINGLE_INTERVAL_MS * GAME_OVER_JINGLE_SIZE + GAME_OVER_FINISHER_DURATION_MS;
                // 1. Jingle start
                timer.setTimer(GAME_OVER_JINGLE_INTERVAL_MS, play_game_over_jingle, GAME_OVER_JINGLE_SIZE);
                timer.setTimeout(GAME_OVER_JINGLE_INTERVAL_MS * GAME_OVER_JINGLE_SIZE, play_game_over_finisher);
                timer.setTimeout(FULL_JINGLE_DURATION, unlock_sound);
            }
        default:
            break;
    }
}
