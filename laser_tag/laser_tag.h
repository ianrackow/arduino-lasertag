#include <CapacitiveSensor.h>
#include <LiquidCrystal.h>

int cap_sensors[4];
int thresholds[4];

// #define TESTING
// bool test_all_tests();

/*
 * Type (enum and struct) definitions for state, orientation,
 * (xy) coordinate, (xyo) coordinate, (upper, lower) bounds
 */
typedef enum {
  sWAITING_FOR_GAME = 1,
  sNEUTRAL = 2,
  sJUST_FIRED = 3,
  sHIT = 4,
  sGAME_OVER = 5,
} state;

typedef enum {
   GAME_IDLE = 1,
} server_packet;


typedef enum {
  UP = 0,
  DOWN = 1,
  LEFT = 2,
  RIGHT = 3,
} orientation;

typedef struct {
  byte x;
  byte y;
  orientation o;
} xyo;

typedef struct {
  int l;
  int u;
} lu;

typedef struct {
  byte x;
  byte y;
} xy;

/*
 * Variables to keep track of inputs
 */
orientation last_button_pressed;
int num_buttons_pressed;

/*
 * Pixel-by-pixel definitions of cursors with and without tails
 */
byte arrows[8][8] = {{ B00100, B01110, B11111, B00000, B00000, B00000, B00000, B00000 },
                     { B00000, B11111, B01110, B00100, B00000, B00000, B00000, B00000 },
                     { B00100, B01100, B11100, B01100, B00100, B00000, B00000, B00000 },
                     { B00100, B00110, B00111, B00110, B00100, B00000, B00000, B00000 },
                     { B00000, B00000, B00000, B00000, B00100, B01110, B11111, B00000 },
                     { B00000, B00000, B00000, B00000, B00000, B11111, B01110, B00100 },
                     { B00000, B00000, B00000, B00100, B01100, B11100, B01100, B00100 },
                     { B00000, B00000, B00000, B00100, B00110, B00111, B00110, B00100 }};

byte tail_arrows[8][8] = {{ B00100, B01110, B11111, B00100, B00000, B00000, B00000, B00000 },
                          { B00100, B11111, B01110, B00100, B00000, B00000, B00000, B00000 },
                          { B00100, B01100, B11110, B01100, B00100, B00000, B00000, B00000 },
                          { B00100, B00110, B01111, B00110, B00100, B00000, B00000, B00000 },
                          { B00000, B00000, B00000, B00000, B00100, B01110, B11111, B00100 },
                          { B00000, B00000, B00000, B00000, B00100, B11111, B01110, B00100 },
                          { B00000, B00000, B00000, B00100, B01100, B11110, B01100, B00100 },
                          { B00000, B00000, B00000, B00100, B00110, B01111, B00110, B00100 }};
                          
// Keep track of which custom cursor is "cached" for which index
bool arrow_is_tail[8];

/*
 * Setup functions
 */
void initialize_system();
void calibrate();
void test_calibration();

/*
 * Read in gamepad inputs and update corresponding variables
 */
void update_inputs();

/*
 * Helper function definitions
 */
void reset_buttons();
void display_level(int l, int c);
void display_cursor(byte x, byte y, orientation o, bool with_tail, int lxb, int uxb);
xyo random_location_orientation(int lxb, int uxb);
orientation random_turn(orientation o, int lxb, int uxb);
lu shrink_bounds(byte x, orientation o, int lxb, int uxb);
void display_game_over(int l);

state update_fsm(state cur_state, long mils, int num_buttons, int last_button);
