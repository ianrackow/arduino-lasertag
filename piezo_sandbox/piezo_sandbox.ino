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
  COS = 1,
  SIN = 2,
} osc_waveform;

sound_function PEW_SOUND_FUNCTION = OSC;

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

sound_function HIT_SOUND_FUNCTION = TONE;

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
 * @param {unsigned long} time_us – Time in microseconds.
 *    
 * @returns {int} Audio frequency value at the specified time, in Hz.
 */
float osc_to_note(
    osc_waveform waveform, float speed_hz, float amount_notes, float base_note, double time_us
) {
  // 1 second = 1,000,000 ms
  float time_s = time_us / (float)1000000;
  double value_note;
  if (waveform == COS) {
    value_note = base_note + amount_notes * cos(TWO_PI * speed_hz * time_s);
  } else {
    value_note = base_note + amount_notes * sin(TWO_PI * speed_hz * time_s);
  }
  return (float)value_note;
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
    current_sound = PEW;
  } else {
    Serial.print("ERROR: Couldn't find the next sound after ");
    Serial.println(current_sound);
  }
}


void make_sound(game_sound sound) {
  // play desired sound
  int tone_hz;

  float base_note;
  float amount_notes = 11;
  float speed_hz;

  float note_to_play;
  int hz_to_play;
  switch (sound) {
    case PEW:
//      tone(PIEZO, 262, 1000);
//      tone_hz = convert_note_to_hz(60);
//      Serial.println(tone_hz);
//      tone(PIEZO, tone_hz, 1000);

//        speed_hz = 8;
        speed_hz = 7;
        base_note = 71;

        for (int i = 0; i < 100; i++) {
          note_to_play = osc_to_note(COS, speed_hz, amount_notes, base_note, 5000 * i);
          hz_to_play = convert_note_to_hz(note_to_play);
          if (i % 10 == 0) {
            Serial.print("Note: ");
            Serial.print(note_to_play);
            Serial.print(" // Hz: ");
            Serial.println(hz_to_play);
          }
          tone(PIEZO, hz_to_play, 69);
          delayMicroseconds(5000);
          noTone(PIEZO);
//          delayMicroseconds(100);
        }
      break;
    case HIT:
//      tone(PIEZO, 100, 1000);
//      tone_hz = convert_note_to_hz(40);
//      Serial.println(tone_hz);
//      tone(PIEZO, tone_hz, 1000);
//        tone(PIEZO, convert_note_to_hz(40), 1000);

        speed_hz = 5;
        base_note = 60;
//        speed_hz = 7;
//        base_note = 71;

        for (int i = 0; i < 140; i++) {
          note_to_play = osc_to_note(SIN, speed_hz, amount_notes, base_note, 7000 * i);
          hz_to_play = convert_note_to_hz(note_to_play);
          if (i % 10 == 0) {
            Serial.print("Note: ");
            Serial.print(note_to_play);
            Serial.print(" // Hz: ");
            Serial.println(hz_to_play);
          }
          tone(PIEZO, hz_to_play, 69);
          delayMicroseconds(7000);
          noTone(PIEZO);
//          delayMicroseconds(100);
        }
      break;
    default:
      break;
  }
}
