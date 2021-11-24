#define TESTING


typedef enum {
  sWAITING_FOR_GAME = 1,
  sNEUTRAL = 2,
  sJUST_FIRED = 3,
  sHIT = 4,
  sGAME_OVER = 5,
  sGUN_COOLDOWN = 6,
} state;

typedef enum {
  INVALID_PACKET = 0,
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
int trigger_pressed;
int sensor_value;

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
void set_vest_lights(light_status level);
void set_laser(int level);
void make_sound(game_sound sound);
void report_hit();
bool connect_to_webpage();
bool test_all_tests();

state update_fsm(state cur_state, long mils, int trigger_pressed, int sensor_value, server_packet received_packet);
