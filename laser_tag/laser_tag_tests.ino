#include "laser_tag_tests.h"

#include <Arduino.h>

#include "laser_tag.h"

#ifdef TESTING
/*
 * Helper function for printing states
 */
const char *s2str(state s) {
  switch (s) {
    case sGAME_NOT_STARTED:
      return "(1) GAME_NOT_STARTED";
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
  deaths = start_state_vars.deaths;
  game_start_time = start_state_vars.game_start_time;
  saved_clock = start_state_vars.saved_clock;
  state result_state = update_fsm(start_state, test_state_inputs.mils, test_state_inputs.trigger_pressed, test_state_inputs.sensor_value);
  bool passed_test = (end_state == result_state and
                      deaths == end_state_vars.deaths and
                      game_start_time == end_state_vars.game_start_time and
                      saved_clock == end_state_vars.saved_clock);
  if (!verbos) {
    return passed_test;
  } else if (passed_test) {
    char s_to_print[200];
    sprintf(s_to_print, "Test from %s to %s PASSED", s2str(start_state), s2str(end_state));
    Serial.println(s_to_print);
    return true;
  } else {
    char s_to_print[200];
    Serial.println(s2str(start_state));
    sprintf(s_to_print, "Test from %s to %s FAILED", s2str(start_state), s2str(end_state));
    Serial.println(s_to_print);
    sprintf(s_to_print, "End state expected: %s | actual: %s", s2str(end_state), s2str(result_state));
    Serial.println(s_to_print);
    sprintf(s_to_print, "Inputs: mils %ld | trigger_pressed %d | sensor_value %d", test_state_inputs.mils, test_state_inputs.trigger_pressed, test_state_inputs.sensor_value);
    Serial.println(s_to_print);
    sprintf(s_to_print, "    %s | %s | %s", "deaths", "game_start_time", "saved_clock");
    Serial.println(s_to_print);
    sprintf(s_to_print, "expected: %d | %d | %d", end_state_vars.deaths, end_state_vars.game_start_time, end_state_vars.saved_clock);
    Serial.println(s_to_print);
    sprintf(s_to_print, "actual:   %d | %d | %d", deaths, game_start_time, saved_clock);
    Serial.println(s_to_print);
    return false;
  }
  return verbos;
}

const state test_states_in[21] = {(state)0, (state)0, (state)0, (state)1, (state)1, (state)1, (state)2, (state)2, (state)2, (state)2, (state)3, (state)3, (state)3, (state)3, (state)4, (state)4, (state)4, (state)6, (state)6, (state)6, (state)6};
const state test_states_out[21] = {(state)0, (state)0, (state)1, (state)1, (state)1, (state)2, (state)2, (state)3, (state)4, (state)5, (state)3, (state)4, (state)5, (state)6, (state)4, (state)2, (state)5, (state)6, (state)2, (state)4, (state)5};
const state_inputs test_input[21] = {{1000, 0, 0}, {4000, 0, 0}, {0, 0, 0}, {0, 0, 0}, {4000, 0, 0}, {4000, 0, 0}, {9000, 0, 400}, {5000, 1, 400}, {60000, 0, 600}, {400000, 0, 0}, {60000, 0, 400}, {60000, 0, 600}, {300201, 0, 0}, {60000, 0, 400}, {60000, 0, 0}, {60000, 0, 0}, {503000, 0, 0}, {60000, 0, 400}, {60000, 0, 400}, {60000, 0, 600}, {310000, 0, 0}};
const state_vars test_in_vars[21] = {{0, 0, 900, 0}, {0, 0, 1900, 0}, {0, 100, 0, 90}, {0, 100, 0, 90}, {0, 100, 0, 99}, {0, 0, 0, 100}, {0, 287000, 0, 0}, {0, 2000, 2000, 0}, {0, 50000, 0, 0}, {0, 3000, 0, 0}, {0, 50000, 59950, 0}, {0, 50000, 0, 0}, {0, 200, 0, 0}, {0, 50000, 100, 0}, {0, 50000, 55000, 0}, {0, 50000, 50000, 0}, {0, 203000, 0, 0}, {0, 50000, 59800, 0}, {0, 50000, 59600, 0}, {1, 50000, 40000, 0}, {0, 10000, 0, 0}};
const state_vars test_out_vars[21] = {{0, 900, 0}, {0, 100, 4000, 0}, {0, 100, 0, 91}, {100, 0, 90}, {0, 100, 0, 100}, {0, 4000, 0, 100}, {287000, 0, 0}, {0, 2000, 5000, 0}, {1, 50000, 60000, 0}, {0, 3000, 0, 0}, {50000, 59950, 0}, {1, 50000, 60000, 0}, {0, 200, 0, 0}, {0, 50000, 60000, 0}, {50000, 55000, 0}, {0, 50000, 50000, 0}, {0, 203000, 0, 0}, {50000, 59800, 0}, {0, 50000, 59600, 0}, {2, 50000, 60000, 0}, {0, 10000, 0, 0}};
const int num_tests = 21;

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

#endif
