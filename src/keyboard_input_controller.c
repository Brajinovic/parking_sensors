#include "keyboard_input_controller.h"


void check_pressed_buttons(unsigned char key, struct rectangle* base_rectangle)
{
	// this is a final state machiene where I am checking how far away the obsticale is
	// and writing that information into the distance attribute of the base rectangle
	// later I am going to use this information in order to draw the proper active rectangle
	// (this is one part of my final state machine)


	// each parking sensor has unique keys on which he gets activated
	// the specific keys are written in the base rectangle in the shape of struct keymap structure
	struct keymap* keys = base_rectangle->keys;

	if (key == keys->far_key)
	{
		base_rectangle->distance = 3; // in case the close distance button has been pressed, set the order to 3

	} else if (key == keys->middle_key)
	{
		base_rectangle->distance = 2; // in case the middle distance button has been pressed, set the order to 2

	} else if (key == keys->close_key)	// in case the close distance button has been pressed, set the order to 1
	{
		base_rectangle->distance = 1;

	} else if (key == keys->clear_key) // in case the clear button has been pressed, set the order to 4
	{
		base_rectangle->distance = 4;

	} 
}


int check_state(struct rectangle* FL_base_rectangle, struct rectangle* FR_base_rectangle, struct rectangle* BL_base_rectangle, struct rectangle* BR_base_rectangle)
{
	int current_state = BASE_RECTANGLE_DEFAULT_DISTANCE;

	if (current_state > FL_base_rectangle->distance)
	{
		current_state = FL_base_rectangle->distance;
	}
	if (current_state > FR_base_rectangle->distance)
	{
		current_state = FR_base_rectangle->distance;
	}
	if (current_state > BL_base_rectangle->distance)
	{
		current_state = BL_base_rectangle->distance;
	}
	if (current_state > BR_base_rectangle->distance)
	{
		current_state = BR_base_rectangle->distance;
	}

	return current_state;
}