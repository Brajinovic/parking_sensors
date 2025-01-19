#ifndef UART_INPUT_CONTROLLER_H
#define UART_INPUT_CONTROLLER_H

#define USE_PARKING_SENSOR 0

#define DEBUG_IDLE 0

#define TONE_ONE_DURATION 50
#define TONE_TWO_DURATION 250
#define TONE_THREE_DURATION 500

// meaning, using the HC-SR04 to simulate button press depending on the
// measured distance from given object
// 1 - using the HC-SR04 sensors
// 0 - not using them
#define USE_PARKING_SENSOR 0

// play audio corresponding to the distance to the object
// 1 - audio enabled
// 0 - audio disabled
#define USE_MP3 1


// only if I am using the HC sensors
// include the following libraries
#if USE_PARKING_SENSOR == 1
	#include <X11/Xlib.h>
	#include <X11/keysym.h>
	#include <X11/extensions/XTest.h>
// otherwise, define these constants as placeholders
#else
	#define Display int
	#define True 1
	#define False 0
#endif

#if USE_MP3 == 1
	#include <alsa/asoundlib.h>
#endif


#include "draw.h"
#include "constants.h"


void check_distance_HC(struct rectangle* base_rectangle, Display *display_thing, unsigned int keycode, int* sensor_values);
void press_key(int key, Display *display_thing, unsigned int keycode);

#endif