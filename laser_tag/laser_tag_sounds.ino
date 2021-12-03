// ———————————————————
// PIN ASSIGNMENT
// ———————————————————

/** Redeclaring is the most hassle-free way to use this pin. */
static int PIEZO_PIN = 4;

// ———————————————————
// Relevant Variables
// ———————————————————

int timer_counter = 0;

/** Whether a sound is currently playing. */
bool is_playing_sound;

/** The last type of sound that _began_ playing. */
game_sound last_played_sound;

// ———————————————————
// Formula Constants
// ———————————————————

/** A4 is the 69th note (A0 is 21, C1 is 24.) */
const int A4_NOTE = 69;
const double A4_FREQ_HZ = 440; // Data type 'double' for efficiency.

/** Equal to (2)^(1/12). */
const float NOTE_TO_HZ_COEFF = 1.059463094359;

// ———————————————————
// Sound Constants
// ———————————————————

const int PEW_TIMER_INTERVAL_MS = 12;
const int PEW_SAMPLES = 40;

const float PEW_BASE_NOTE = 71;
const float PEW_AMOUNT_NOTES = 11;
const float PEW_SPEED_HZ = 8.2;

// ———

const int HIT_TIMER_INTERVAL_MS = 12;
const int HIT_SAMPLES = 106;
const int HIT_OSC_NUM_SAMPLES = 50;

const float HIT_BASE_NOTE = 66;
const float HIT_AMOUNT_NOTES = 16;
const float HIT_SPEED_HZ = 1.15;

// ———

const int REVIV_TIMER_INTERVAL_MS = 12;
const int REVIV_SAMPLES = 46;

const int REVIV_DURATION_MS = 560;
const int REVIV_TIMESHIFT_MS = 200;
const float REVIV_START_NOTE = 36;
const float REVIV_END_NOTE = 108;
const float REVIV_CURVE_FACTOR = 1.6;

// ———

const int GAME_OVER_JINGLE[] = {
  36, 43, 48, 52, 55, 60, 64, 67, // Arp 1
  38, 45, 50, 54, 57, 62, 66, 69, // Arp 2
  40, 45, 47, 52, 56, 59, 64, 68, 71 // Arp 3
};
const int GAME_OVER_JINGLE_SIZE = 25;
const int GAME_OVER_JINGLE_INTERVAL_MS = 72;

const int GAME_OVER_FINISHER_NOTE = 76;
const int GAME_OVER_FINISHER_DURATION_MS = 300;

// ———————————————————
// Waveform Functions
// ———————————————————

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
 * @param {float} speed_hz – "Oscillation frequency." The higher this is,
 *    the faster the cycle (and the more times the cycle completes in 1 second.)
 * @param {float} amount_notes – Another way of saying "amplitude."
 *    The higher this is, the more extreme the wobble.
 * @param {float} base_note – The base note for the oscillation.
 * @param {long} time_us – Time in microseconds.
 * 
 * @returns {int} Audio frequency value at the specified time, in Hz.
 */
float osc_to_note(
    bool is_cos, float speed_hz, float amount_notes, float base_note, long time_us
) {
  // 1 second = 1,000,000 ms
  float time_s = time_us / (float)1000000;
  double value_note;
  if (is_cos) {
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

// ———————————————————
// Play Sound Functions
// ———————————————————

/** Timer function. Plays next PEW note. */
void play_next_pew_note() {
  if (last_played_sound != PEW) {
    return;
  }
  noTone(PIEZO_PIN);
  
  float note_to_play = osc_to_note(
      true,
      PEW_SPEED_HZ,
      PEW_AMOUNT_NOTES,
      PEW_BASE_NOTE,
      PEW_TIMER_INTERVAL_MS * 1000 * timer_counter
    );
  int hz_to_play = convert_note_to_hz(note_to_play);
  tone(PIEZO_PIN, hz_to_play, PEW_TIMER_INTERVAL_MS);

  timer_counter += 1;
}

/** Timer function. Plays next HIT note. */
void play_next_hit_note() {
  if (last_played_sound != HIT) {
    return;
  }
  noTone(PIEZO_PIN);

  float note_to_play;
  if (timer_counter <= HIT_OSC_NUM_SAMPLES) {
      // Phase 1: Oscillate.
      note_to_play = osc_to_note(
          false,
          HIT_SPEED_HZ,
          HIT_AMOUNT_NOTES,
          HIT_BASE_NOTE, 
          HIT_TIMER_INTERVAL_MS * 1000 * timer_counter
        );
  } else {
      // Phase 2: Low note.
      note_to_play = HIT_BASE_NOTE - HIT_AMOUNT_NOTES + 5;
  }

  int hz_to_play = convert_note_to_hz(note_to_play);
  tone(PIEZO_PIN, hz_to_play, HIT_TIMER_INTERVAL_MS);

  timer_counter += 1;
}

/** Timer function. Plays next REVIVED note. */
void play_next_reviv_note() {
  if (last_played_sound != REVIVED && last_played_sound != GAME_STARTING) {
    return;
  }
  noTone(PIEZO_PIN);
  
  float note_to_play = sweep_to_note(
      REVIV_DURATION_MS,
      REVIV_TIMESHIFT_MS,
      REVIV_START_NOTE,
      REVIV_END_NOTE,
      REVIV_CURVE_FACTOR,
      REVIV_TIMER_INTERVAL_MS * 1000 * timer_counter // takes us
    );
  int hz_to_play = convert_note_to_hz(note_to_play);
  tone(PIEZO_PIN, hz_to_play, REVIV_TIMER_INTERVAL_MS);

  timer_counter += 1;
}

/** Timer function. Plays next GAME_OVER_JINGLE note. */
void play_game_over_jingle() {
  if (last_played_sound != GAME_OVER) {
    return;
  }
  noTone(PIEZO_PIN);
  if (timer_counter >= GAME_OVER_JINGLE_SIZE) {
    Serial.print("Error: tried to play another note in Game Over Jingle. Index:");
    Serial.println(timer_counter);
    return;
  }

  float hz_to_play = convert_note_to_hz(GAME_OVER_JINGLE[timer_counter] + 14);
  tone(PIEZO_PIN, hz_to_play, GAME_OVER_JINGLE_INTERVAL_MS);

  timer_counter += 1;
}

/** 
 * Timer function. Plays the GAME_OVER_FINISHER note.
 * Is played after the GAME_OVER jingle is done.
 */
void play_game_over_finisher() {
  if (last_played_sound != GAME_OVER) {
    return;
  }
  noTone(PIEZO_PIN);
  float hz_to_play = convert_note_to_hz(GAME_OVER_FINISHER_NOTE + 14);
  tone(PIEZO_PIN, hz_to_play, GAME_OVER_FINISHER_DURATION_MS);
}

// ————————————

void start_sound(game_sound new_sound) {
    timer_counter = 0;
    is_playing_sound = true;
    last_played_sound = new_sound;
}

void end_sound() {
    is_playing_sound = false;
}
