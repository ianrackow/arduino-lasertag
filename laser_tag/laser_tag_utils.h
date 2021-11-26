#include "global.h"

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
