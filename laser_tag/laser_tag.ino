#include "laser_tag.h"

// Global game variables
int shot_delay = 400;
int shot_duration = 100;
int cooldown_period = 10000;
int game_duration = 300000;
int vest_threshold = 500;  // We need to calibrate this

// FSM variables
int deaths = 0;
int game_start_timestamp = 0;
int saved_clock = 0;
int countdown = 5000;
server_packet received_packet = GAME_IDLE;

state CURRENT_STATE;

// ########### WIFI CODE ############
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;

WiFiClient client;
NTPClient timeClient(ntpUDP);

byte mac[6];
char player_id[18];

// char server_url[] = "67b8-192-91-235-243.ngrok.io";  // URL for our server
char server_url[] = "http://104.131.46.88/";  // URL for our server
//char ssid[] = "Brown-Guest";                  // network SSID (name)
//char pass[] = "";                             // for networks that require a password
char ssid[] = "29 CREIGHTON - 1";  
char pass[] = "R3m0t3L3@rn1ng!";
int status = WL_IDLE_STATUS;                  // the WiFi radio's status

void setup_wifi() {
  WiFi.macAddress(mac);
  sprintf(player_id, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(player_id);
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
    Serial.println(pass);
    status = WiFi.begin(ssid, pass);  // WiFi.begin(ssid, pass) for password
    Serial.println(WiFi.status());
    delay(10000);
  }
}

bool connect_to_webpage() {
  if (client.connect(server_url, 80)) {
    Serial.println("player_id: " + String(player_id));
    client.println("GET /api/score/hit?id=" + String(player_id) + " HTTP/1.0");
    client.println("Host: " + String(server_url));
    client.println();
    return true;
  } else {
    Serial.println("Failed to fetch webpage");
    return false;
  }
}
// ##################################

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  setup_wifi();
  initialize_system();

  //  calibrate();

  //   test_calibration();

  CURRENT_STATE = sNEUTRAL;
  set_vest_lights(ON);
  saved_clock = millis();
  game_start_timestamp = saved_clock;

  #ifdef TESTING
  test_all_tests();
  #endif
}

void loop() {
  #ifndef TESTING
  update_inputs();
  CURRENT_STATE = update_fsm(CURRENT_STATE, millis(), trigger_pressed, sensor_value, received_packet);
  Serial.println(CURRENT_STATE);
  delay(500);
  #endif
}

state update_fsm(state cur_state, long mils, int trigger_pressed, int sensor_value, server_packet received_packet) {
  state next_state = cur_state;
  switch (cur_state) {
    case sWAITING_FOR_GAME:
      if (received_packet == GAME_START) {  // Transition from 1-2
        make_sound(GAME_STARTING);
        set_vest_lights(ON);
        game_start_timestamp = mils;
        next_state = sNEUTRAL;
      } else {
        next_state = sWAITING_FOR_GAME;
      }
      break;
    case sNEUTRAL:
      if ((mils - game_start_timestamp) >= game_duration) {  // Transition 2-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      } else if (trigger_pressed == 1 && sensor_value < vest_threshold) {  // Transition 2-3
        set_laser(HIGH);
        make_sound(PEW);
        saved_clock = mils;
        next_state = sJUST_FIRED;
      } else if (sensor_value >= vest_threshold) {  // Transition from 2-4
        set_vest_lights(OFF);
        report_hit();
        saved_clock = mils;
        deaths = deaths + 1;
        next_state = sHIT;
      } else {
        next_state = sNEUTRAL;
      }
      break;
    case sJUST_FIRED:
      if ((mils - game_start_timestamp) >= game_duration) {  // Transition from 3-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      } else if (sensor_value >= vest_threshold) {  // Transition from 3-4
        set_laser(LOW);
        set_vest_lights(OFF);
        make_sound(HIT);
        report_hit();
        saved_clock = mils;
        deaths = deaths + 1;
        next_state = sHIT;
      } else if ((mils - saved_clock) >= shot_duration) {  // Transition from 3-6
        set_laser(LOW);
        saved_clock = mils;
        next_state = sGUN_COOLDOWN;
      } else {
        next_state = sJUST_FIRED;
      }
      break;
    case sHIT:
      if ((mils - game_start_timestamp) >= game_duration) {  // Transition 4-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      } else if ((mils - saved_clock) >= cooldown_period) {  // Transition from 4-2
        make_sound(REVIVED);
        set_vest_lights(ON);
        next_state = sNEUTRAL;
      } else {
        next_state = sHIT;
      }
      break;
    case sGAME_OVER:
      // There is no other state to transition to
      next_state = sGAME_OVER;
      break;
    case sGUN_COOLDOWN:
      if ((mils - game_start_timestamp) >= game_duration) {  // Transition from 6-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      } else if ((mils - saved_clock) >= shot_delay && sensor_value < vest_threshold) {  // Transition 6-2
        set_vest_lights(ON);
        next_state = sNEUTRAL;
      } else if (sensor_value >= vest_threshold) {  // Transition 6-4
        set_vest_lights(OFF);
        make_sound(HIT);
        report_hit();
        deaths = deaths + 1;
        saved_clock = mils;
        next_state = sHIT;
      } else {
        next_state = sGUN_COOLDOWN;
      }
      break;
  }
  return next_state;
}
