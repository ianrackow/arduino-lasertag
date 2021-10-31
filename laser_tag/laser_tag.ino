#include "laser_tag.h"

//Global game variables
int shot_delay = 400;
int shot_duration = 100;
int cooldown_period = 10000;
int game_duration = 300000;
int vest_threshold = 0; //We need to calibrate this

// FSM variables
int deaths = 0;
int game_start_timestamp = 0;
int saved_clock = 0;
int countdown = 5000;
server_packet received_packet = GAME_IDLE;

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


state update_fsm(state cur_state, long mils, int num_buttons, int last_button) {
  state next_state = cur_state;
  switch(cur_state) {
    case sWAITING_FOR_GAME:
      if (received_packet == GAME_START){ //Transition from 1-2
        make_sound(GAME_STARTING);
        set_vest_lights(ON);
        game_start_timestep = mils;
        next_state = sNEUTRAL;
      }else{
        next_state = sWAITING_FOR_GAME;
      }
      break;
    case sNEUTRAL:
     if( (mils - game_start_timestamp) >= game_duration){ //Transition 2-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      }else if (trigger_pressed == 1 && sensor_value < vest_threshold){ //Transition 2-3
        set_laser(HIGH);
        make_sound(PEW);
        saved_clock = mils;
        next_state = sFIRING;
      }else if( sensor_value >= vest_threshold){ //Transition from 2-4
        set_vest_lights(OFF);
        report_hit();
        saved_clock = mils;
        deaths = deaths + 1;
        next_state = sHIT;
      }else{
        next_state = sNEUTRAL;
      }
      break;
    case sJUST_FIRED:
      if( (mils - game_start_timestamp) >= game_duration){ //Transition from 3-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      else if ( (mils - saved_clock) >= shot_duration){ // Transition from 3-6
        set_laser(LOW);
        saved_clock = mils;
        next_state = sGUN_COOLDOWN;
      }else if(sensor_value >= vest_threshold){ //Transition from 3-4
        set_laser(LOW);
        set_vest_lights(OFF);
        make_sound(HIT);
        report_hit();
        saved_clock = mils;
        next_state = sHIT;
      }else{
        next_state = sJUST_FIRED;
      }
      break;
    case sHIT:
     if( (mils - game_start_timestamp_ >= game_duration){ //Transition 4-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      else if( (mils - saved_clock) >= cooldown_period){ //Transition from 4-2
        make_sound(REVIVED);
        set_vest_lights(ON);
        next_state = sNEUTRAL;
      }else{
        next_state = sHIT;
      }
      break;
    case sGAME_OVER:
      //There is no other state to transition to
      next_state = sGAME_OVER;
      break;
    case sGUN_COOLDOWN:
      if( (mils - game_start_timestamp) >= game_duration){ //Transition from 6-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      else if ( (mils - saved_clock) >= shot_delay && sensor_value < vest_threshold) { //Transition 6-2
        next_state = sNEUTRAL;
      }else if(sensor_value >= vest_threshold){ //Transition 6-4
        set_vest_lights(OFF);
        make_sound(HIT);
        report_hit();
        deaths = deaths + 1;
        saved_clock = mils;
        next_state = sHIT;
      }else{
        next_state = sGUN_COOLDOWN;
      }
      break;
  }
  return next_state;
}
