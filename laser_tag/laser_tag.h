#include "global.h"

state update_fsm(state cur_state, long mils, int trigger_pressed, int sensor_value, server_packet received_packet);

bool connect_to_webpage();
