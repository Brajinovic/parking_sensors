#ifndef IDLE_H
#define IDLE_H

#include "uart_handler.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <sys/time.h>
#include "draw.h"

#define TONE_ONE_DURATION 50
#define TONE_TWO_DURATION 250
#define TONE_THREE_DURATION 500

#define USE_PARKING_SENSOR 1
#define USE_AUDIO 1

// located in main.c
extern int* sensor_values;
extern Display *display_thing;
extern unsigned int keycode;
extern int fd;

void idle();
void press_key(int);

#endif