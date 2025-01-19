#ifndef CONSTANTS_H
#define CONSTANTS_H

// ################################################################
//						GENERAL AREA
// boolean values
#define TRUE 1
#define FALSE 0

// define return constants
#define SUCCESS 1
#define FAIL 0

// define default datatype values
#define DEFAULT_INT 0
#define DEFAULT_PTR NULL
#define DEFAULT_FLOAT 0.0

// GLUT
#define N_OF_TEXTURES 1
#define TEXTURE_OFFSET_1 75
#define TEXTURE_OFFSET_2 25
// ##################################################################
// 						WINDOW PROPERTIES

// define origin coordinates
#define START_COORDINATE_X 0
#define START_COORDINATE_Y 0

// define window size
#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

// define window position on screen
#define WINDOW_X 545
#define WINDOW_Y 180

// define depth
#define MIN_HEIGHT -4
#define MAX_HEIGHT 2
#define BASE_LEVEL 0

// image settings
#define IMAGE_BUFFER_SIZE 128

// ###############################################################
// 						AUDIO SETTINGS
// number of samples (numbers) to be read from the audio file
#define PCM_INSTANCE_ID 1
#define SAMPLE_SIZE 9600
#define CHANNEL_COUNT 1 // number of channels to be used in playback
#define PERIOD 100000
#define NUMBER_OF_PERIODS 1

// different sample rates for playback
// by changing the sample rate you get the illusion of 
// variable speed
#define SLOW_RATE 16000
#define MEDIUM_RATE 32000
#define FAST_RATE 48000

// ###############################################################
// 						BASE RECTANGLE SETTINGS

#define BASE_RECTANGLE_WIDTH 45
#define BASE_RECTANGLE_HEIGHT 17

#define BASE_RECTANGLE_DEFAULT_DISTANCE 4

#define BASE_RECTANGLE_RED 1
#define BASE_RECTANGLE_GREEN 0
#define BASE_RECTANGLE_BLUE 0
#define BASE_RECTANGLE_ALPHA 1


#define FRAME_TRANSPARENCY 10

// base rectangle distance constants
#define CLOSE 1
#define MIDDLE 2
#define FAR 3
#define F_FAR 4

// base rectangle individual constants

// Front Right parking sensor
#define FR_X 129 			// x coordinate
#define FR_Y -318 			// y coordinate
#define FR_ROTATION 126 	// rotation angle
#define FR_FAR_KEY 113 		// q
#define FR_MIDDLE_KEY 119 	// w
#define FR_CLOSE_KEY 101 	// e
#define FR_F_FAR_KEY 114 	// r

// Front Left parking sensor
#define FL_X 405 			// x coordinate
#define FL_Y 105 			// y coordinate
#define FL_ROTATION 54 		// rotation angle
#define FL_FAR_KEY 97 		// a
#define FL_MIDDLE_KEY 115 	// s
#define FL_CLOSE_KEY 100 	// d
#define FL_F_FAR_KEY 102 	// f

// Back Right parking sensor
#define BR_X -610 			// x coordinate
#define BR_Y 260 			// y coordinate
#define BR_ROTATION 233 	// rotation angle
#define BR_FAR_KEY 122 		// z
#define BR_MIDDLE_KEY 117 	// u
#define BR_CLOSE_KEY 105 	// i
#define BR_F_FAR_KEY 111 	// o

// Back Left parking sensor
#define BL_X 25 			// x coordinate
#define BL_Y 693 			// y coordinate
#define BL_ROTATION 310		// rotation angle
#define BL_FAR_KEY 104 		// h
#define BL_MIDDLE_KEY 106 	// j
#define BL_CLOSE_KEY 107 	// k
#define BL_F_FAR_KEY 108 	// l

// ############################################################
// 							TTY - UART
#define READ_MODE 0
#define PARITY 0
#define BLOCKING 0

#endif