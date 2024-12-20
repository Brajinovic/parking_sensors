#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#define NUMBER_OF_SENSORS 3
#define SUCCESS 0
#define FAIL -1

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

/*
int main(void)
{

	char *portname = "/dev/ttyUSB0";

	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
		printf ("error %d opening %s: %s", errno, portname, strerror (errno));
		return FAIL;
	}

	set_interface_attribs (fd, B9600, 0);  // set speed to 9,600 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking

	char buf [10];
	int n = 0;
	char sensor[1];
	while (1)
	{
		
		
		for (int i = 0; i < NUMBER_OF_SENSORS; i++)
		{
			sprintf(sensor, "%d", i);
			write (fd, sensor, 1);
			// sleep(0.01);
			n = read (fd, buf, sizeof(buf));  // read up to 100 characters if ready to read
			printf("%d: %s", i, buf);
			memset(buf, 0, sizeof(buf));
		}
		//printf("\n");
			
		sleep(0.5);
	}
	return SUCCESS;
}
*/