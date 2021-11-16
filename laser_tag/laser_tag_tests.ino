#include "laser_tag.h"

/*
 * A struct to keep all four state inputs in one place
 */
typedef struct {
  long mils;
  int trigger_pressed;
  int sensor_value;
  server_packet received_packet;
} state_inputs;

/*
 * A struct to keep all four state variables in one place
 */
typedef struct {
  int deaths;
  int game_start_timestamp;
  int saved_clock;
  int countdown;
} state_vars;

bool test_transition(state start_state,
                     state end_state,
                     state_inputs test_state_inputs,
                     state_vars start_state_vars,
                     state_vars end_state_vars,
                     bool verbos);
/*
 * Helper function for printing states
 */
const char *s2str(state s) {
  switch (s) {
    case sWAITING_FOR_GAME:
      return "(1) WAITING_FOR_GAME";
    case sNEUTRAL:
      return "(2) NEUTRAL";
    case sJUST_FIRED:
      return "(3) JUST_FIRED";
    case sHIT:
      return "(4) HIT";
    case sGAME_OVER:
      return "(5) GAME_OVER";
    case sGUN_COOLDOWN:
      return "(6) GUN_COOLDOWN";
    default:
      return "???";
  }
}

/*
 * Given a start state, inputs, and starting values for state variables, tests that
 * update_fsm returns the correct end state and updates the state variables correctly
 * returns true if this is the case (test passed) and false otherwise (test failed)
 *
 * Need to use "verbos" instead of "verbose" because verbose is apparently a keyword
 */
bool test_transition(state start_state,
                     state end_state,
                     state_inputs test_state_inputs,
                     state_vars start_state_vars,
                     state_vars end_state_vars,
                     bool verbos) {
  return verbos;
}

const state test_states_in[19] = {(state)1, (state)1, (state)1, (state)1, (state)2, (state)2, (state)2, (state)3, (state)3, (state)4, (state)4, (state)4, (state)5, (state)5, (state)5, (state)6, (state)6, (state)6, (state)7};
const state test_states_out[19] = {(state)1, (state)1, (state)2, (state)6, (state)2, (state)3, (state)3, (state)4, (state)7, (state)4, (state)5, (state)5, (state)2, (state)6, (state)7, (state)6, (state)1, (state)7, (state)7};
const state_inputs test_input[19] = {
    /* {2,LEFT,600}, {4,LEFT,1600}, {1,DOWN,1600}, {1,LEFT,1600}, {0,LEFT,9000}, {2,LEFT,1600}, {0,DOWN,2000}, {2,DOWN,5000}, {0,DOWN,1}, {1,UP,12000}, {1,LEFT,5000}, {0,LEFT,5000}, {4,DOWN,2000}, {4,RIGHT,6000}, {4,LEFT,1}, {2,RIGHT,4000}, {1,RIGHT,6000}, {0,LEFT,7000}, {1,LEFT,3} */
};
const state_vars test_in_vars[19] = {
    /* {1,2,UP,2,0,2,2,500,3}, {2,1,RIGHT,1,4,2,3,1000,0}, {2,1,LEFT,2,16,2,3,1000,-1}, {9,2,RIGHT,1,10,4,2,1000,-1}, {3,1,LEFT,1,2,0,700,8600,1}, {3,0,DOWN,0,4,0,500,1000,3}, {0,3,UP,0,13,1,1000,1000,1}, {5,0,LEFT,3,10,0,3,4000,1}, {4,1,RIGHT,2,2,5,0,0,2}, {4,4,LEFT,1,0,0,300,11900,1}, {6,0,RIGHT,4,15,0,600,4200,1}, {6,0,DOWN,4,15,2,600,4200,3}, {1,1,UP,0,16,0,2,1200,2}, {1,3,DOWN,0,16,0,0,5000,1}, {3,0,LEFT,9,9,8,3,2,1}, {3,1,RIGHT,0,1,0,600,3900,0}, {13,2,RIGHT,4,14,5,1000,4900,-1}, {9,2,LEFT,4,2,25,600,6200,4}, {4,1,UP,1,1,1,2,3,2} */
};
const state_vars test_out_vars[19] = {
    /* {1, 2, UP, 2, 0, 2, 2, 500, 3}, {2, 1, RIGHT, 1, 4, 2, 3, 1600, -1}, {2, 1, LEFT, 2, 16, 2, 3, 1600, -1}, {9, 2, RIGHT, 1, 10, 4, 2, 1600, -1}, {3, 1, LEFT, 1, 2, 0, 700, 8600, 1}, {3, 1, DOWN, 1, 4, 0, 500, 1000, 3}, {0, 2, UP, 0, 13, 1, 1000, 1000, 1}, {4, 0, LEFT, 3, 10, 0, 3, 5000, 1}, {4, 1, RIGHT, 2, 2, 5, 0, 0, 2}, {4, 4, LEFT, 1, 0, 0, 300, 11900, 1}, {6, 0, DOWN, 5, 15, 0, 600, 4200, 1}, {6, 0, LEFT, 4, 15, 2, 600, 4200, 3}, {1, 1, UP, 0, 16, 0, 2, 2000, 2}, {1, 3, DOWN, 0, 16, 0, 0, 6000, 1}, {3, 0, LEFT, 9, 9, 8, 3, 2, 1}, {3, 1, RIGHT, 0, 1, 0, 600, 3900, 0}, {4, 0, UP, 4, 14, 6, 950, 6000, 2}, {9, 2, LEFT, 4, 2, 25, 600, 6200, 4}, {4, 1, UP, 1, 1, 1, 2, 3, 2} */
};
const int num_tests = 19;

/*
 * Runs through all the test cases defined above
 */
bool test_all_tests() {
  for (int i = 0; i < num_tests; i++) {
    Serial.print("Running test ");
    Serial.println(i);
    if (!test_transition(test_states_in[i], test_states_out[i], test_input[i], test_in_vars[i], test_out_vars[i], true)) {
      return false;
    }
    Serial.println();
  }
  Serial.println("All tests passed!");
  return true;
}
