Repository architecture:
	- main.c
		- The main file. From this file, all other files are used.

	- draw.c
		- this file contains the code used for drawing objects using Glut

	- idle.c
		- user input processing logic
		- HC-SR04 data processing logic
		- tone generation logic (w/ printf(\a))

	- uart_handler.c
		- UART interface handler (setup, read data)