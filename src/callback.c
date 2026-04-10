#include "menu.h"
#include "serial.h"

#include <unistd.h>
#include <fcntl.h>  
#include <stdint.h>

#define DEBUG 0
#define UART_EN 0

void callbackStartUpload(void)
{
#if UART_EN == 1
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
    
    write(fd, "!!!", 3);
#endif

	// assemble program.psm to program.hex (opbasm) - optional 
	// open program.hex
	// read binary file
	FILE* programFile = fopen("./src/program.hex", "rb");
	if (programFile == NULL)
        exit(EXIT_FAILURE);
    uint64_t command = 0x0000000000000000;
    uint8_t character = 0x00;
    uint8_t	hex = 0x00;
    uint8_t offset = 2;
	mvprintw(1, 1, "  							");
	for (int i = 0; i < 2048; i++)
	{
		// 5 chars per row and \n
		command = i;
		// only for the first char, offset should be 2
#if DEBUG == 1
		mvprintw(2, 1, "Command: i: %d %lx", i, command);
		getch();
#endif
		command = command << offset;
#if DEBUG == 1
		mvprintw(2, 1, "Command: %lx", command);
		getch();
#endif
		// for the rest, the offset is 4
		offset = 4;
		for( int j = 0; j < 6; j++)
		{
			fread(&character, 1, 1, programFile);
			// if \n is read, skip the processing
			if (j == 5)
				continue;
			// if it is in range 0 - 9
			
			if (character > 0x2F && character < 0x3A)
			{
				hex = character & 0x0F;
			}
			// if the char is in range A(10) - F(15)
			else if (character > 0x40 && 0x47)
			{
				hex = character & 0x0F;
				// 2nd complement to reduce the number by 1
				hex = hex | 0x0D;
			}
			command = command | hex;
#if DEBUG == 1
			mvprintw(1, 1, "Char %d: %x", j, hex);
			mvprintw(2, 1, "Command row: %d: %lx", j, command);
			getch();
#endif
			// on the last char, there should be no offset
			if (j != 4)
				command = command << offset;
			
		}
#if DEBUG == 1
		mvprintw(2, 1, "Final Command: %d", command);
		getch();
#endif
		offset = 2;

#if UART_EN == 1
		write(fd, &command, 4);
#endif
		// write...
	}

/*#if UART_EN == 1
	
    // write(fd, "a", 1);
	
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
	// send init sequence
	// send program
	// send termination sequence
	// the process wasn't started, exit the program with an error message...
	
#endif

*/
#if UART_EN == 1
	write(fd, "!!!", 3);
	mvprintw(1, 1, "Upload finished!");
	getch();
	
	close(fd);
#endif
	fclose(programFile);
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