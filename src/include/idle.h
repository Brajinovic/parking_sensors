#ifndef IDLE_H
#define IDLE_H

#define USE_PARKING_SENSOR 0
#if USE_PARKING_SENSOR == 1
// deleted uart_handler.h as it is included in main.c, might cause problems later
	#include <X11/Xlib.h>
	#include <X11/keysym.h>
	#include <X11/extensions/XTest.h>
#else
	#define Display int
	#define True 1
	#define False 0
#endif

#include <sys/time.h>
#include "draw.h"

#define DEBUG_IDLE 0

#define TONE_ONE_DURATION 50
#define TONE_TWO_DURATION 250
#define TONE_THREE_DURATION 500

// meaning, using the HC-SR04 to simulate button press depending on the
// measured distance from given object
#define USE_PARKING_SENSOR 0

// play audio corresponding to the distance to the object
#define USE_AUDIO 1



void check_distance(struct rectangle* base_rectangle, Display *display_thing, unsigned int keycode, int* sensor_values);
void press_key(int key, Display *display_thing, unsigned int keycode);

#endif