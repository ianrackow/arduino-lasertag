//
//
///*
// * A struct to keep all three state inputs in one place
// */
//typedef struct {
//  int num_buttons;
//  orientation last_button;
//  long mils;
//} state_inputs;
//
///*
// * A struct to keep all 9 state variables in one place
// */
//typedef struct {
//  byte x;
//  byte y;
//  orientation o;
//  int lxb;
//  int uxb;
//  int level;
//  int time_step;
//  int saved_clock;
//  int countdown;
//} state_vars;
//
//bool test_transition(state start_state,
//                     state end_state,
//                     state_inputs test_state_inputs,
//                     state_vars start_state_vars,
//                     state_vars end_state_vars,
//                     bool verbos);
///*
// * Helper function for printing states
// */
//char* s2str(state s) {
//  switch(s) {
//    case sDISP_COUNTDOWN:
//    return "(1) DISP_COUNTDOWN";
//    case sWAIT_AFTER_ROT:
//    return "(2) WAIT_AFTER_ROT";
//    case sMOV:
//    return "(3) MOV";
//    case sWAIT_AFTER_MOV:
//    return "(4) WAIT_AFTER_MOV";
//    case sROT:
//    return "(5) ROT";
//    case sWAIT_FOR_BUT:
//    return "(6) WAIT_FOR_BUT";
//    case sGAME_OVER:
//    return "(7) GAME_OVER";
//    default:
//    return "???";
//  }
//}
//
///*
// * Given a start state, inputs, and starting values for state variables, tests that
// * update_fsm returns the correct end state and updates the state variables correctly
// * returns true if this is the case (test passed) and false otherwise (test failed)
// *
// * Need to use "verbos" instead of "verbose" because verbose is apparently a keyword
// */
//bool test_transition(state start_state,
//                     state end_state,
//                     state_inputs test_state_inputs,
//                     state_vars start_state_vars,
//                     state_vars end_state_vars,
//                     bool verbos) {
//  return verbos;
//}
//
///*
// * REPLACE THE FOLLOWING 6 LINES WITH YOUR TEST CASES
// */
////Alex's tests
//const state a_test_states_in[19] = {(state) 1, (state) 1, (state) 1, (state) 1, (state) 2, (state) 2, (state) 2, (state) 3, (state) 3, (state) 4, (state) 4, (state) 4, (state) 5, (state) 5, (state) 5, (state) 6, (state) 6, (state) 6, (state) 7};
//const state a_test_states_out[19] = {(state) 1, (state) 1, (state) 2, (state) 6, (state) 2, (state) 3, (state) 3, (state) 4, (state) 7, (state) 4, (state) 5, (state) 5, (state) 2, (state) 6, (state) 7, (state) 6, (state) 1, (state) 7, (state) 7};
//const state_inputs a_test_input[19] = {{0,DOWN,600}, {3,RIGHT,1600}, {2,LEFT,2200}, {1,LEFT,2200}, {4,LEFT,2300}, {1,LEFT,3200}, {0,LEFT,2000}, {0,DOWN,53421}, {2,RIGHT,4}, {2,LEFT,1900}, {1,DOWN,2000}, {0,DOWN,2000}, {1,LEFT,1000}, {3,LEFT,1000}, {0,RIGHT,2}, {2,DOWN,1900}, {1,LEFT,2000}, {1,DOWN,2000}, {0,UP,0}};
//const state_vars a_test_in_vars[19] = {{0,1,DOWN,1,1,2,0,500,2}, {0,1,LEFT,1,2,2,1,1000,0}, {0,0,DOWN,0,16,1,2,1600,-1}, {0,0,UP,0,16,1,4,1600,-1}, {2,2,RIGHT,2,1,0,1000,2200,1}, {0,0,DOWN,0,16,1,1000,2200,4}, {0,3,UP,0,13,1,1000,1000,4}, {0,0,DOWN,0,16,0,0,1000,4}, {2,0,DOWN,8,8,1,4,0,2}, {4,2,LEFT,4,2,4,4,1000,2}, {0,0,DOWN,0,16,2,1000,1000,1}, {0,0,DOWN,0,16,1,1000,1000,1}, {0,0,DOWN,0,16,1,2,0,0}, {0,0,UP,0,16,4,2,0,2}, {3,4,LEFT,8,8,2,3,0,4}, {3,3,UP,4,3,1,1000,1000,0}, {0,0,LEFT,1,16,1,1000,1000,-1}, {0,0,LEFT,1,16,0,1000,1000,3}, {3,4,DOWN,4,2,0,2,1,2}};
//const state_vars a_test_out_vars[19] = {{0,1,DOWN,1,1,2,0,500,2}, {0,1,LEFT,1,2,2,1,1600,-1}, {0,0,DOWN,0,16,1,2,2200,-1}, {0,0,UP,0,16,1,4,2200,-1}, {2,2,RIGHT,2,1,0,1000,2200,1}, {0,1,DOWN,1,16,1,1000,2200,4}, {0,2,UP,0,13,1,1000,1000,4}, {0,0,DOWN,0,16,0,0,53421,4}, {2,0,DOWN,8,8,1,4,0,2}, {4,2,LEFT,4,2,4,4,1000,2}, {0,0,LEFT,1,16,2,1000,1000,1}, {0,0,LEFT,0,16,1,1000,1000,1}, {0,0,DOWN,0,16,1,2,1000,0}, {0,0,UP,0,16,4,2,1000,2}, {3,4,LEFT,8,8,2,3,0,4}, {3,3,UP,4,3,1,1000,1000,0}, {1,0,UP,1,16,2,950,2000,2}, {0,0,LEFT,1,16,0,1000,1000,3}, {3,4,DOWN,4,2,0,2,1,2}};
//
//const state test_states_in[23] = {(state) 1, (state) 1, (state) 1, (state) 1, (state) 2, (state) 2, (state) 2, (state) 3, (state) 3, (state) 4, (state) 4, (state) 4, (state) 5, (state) 5, (state) 5, (state) 6, (state) 6, (state) 6, (state) 7, a_test_states_in[6], a_test_states_in[7], a_test_states_in[10], a_test_states_in[14]};
//const state test_states_out[23] = {(state) 1, (state) 1, (state) 2, (state) 6, (state) 2, (state) 3, (state) 3, (state) 4, (state) 7, (state) 4, (state) 5, (state) 5, (state) 2, (state) 6, (state) 7, (state) 6, (state) 1, (state) 7, (state) 7, a_test_states_out[6], a_test_states_out[7], a_test_states_out[10], a_test_states_out[14]};
//const state_inputs test_input[23] = {{2,LEFT,600}, {2,LEFT,1600}, {2,LEFT,1600}, {1,UP,1600}, {1,UP,400}, {1,UP,600}, {0,UP,600}, {0,DOWN,600}, {3,LEFT,3}, {2,RIGHT,400}, {1,RIGHT,600}, {0,UP,600}, {4,DOWN,0}, {0,LEFT,0}, {1,DOWN,2}, {2,UP,400}, {1,UP,600}, {0,UP,600}, {0,RIGHT,3}, a_test_input[6], a_test_input[7], a_test_input[10], a_test_input[14]};
//const state_vars test_in_vars[23] = {{0,1,RIGHT,3,3,4,1,0,1}, {2,1,DOWN,3,2,2,2,1000,0}, {0,0,DOWN,0,13,3,2,1000,-1}, {0,0,UP,0,13,0,0,1000,-1}, {2,0,DOWN,3,4,3,500,0,0}, /*5*/ {0,3,UP,0,13,2,500,0,1}, {0,3,UP,0,13,3,500,0,2}, {0,3,UP,0,13,1,0,2,4}, {0,2,LEFT,13,13,1,3,3,4}, {3,1,DOWN,3,2,2,500,0,4}, /*10*/ {1,3,UP,0,13,1,500,0,3}, {1,3,UP,0,13,3,500,0,4}, {1,3,UP,0,13,2,0,3,0}, {1,0,UP,0,13,1,2,3,0}, {3,0,UP,13,13,1,1,1,2}, /*15*/ {1,1,DOWN,3,0,2,500,0,0}, {2,2,UP,0,13,1,500,0,-1}, {1,1,RIGHT,1,4,1,500,0,2}, {1,1,RIGHT,3,1,4,4,2,4}, a_test_in_vars[6], a_test_in_vars[7], a_test_in_vars[10], a_test_in_vars[14]};
//const state_vars test_out_vars[23] = {{0,1,RIGHT,3,3,4,1,600,0}, {2,1,DOWN,3,2,2,2,1600,-1}, {0,0,DOWN,0,13,3,2,1600,-1}, {0,0,UP,0,13,0,0,1600,-1}, {2,0,DOWN,3,4,3,500,0,0}, /*5*/ {0,2,UP,1,13,2,500,0,1}, {0,2,UP,0,13,3,500,0,2}, {0,3,UP,0,13,1,0,600,4}, {0,2,LEFT,13,13,1,3,3,4}, {3,1,DOWN,3,2,2,500,0,4}, /*10*/ {1,3,RIGHT,1,13,1,500,0,3}, {1,3,RIGHT,0,13,3,500,0,4}, {1,3,UP,0,13,2,0,0,0}, {1,0,UP,0,13,1,2,0,0}, {3,0,UP,13,13,1,1,1,2}, /*15*/ {1,1,DOWN,3,0,2,500,0,0}, {0,0,UP,0,13,2,475,600,2}, {1,1,RIGHT,1,4,1,500,0,2}, {1,1,RIGHT,3,1,4,4,2,4}, a_test_out_vars[6], a_test_out_vars[7], a_test_out_vars[10], a_test_out_vars[14]};
//const int num_tests = 23;
//
//
//
///*
// * Runs through all the test cases defined above
// */
//bool test_all_tests() {
//  for (int i = 0; i < num_tests; i++) {
//    Serial.print("Running test ");
//    Serial.println(i);
//    if (!test_transition(test_states_in[i], test_states_out[i], test_input[i], test_in_vars[i], test_out_vars[i], true)) {
//      return false;
//    }
//    Serial.println();
//  }
//  Serial.println("All tests passed!");
//  return true;
//}*/
