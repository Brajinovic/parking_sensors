#include "keyboard_input_controller.h"


void check_pressed_buttons(unsigned char key, struct rectangle* base_rectangle)
{
	// in this function the key is being mapped to the correspoinding distance
	// each parking sensor has unique keys on which he gets activated
	// the specific keys are written in the base rectangle in the shape of struct keymap structure
	struct keymap* keys = base_rectangle->keys;

	if (key == keys->far_key)
	{
		// in case the close distance button has been pressed, set the order to 3
		base_rectangle->distance = FAR; 

	} else if (key == keys->middle_key)
	{
		// in case the middle distance button has been pressed, set the order to 2
		base_rectangle->distance = MIDDLE; 

	} else if (key == keys->close_key)	
	{
		// in case the close distance button has been pressed, set the order to 1
		base_rectangle->distance = CLOSE;

	} else if (key == keys->clear_key) 
	{
		// in case the clear button has been pressed, set the order to 4
		base_rectangle->distance = F_FAR;

	} 
}


int check_distance(struct rectangle* FL_base_rectangle, struct rectangle* FR_base_rectangle, struct rectangle* BL_base_rectangle, struct rectangle* BR_base_rectangle)
{
	// this function is responsible for selecting the audio 
	// which will be played in the application
	// all it does is, it compaires the distances for each of the
	// four base rectangles and returns the lowest value
	// the lowest value corresponds with the object beeing
	// closest to the vehicle
	int current_distance = BASE_RECTANGLE_DEFAULT_DISTANCE;

	if (current_distance > FL_base_rectangle->distance)
	{
		current_distance = FL_base_rectangle->distance;
	}
	if (current_distance > FR_base_rectangle->distance)
	{
		current_distance = FR_base_rectangle->distance;
	}
	if (current_distance > BL_base_rectangle->distance)
	{
		current_distance = BL_base_rectangle->distance;
	}
	if (current_distance > BR_base_rectangle->distance)
	{
		current_distance = BR_base_rectangle->distance;
	}

	return current_distance;
}