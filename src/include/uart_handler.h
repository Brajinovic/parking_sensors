#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <stdlib.h>

#define NUMBER_OF_SENSORS 3
#define SUCCESS 1
#define FAIL 0

#define KEY_SIM 0

#define UART_DEBUG 0

// generic UART 
int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);


/*
@param sensor_values is supposed to be the array where the sensor values are to be stored
@param id is the value you get from config_uart()
*/
int get_sensor_data(int* sensor_values, int fd);

/*
@param *fd is used to return the value of the open command which is used to open the UART
communication port. I do not know why it is this important, but if I just set it to 0,
the rest of the code does not work. This is a TODO, to see why it is important.
*/
int config_uart(int* fd);



/*
///////////////////////////////////////////////////////////////////////////////////////////////////////
// example of use

int fd;

config_uart(&fd);

int* sensor_values = (int*)calloc(sizeof(int), NUMBER_OF_SENSORS);

while (1)
{
	if (get_sensor_data(sensor_values, fd) == 0)
	{
		printf("Error when reading from UART!");
	}

#if UART_DEBUG == 1
	printf("Sensor values: %d\t%d\t%d\n", sensor_values[0], sensor_values[1], sensor_values[2]);
#endif
	sleep(0.5);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

OUTPUT: 
Sensor values: 372	6	369
Sensor values: 372	7	368
Sensor values: 371	5	352
Sensor values: 371	9	352
Sensor values: 371	9	352
Sensor values: 371	9	352
Sensor values: 371	9	352
Sensor values: 371	9	352
Sensor values: 371	9	352
Sensor values: 372	9	352
...

*/
#endif