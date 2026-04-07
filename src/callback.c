#include "menu.h"
#include "serial.h"

#include <unistd.h>
#include <fcntl.h>  
#include <stdint.h>

#define DEBUG 0

void callbackStartUpload(void)
{
	// assemble program.psm to program.hex (opbasm) - optional 
	// open program.hex
	// FILE* programFile = fopen("program.hex", "r");
	// open UART to FPGA
	int fd = open("/dev/ttyUSB1", O_RDWR);
	int returnValue = set_interface_attribs (fd, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
	if ( returnValue == -1)
	{
		mvprintw(1, 1, "set_interface_attribs error: tcgetattr");
		getch();
		return;
	} else if ( returnValue == -2 )
	{

		mvprintw(1, 1, "set_interface_attribs error: tcsetattr");
		getch();
		return;
	}

    set_blocking (fd, 1);                // set no blocking
    // write(fd, "a", 1);
	write(fd, "!!!", 3);
    uint32_t number = 0b00000000000000001001000000000000;
    write(fd, &number, 4);
#if DEBUG == 1
    mvprintw(1, 1, "First instruction sent!");
	getch();
#endif
	// write(fd, "%d%d%d%d", 0b00000000, 0b00000000, 0b10010000, 0b00000000, 4);
	number = 0b00000000000001101101000000000000;
    write(fd, &number, 4);
#if DEBUG == 1
	mvprintw(1, 1, "Second instruction sent");
	getch();
#endif
	// write(fd, "%d%d%d%d", 0b00000000, 0b00000110, 0b11010000, 0b00000000, 4);
	number = 0b00000000000010100010000000000000;
    write(fd, &number, 4);
#if DEBUG == 1
	mvprintw(1, 1, "Third instruction sent");
	getch();
#endif
	// write(fd, "%d%d%d%d", 0b00000000, 0b00001010, 0b00100000, 0b00000000, 4);
	write(fd, "!!!", 3);
	mvprintw(1, 1, "Upload finished!");
	getch();
	// send init sequence
	// send program
	// send termination sequence
	// the process wasn't started, exit the program with an error message...
	close(fd);
}


void callbackConfigSettings(void)
{
	exit_loop = 1;
}


/*
level_up - flag indicating should the cursor
		   go up one level regarding the menu level
		   Active on 1
*/
void callbackLevelUp(void)
{
	level_up = 1;
}


/*
exit_loop - if set true, this flag indicates the program should 
			be terminated
			Active on 1
*/
void callbackExit(void)
{
	exit_loop = 1;
}