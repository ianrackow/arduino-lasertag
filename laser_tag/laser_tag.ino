#include "laser_tag.h"

#include <Arduino.h>

#include "laser_tag_tests.h"
#include "laser_tag_utils.h"

#define TESTING

// Global game variables
int shot_duration = 500;
int shot_cooldown = 200;
int hit_cooldown = 5000;
int game_duration = 300000;
int poll_game_start_interval = 2000;
int vest_threshold = 500;

// FSM variables
int deaths;
int game_start_time;
int saved_clock;
int curr_time;

// FSM inputs
int trigger_pressed;
int sensor_value;

state CURRENT_STATE;

// Sound
SimpleTimer sound_player;

// ########### WIFI CODE ############
//#include <HttpClient.h>
#include <NTPClient.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;

WiFiClient client;
// HttpClient http(client);
NTPClient timeClient(ntpUDP);

byte mac[6];
char player_id[18];

char server_url[] = "http://2030-138-16-110-48.ngrok.io";  // URL for our server
char host_name[] = "2030-138-16-110-48.ngrok.io";
// char server_url[] = "http://104.131.46.88/";  // URL for our server

char ssid[] = "Brown-Guest";  // network SSID (name)
char pass[] = "";             // for networks that require a password
// char ssid[] = "29 CREIGHTON - 1";
// char pass[] = "R3m0t3L3@rn1ng!";

// Attempt to connect to WiFi network.
// Returns true on success and false on failure.
bool attempt_connect() {
  int status = WiFi.begin(ssid);  // WiFi.begin(ssid, pass) for password
  Serial.println(status);
  return status == WL_CONNECTED;
}

// Connects to the WiFi network specified by ssid and sets player_id to the
// Arduino's MAC address.
void setup_wifi() {
  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to: ");
  Serial.println(ssid);
  while (!attempt_connect()) {
    Serial.println("Trying again");
    delay(5000);
  }
  WiFi.macAddress(mac);
  sprintf(player_id, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(player_id);
}

bool connect_to_webpage() {
  if (client.connect(server_url, 80)) {
    Serial.println("player_id: " + String(player_id));
    client.println("GET /api/score/hit?id=" + String(player_id) + " HTTP/1.0");
    client.println("Host: " + String(host_name));
    client.println();
    return true;
  } else {
    Serial.println("Failed to fetch webpage");
    return false;
  }
}

bool register_for_game() {
  // http.get(server_url, "/api/score/register");
  if (client.connect(server_url, 80)) {
    String response = "";
    Serial.println("player_id: " + String(player_id));
    client.println("GET /api/score/register?id=" + String(player_id) + " HTTP/1.0");
    client.println("Host: " + String(host_name));
    client.println();

    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        response += c;
      }
    }

    // the server's disconnected, stop the client:
    client.stop();

    response = response.substring(response.length() - 2);

    return response.equals("OK");

  } else {
    Serial.println("Failed to fetch webpage");
    return false;
  }
}

int get_start_time() {
#ifdef TESTING
  return 100;
#else
  String response = "";
  if (client.connect(server_url, 80)) {
    // Serial.println("player_id: " + String(player_id));
    client.println("GET /api/score/start HTTP/1.0");
    client.println("Host: " + String(host_name));
    client.println();

    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        response += c;
        if (c == '\n') {
          response = "";
        }
      }
    }

    // the server's disconnected, stop the client:
    client.stop();

    Serial.println(response);
    return response.toInt();
  } else {
    Serial.println("Failed to fetch webpage");
    return 0;
  }
#endif
}

int get_current_time() {
#ifdef TESTING
  return curr_time++;
#else
  timeClient.update();
  return timeClient.getEpochTime();
#endif
}

// ##################################

void setup() {
  // Serial.begin(9600);
  // while (!Serial)
  //   ;

  setup_wifi();
  initialize_system();

  calibrate();

  // test_calibration();

  CURRENT_STATE = sGAME_NOT_STARTED;
  set_vest_lights(ON);
  saved_clock = millis();
  game_start_time = 0;
  curr_time = 0;


  #ifndef TESTING
  Serial.println("Trying to register for game!");
  while (!register_for_game()) {
    Serial.println("Trying again");
    delay(1000);
  }
  Serial.println("Registration successful");
  #endif
  

  // Watchdog configuration
  NVIC_DisableIRQ(WDT_IRQn);
  NVIC_ClearPendingIRQ(WDT_IRQn);
  NVIC_SetPriority(WDT_IRQn, 0);
  NVIC_EnableIRQ(WDT_IRQn);

  // Configure and enable WDT GCLK:
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(4) | GCLK_GENDIV_ID(5);
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;
  // set GCLK->GENCTRL.reg and GCLK->CLKCTRL.reg
  GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_ID(5) | GCLK_GENCTRL_DIVSEL;
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(5) | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_ID_WDT;

  // Configure and enable WDT:
  // WDT->CONFIG.reg, WDT->EWCTRL.reg, WDT->CTRL.reg

  WDT->CONFIG.reg = 0x9;
  WDT->EWCTRL.reg = 0x8;
  WDT->CTRL.reg = WDT_CTRL_ENABLE;
  while (WDT->STATUS.bit.SYNCBUSY)
    ;

#ifdef TESTING
  test_all_tests();
#endif
}

void loop() {
#ifndef TESTING
  WDT->CLEAR.reg = 0xA5;
  update_inputs();
  WDT->CLEAR.reg = 0xA5;
  CURRENT_STATE = update_fsm(CURRENT_STATE, millis(), trigger_pressed, sensor_value);
  WDT->CLEAR.reg = 0xA5;
  sound_player.run();
  delay(12);
#endif
}

state update_fsm(state cur_state, long mils, int trigger_pressed, int sensor_value) {
  state next_state = cur_state;
  Serial.println(cur_state);
  switch (cur_state) {
    case sGAME_NOT_STARTED:
      if (game_start_time > 0) {  // Transition from 0-1
        Serial.println("Server gave us a start timestamp! :");
        Serial.println(game_start_time);
        curr_time = get_current_time();
        next_state = sCOUNTDOWN_TILL_START;
      } else if ((mils - saved_clock) >= poll_game_start_interval) {
        game_start_time = get_start_time();
        Serial.println(game_start_time);
        saved_clock = mils;
        next_state = sGAME_NOT_STARTED;  // Transition from 0-0
      } else {
        next_state = sGAME_NOT_STARTED;  // Transition from 0-0
      }
      break;
    case sCOUNTDOWN_TILL_START: {
      if (curr_time >= game_start_time) {  // Transition from 1-2
        Serial.println("Game started!");
        make_sound(GAME_STARTING);
        set_vest_lights(ON);
        game_start_time = mils;
        next_state = sNEUTRAL;
      } else {  // Transition from 1-1
        curr_time = get_current_time();
        next_state = sCOUNTDOWN_TILL_START;
      }
      break;
    }
    case sNEUTRAL:
      if ((mils - game_start_time) >= game_duration) {  // Transition 2-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      } else if (sensor_value >= vest_threshold) {  // Transition from 2-4
        set_laser(LOW);
        set_vest_lights(OFF);
        make_sound(HIT);
        report_hit();
        saved_clock = mils;
        deaths = deaths + 1;
        next_state = sHIT;
      } else if (trigger_pressed == 1) {  // Transition 2-3
        set_laser(HIGH);
        make_sound(PEW);
        saved_clock = mils;
        next_state = sJUST_FIRED;
      } else {
        next_state = sNEUTRAL;
      }
      break;
    case sJUST_FIRED:
      if ((mils - game_start_time) >= game_duration) {  // Transition from 3-5
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
      if ((mils - game_start_time) >= game_duration) {  // Transition 4-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      } else if ((mils - saved_clock) >= hit_cooldown) {  // Transition from 4-2
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
      if ((mils - game_start_time) >= game_duration) {  // Transition from 6-5
        make_sound(GAME_OVER);
        set_vest_lights(OFF);
        next_state = sGAME_OVER;
      } else if (sensor_value >= vest_threshold) {  // Transition 6-4
        set_laser(LOW);
        set_vest_lights(OFF);
        make_sound(HIT);
        report_hit();
        deaths = deaths + 1;
        saved_clock = mils;
        next_state = sHIT;
      } else if ((mils - saved_clock) >= shot_cooldown) {  // Transition 6-2
        set_vest_lights(ON);
        next_state = sNEUTRAL;
      } else {
        next_state = sGUN_COOLDOWN;
      }
      break;
  }
  return next_state;
}
