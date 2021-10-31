// #define TESTING
// bool test_all_tests();

typedef enum {
  sWAITING_FOR_GAME = 1,
  sNEUTRAL = 2,
  sJUST_FIRED = 3,
  sHIT = 4,
  sGAME_OVER = 5,
  sGUN_COOLDOWN = 6,
} state;

typedef enum {
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
 * Variables to keep track of inputs
 */
orientation last_button_pressed;
int num_buttons_pressed;

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
