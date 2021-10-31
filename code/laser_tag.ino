#include "lab5.h"

// FSM variables
byte x, y;
orientation o;
int lxb, uxb;
int level, time_step, saved_clock, countdown;

state CURRENT_STATE;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initialize_system();

  /*
   * LAB STEP 3b
   */
//  calibrate();
  // Change to 7, 8, 9, 10 based on calibration
  cap_sensors[UP] = 9;
  cap_sensors[RIGHT] = 8;
  cap_sensors[DOWN] = 7;
  cap_sensors[LEFT] = 10;
  // Change based on calibration
  thresholds[UP] = 600;
  thresholds[RIGHT] = 700;
  thresholds[DOWN] = 2500;
  thresholds[LEFT] = 600;

  /*
   * LAB STEP 3c
   */
//   test_calibration();
  
  /*
   * LAB STEP 5
   * Initialize all variables
   */
  CURRENT_STATE = sDISP_COUNTDOWN;
  lxb = 0;
  uxb = 16;
  xyo random_xyo = random_location_orientation(lxb, uxb);
  x = random_xyo.x;
  y = random_xyo.y;
  o = random_xyo.o;
  level = 0;
  time_step = 1000;
  saved_clock = millis();
  countdown = 3;
  // display something on the LCD

//  test_all_tests();
}

void loop() {
  update_inputs();
  CURRENT_STATE = update_fsm(CURRENT_STATE, millis(), num_buttons_pressed, last_button_pressed);
  delay(10);
}

/*
 * LAB STEP 4
 */
bool at_edge(byte x, byte y, byte o, int lxb, int uxb) {
  switch (o){
    case UP:
      return y == 0;
      break;
    case DOWN:
      return y == 3;
      break;
    case LEFT:
      return x == lxb;
      break;
    case RIGHT:
      return x == uxb - 1;
      break;
    default:
      return false;
      break;
  }
  
}

/*
 * LAB STEP 4
 */
xy mv(byte x, byte y, byte o) {
  switch (o){
    case UP:
      y = y - 1;
      break;
    case DOWN:
      y = y + 1;
      break;
    case LEFT:
      x = x - 1;  
      break;
    case RIGHT:
      x = x + 1;
      break;
  }
      
  xy retval = {x, y};
  return retval;
}

/*
 * LAB STEP 7
 */
state update_fsm(state cur_state, long mils, int num_buttons, int last_button) {
  state next_state = cur_state;
  switch(cur_state) {
  case sDISP_COUNTDOWN:
    if ((mils - saved_clock) >= 500 and countdown >= 0) { // transition 1-1
      display_level(level, countdown);
      countdown -= 1;
      saved_clock = mils;
      next_state = sDISP_COUNTDOWN;
    }else if(((mils-saved_clock) >= 500) && countdown <= 0 && !at_edge(x, y, o, lxb, uxb)){ //transition 1-2
      display_cursor(x,y,o,false,lxb,uxb);
      saved_clock = mils;
      reset_buttons();
      next_state = sWAIT_AFTER_ROT;
    }else if(((mils-saved_clock) >= 500) && countdown <= 0 && at_edge(x,y,o,lxb,uxb)){ //transition 1-6
      display_cursor(x,y,o,false,lxb,uxb);
      saved_clock = mils;
      reset_buttons();
      next_state = sWAIT_FOR_BUT;
    }
    break;
  case sWAIT_AFTER_ROT:
    if ((mils - saved_clock) >= time_step and num_buttons > 0) { // transition 2-3 (a)
      display_cursor(x, y, o, false, lxb, uxb);
      lu shrunken = shrink_bounds(x, o, lxb, uxb);
      lxb = shrunken.l;
      uxb = shrunken.u;
      xy new_xy = mv(x, y, o);
      x = new_xy.x;
      y = new_xy.y;
      next_state = sMOV;
    } else if ((mils - saved_clock) >= time_step and num_buttons == 0) { // transition 2-3 (b)
      display_cursor(x, y, o, false, lxb, uxb);
      xy new_xy = mv(x, y, o);
      x = new_xy.x;
      y = new_xy.y;
      next_state = sMOV;
    } else {
      next_state = sWAIT_AFTER_ROT;
    }
    break;
  case sMOV:
    if (lxb < uxb){ //Transition 3-4
       display_cursor(x, y, o, true, lxb, uxb);
       saved_clock = mils;
       reset_buttons();
       next_state = sWAIT_AFTER_MOV;
    }else if(lxb == uxb) {// Transition 3-7
      display_game_over(level);
      next_state = sGAME_OVER;
    }
    break;
  case sWAIT_AFTER_MOV:
    if (mils - saved_clock >= time_step) {
      display_cursor(x,y,o,false,lxb,uxb);
      
      o = random_turn(o, lxb, uxb);
      if (num_buttons != 0) {
        lu new_bounds = shrink_bounds(x, o, lxb, uxb);
        lxb = new_bounds.l;
        uxb = new_bounds.u;
      }
  
      next_state = sROT;
    }
    break;
  case sROT:
    if (lxb == uxb) { 
      display_game_over(level);
      next_state = sGAME_OVER;
    } else {
      display_cursor(x,y,o,false,lxb,uxb);
      saved_clock = mils;
      reset_buttons();
      if (at_edge(x, y, o, lxb, uxb)) {
        next_state = sWAIT_FOR_BUT;
      } else {
        next_state = sWAIT_AFTER_ROT;
      }
    }
    break;
  case sWAIT_FOR_BUT:
    if (mils - saved_clock >= time_step){
      if (num_buttons == 1 && last_button == o) {
        display_level(level + 1, 3);
        countdown = 2;
        saved_clock = mils;
        level += 1;
        time_step = time_step * 0.95;
        xyo ret = random_location_orientation(lxb, uxb);
        x = ret.x;
        y = ret.y;
        o = ret.o;
        next_state = sDISP_COUNTDOWN;
      } else if ((num_buttons >= 1 && last_button != 0) || num_buttons == 0) {
        display_game_over(level);
        next_state = sGAME_OVER;
      }
  }
    break;
  case sGAME_OVER: // no transitions from state 7
    next_state = sGAME_OVER;
    break;
  }
  return next_state;
}
