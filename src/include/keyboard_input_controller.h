#ifndef KEYBOARD_INPUT_CONTROLLER_H
#define KEYBOARD_INPUT_CONTROLLER_H

#include "draw.h"
#include "constants.h"

int check_state(struct rectangle*, struct rectangle*, struct rectangle*, struct rectangle*);
void check_pressed_buttons(unsigned char key, struct rectangle* base_rectangle);


#endif