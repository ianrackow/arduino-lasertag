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
void set_vest_lights(bool level);
void set_laser(bool level);

state update_fsm(state cur_state, long mils, int num_buttons, int last_button);
