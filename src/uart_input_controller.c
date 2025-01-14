#include "uart_input_controller.h"

#if USE_PARKING_SENSOR == 1

	void press_key(int key, Display *display_thing, unsigned int keycode)
	{
		keycode = XKeysymToKeycode(display_thing, key);
		XTestFakeKeyEvent(display_thing, keycode, True, 0);
		XTestFakeKeyEvent(display_thing, keycode, False, 0);
		XFlush(display_thing);
	}

#endif

void check_distance(struct rectangle* base_rectangle, Display *display_thing, unsigned int keycode, int* sensor_values)
{

// start of parking sensor logic
// I am using the HC-SR04 ultrasonic sensors...
#if USE_PARKING_SENSOR == 1

	if (*(sensor_values + 0) < 31)	// if the distance is less than 30 cm, that is state 1
	{
		// press the key only if there was a state change
		if (base_rectangle->distance != 1)
		{
			press_key(XK_e, display_thing, keycode);
		} else
		{

		}
		
	}else if (*(sensor_values + 0) < 61)	// if the distance is less than 60 cm, that is state 2
	{
		if (base_rectangle->distance != 2)
		{
			press_key(XK_w, display_thing, keycode);
		} else
		{

		}
	} else if (*(sensor_values + 0) < 101 )		// if the distance is less than 100 cm, that is state 3
	{
		if (base_rectangle->distance != 3)
		{
			press_key(XK_q, display_thing, keycode);
		} else
		{

		}
	} else if (*(sensor_values + 0) > 100)
	{
		if (base_rectangle->distance != 4)
		{
			press_key(XK_r, display_thing, keycode);
		} else
		{

		}
	}
#endif
}
