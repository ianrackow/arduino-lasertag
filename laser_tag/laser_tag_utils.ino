// PIN ASSIGNMENTS

int LASER = 3;
int TRIGGER = 5;
int LIGHT_SENSOR = A1;
int VEST_LIGHTS = 7;




/*
 * Initialize system: set random, initialize arrow characters, set up LCD
 */
void initialize_system() {

  pinMode(LASER, OUTPUT);
  pinMode(VEST_LIGHTS, OUTPUT);
  pinMode(LIGHT_SENSOR, INPUT);
  pinMode(TRIGGER, INPUT);

}

/*
 * Display "CALIBRATING" as a scroll on the LCD
 * Display capacitive sensor readings from pins 7-10 in serial monitor
 */
void calibrate() {
}

/*
 * Display which sensor has been pressed on the gamepad, assuming labels[] and thresholds[]
 * have been set up correctly
 */
void test_calibration() {

}

/*
 * Read in gamepad inputs and update num_buttons_pressed and last_button_pressed
 * We only increase "num_buttons_pressed" if the current button pressed is greater than the previous button pressed
 */
void update_inputs() {

  trigger_pressed = digitalRead(TRIGGER);
  sensor_value = analogRead(LIGHT_SENSOR);  

}

void set_vest_lights(bool level){
  if (level){
    digitalWrite(VEST_LIGHTS, HIGH);
  } else {
    digitalWrite(VEST_LIGHTS, LOW);
  }
}
void set_laser(bool level){
  if (level){
    digitalWrite(LASER, HIGH);
  } else {
    digitalWrite(LASER, LOW);
  }
}
