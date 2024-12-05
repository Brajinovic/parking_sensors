#include "idle.h"


void press_key(int key)
{
	keycode = XKeysymToKeycode(display_thing, key);
	XTestFakeKeyEvent(display_thing, keycode, True, 0);
	XTestFakeKeyEvent(display_thing, keycode, False, 0);
	XFlush(display_thing); 
}


void check_distance(struct rectangle* base_rectangle)
{
#if DEBUG_IDLE == 1
	printf("\nDistance of Sensor 1: %d\n", *(sensor_values + 0));
#endif
	// now interpret the received values
	if (*(sensor_values + 0) < 31)	// if the distance is less than 30 cm, that is state 3
	{
		if (base_rectangle->order != 3)
		{

			printf("\n< 31\n");
			press_key(XK_e);
		} else
		{

		}
		
	}else if (*(sensor_values + 0) < 61)	// if the distance is less than 60 cm, that is state 2
	{
		if (base_rectangle->order != 2)
		{

			printf("\n< 61\n");
			press_key(XK_w);
		} else
		{

		}
	} else if (*(sensor_values + 0) < 101 )		// if the distance is less than 100 cm, that is state 1	
	{
		// only press the button once, do not do it repeatedly
		// if the distance is less than 100 cm, press q, but in the
		// next iteration, if the base_rectangle->order is already 1, it means
		// that the button was already pressed
		if (base_rectangle->order != 1)
		{

			printf("\n< 101\n");
			press_key(XK_q);
		} else
		{

		}
	} else if (*(sensor_values + 0) > 100)
	{
		if (base_rectangle->order != 4)
		{
			printf("\n > 100\n");
			press_key(XK_r);
		} else
		{

		}
	}
#if USE_AUDIO == 1
	
	int factor = 0;
	struct timeval time;
	int current_time = 0;
	static int previous_time = 0;
	
	gettimeofday(&time, NULL);

	// get the microsecond part of the time and divide it by 1000 to get miliseconds
	current_time = time.tv_usec / 1000;

#if DEBUG_IDLE == 1
	printf("\n%d\n", current_time);
#endif

	// calculate how mutch time has passed since the last time this function was called
	factor = current_time - previous_time;
	// given that the result can be negative, you need to check if it is negative and turn it to positive
	factor = factor > 0 ? factor : factor * -1;

#if DEBUG_IDLE == 1
	printf("\nBase rectangle order: %d\n", base_rectangle->order);
#endif
	printf(" ");
	// with the help of struct rectangle member order, determine which tone is playing
	// i.e. what should be the period of the audio sound
	if (base_rectangle->order == 3){
		
		if (factor > TONE_ONE_DURATION) // play a sound every 100 ms
		{
			printf("\a");
			previous_time = current_time;
		} else 
		{

		}
	} else if (base_rectangle->order == 2)
	{
		if (factor > TONE_TWO_DURATION) // play a sound every 300 ms
		{
			printf("\a");
			previous_time = current_time;
		} else
		{

		}
	} else if (base_rectangle->order == 1)
	{
		if (factor > TONE_THREE_DURATION) // play a sound every 500 ms
		{
			printf("\a");
			previous_time = current_time;
		} else
		{

		}
	} else
	{	
	}
#endif
}


void idle()
{
#if USE_PARKING_SENSOR == 1
`	// read the parking sensor values
	if (get_sensor_data(sensor_values, fd) == 0)
	{
		printf("Error when reading from UART!");
	}
	check_distance(FL_base_rectangle);

#endif
}