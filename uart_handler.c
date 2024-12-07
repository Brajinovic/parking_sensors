#include "uart_handler.h"


int set_interface_attribs (int fd, int speed, int parity)
{
	struct termios tty;
	if (tcgetattr (fd, &tty) != 0)
	{
			printf ("error %d from tcgetattr", errno);
			return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
									// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
									// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
			printf ("error %d from tcsetattr", errno);
			return -1;
	}
	return 0;
}


void set_blocking (int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
			printf ("error %d from tggetattr", errno);
			return;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
			printf ("error %d setting term attributes", errno);
}


int get_sensor_data(int* sensor_values, int fd)
{
	if (sensor_values == NULL)
	{
		return FAIL;
	}

	// temp variable to hold the sensor index
	char sensor_id[1];
	// temp variable to hold the sensor value
	char rx_buffer[10];

	int n = 0;

	for (int i = 0; i < NUMBER_OF_SENSORS; i++)
		{
			sprintf(sensor_id, "%d", i); // put the index of the sensor inside the variable sensor
			write (fd, sensor_id, 1); // ask for the data of the nth sensor
			
			// if ready to read, read the received message
			n = read (fd, rx_buffer, sizeof(rx_buffer));

			// convert the sensor value from string to int
			*(sensor_values + i) = atoi(rx_buffer); 
			// clean the buf in order to get rid of residual garbage
			memset(rx_buffer, 0, sizeof(rx_buffer));
		}

	return SUCCESS;
}


int config_uart(int* fd)
{
	char *portname = "/dev/ttyUSB0";

	*fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (*fd < 0)
	{
		printf ("error %d opening %s: %s", errno, portname, strerror (errno));
		return FAIL;
	}

	set_interface_attribs (*fd, B9600, 0);  // set speed to 9,600 bps, 8n1 (no parity)
	set_blocking (*fd, 0);                // set no blocking
	return SUCCESS;
}

/*
int main(void)
{
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
	
#if KEY_SIM == 1

	Display *display;
	unsigned int keycode;
	display = XOpenDisplay(NULL);

	keycode = XKeysymToKeycode(display, XK_Return);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);

	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);

	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);

#endif
	return SUCCESS;
}
*/