#include <string.h>
#include <errno.h>
#include "idle.h"

#include "uart_handler.h"
#include "draw.h"

// define return constants
#define SUCCESS 1
#define FAIL 0
#define True 1
#define False 0

#define DEBUG 0
#define DEBUG_IDLE 0

// define origin coordinates
#define START_COORDINATE_X 0
#define START_COORDINATE_Y 0

// define window dimensions
#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

// define depth
#define MIN_HEIGHT -4
#define MAX_HEIGHT 2

// audio settings
#define RATE 44100

// front left base rectangle
static struct rectangle* FL_base_rectangle = NULL;
// front right base rectangle
static struct rectangle* FR_base_rectangle = NULL;
// back left base rectangle
static struct rectangle* BL_base_rectangle = NULL;
// back right base rectangle
static struct rectangle* BR_base_rectangle = NULL;

static int* sensor_values;

// uart handler initialisation
static Display *display_thing;
static unsigned int keycode;
static int fd;

// audio controller instance pointer
static snd_pcm_t *pcm;
static snd_pcm_hw_params_t *hw_params_slow;
static snd_pcm_hw_params_t *hw_params_medium;
static snd_pcm_hw_params_t *hw_params_fast;
static short samples[9600];

unsigned char* loadPPM(const char* filename, int* width, int* height) {
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("error reading ppm file, could not locate %s", filename);
		*width = 0;
		*height = 0;

		return NULL;
	}
	retval_fgets = fgets(buf[0], BUFSIZE, fp);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	*width = atoi(buf[1]);
	*height = atoi(buf[2]);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	rawData = (char*)calloc(*width * *height, 3);
	read = fread(rawData, (*width) * (*height) * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		printf("error parsing ppm file, incomplete data\n %s\n", strerror(errno));
		memset(rawData, 0, *width * *height * 3);
		*width = 0;
		*height = 0;

		return NULL;
	}

#if DEBUG == 1
	printf("\nloadPPM rawData[0]: %d\n", rawData[0]);
#endif

	return rawData;
}


void initGL()
{
#if DEBUG == 1
	printf("\ninitGL\n");
#endif
	// init alpha blending function
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glShadeModel(GL_SMOOTH); // enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // get clear background (black color)
	glClearDepth(1.0f); // color depth buffer
	glDepthFunc(GL_LEQUAL); // configuration of depth testing
							// enable additional options regarding: perspective correction, anti-aliasing, etc
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}


// this needs to be executed at least once in order to set the display properties :)
void reshape(int width, int height)
{
#if DEBUG == 1
	printf("\nreshape callback\n");
#endif
	// specify the top left corner coordinates
	// specify the new rectangle width and height
	glViewport(START_COORDINATE_X, START_COORDINATE_Y, width, height);
	// switch to matrix projection
	glMatrixMode(GL_PROJECTION);
	// clean projection matrix
	glLoadIdentity();
	// set camera view
	// set the coordinate system in the range [0, WINDOW_WIDTH], [0, WINDOW_HEIGHT]
	glOrtho(START_COORDINATE_X, WINDOW_WIDTH, WINDOW_HEIGHT, START_COORDINATE_X, MAX_HEIGHT, MIN_HEIGHT);
	// swith back to matrix
	glMatrixMode(GL_MODELVIEW);
}


void draw_canvas(int width, int height)
{

	// create the canvas over the whole window
	// pick the color (white in this case)
	glColor3f(1.0, 1.0, 1.0);
	// draw the canvas over the given area
	glTexCoord2f(START_COORDINATE_X, WINDOW_HEIGHT); glVertex3f(0, 0, 0);
	glTexCoord2f(WINDOW_WIDTH, WINDOW_HEIGHT); glVertex3f(width, 0, 0);
	glTexCoord2f(WINDOW_WIDTH, START_COORDINATE_Y); glVertex3f(width, height, 0);
	glTexCoord2f(START_COORDINATE_X, START_COORDINATE_Y); glVertex3f(0, height, 0);
}


void loadTexture()
{
#if DEBUG == 1
	printf("\nloadTexture\n");
#endif

	GLuint texture[1]; // declaring space for one texture
	int twidth, theight; // declaring variable for width and height of an image
	unsigned char* tdata; // declaring pixel data
						  // loading image data from specific file:
	tdata = loadPPM("auto3.ppm", &twidth, &theight);
	if (tdata == NULL) return; // check if image data is loaded
							   // generating a texture to show the image
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


void display() {
// debug printing
#if DEBUG_DRAW == 1
	printf("\nDisplay\n");
#endif

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	
	// this is a specific way to load the image
	// in here we are loading the image as a texture, drawing a rectangle and then 
	// we apply this texture to the rectangle
	glTexCoord2f(0, 1); glVertex3f(0, WINDOW_HEIGHT * 0.75, 0);
	glTexCoord2f(1, 1); glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT * 0.75, 0);
	glTexCoord2f(1, 0); glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT * 0.25, 0);
	glTexCoord2f(0, 0); glVertex3f(0, WINDOW_HEIGHT * 0.25, 0);
	glEnd();
	
	draw_parking_sensor_outline(FR_base_rectangle);
	draw_parking_sensor_outline(FL_base_rectangle);
	draw_parking_sensor_outline(BR_base_rectangle);
	draw_parking_sensor_outline(BL_base_rectangle);
	// apply the drawings to the window
	glutSwapBuffers();

}


void check_pressed_buttons(unsigned char key, struct rectangle* base_rectangle)
{
	struct keymap* keys = base_rectangle->keys;
	if (key == keys->far_key)
	{

		base_rectangle->distance = 1; // in case the close distance button has been pressed, set the order to 1

	} else if (key == keys->middle_key)
	{
		base_rectangle->distance = 2; // in case the middle distance button has been pressed, set the order to 2

	} else if (key == keys->close_key)	// in case the close distance button has been pressed, set the order to 3
	{
		base_rectangle->distance = 3;

	} else if (key == keys->clear_key) // in case the clear button has been pressed, set the order to 4
	{
		base_rectangle->distance = 4;

	} 
}


void button_pressed(unsigned char key, int x, int y)
{
	static previous_key = 0;
	// execute the following code only if there was a different key pressed
	if (key != previous_key){
		check_pressed_buttons(key, FR_base_rectangle);
		check_pressed_buttons(key, FL_base_rectangle);
		check_pressed_buttons(key, BR_base_rectangle);
		check_pressed_buttons(key, BL_base_rectangle);
		// call the function for drawing the 3 rectangles representing the distances
		// in the parking sensors
	#if DEBUG_DRAW == 1
		printf("call draw_parking_sensors \n");
	#endif
		draw_all_parking_sensors(FL_base_rectangle, FR_base_rectangle, BL_base_rectangle, BR_base_rectangle);
		previous_key = key;
	} else
	{

	}
}


void fill_base_rectangle(float x, float y, float angle, struct rectangle* base_rectangle)
{
	// create the base rectangle structure and fill it with data
	base_rectangle->width = 45.0f;
	base_rectangle->height = 17.0f;
	base_rectangle->angle = angle;
	base_rectangle->x = x;
	base_rectangle->y = y;
	base_rectangle->distance = 4;
	// using the RGBA color model, hence 4 bit array
	// R - red
	// G - green
	// B - blue
	// A - alpha (transparency)
	base_rectangle->rgba_color[0] = 1.0f;
	base_rectangle->rgba_color[1] = 0.0f;
	base_rectangle->rgba_color[2] = 0.0f;
	base_rectangle->rgba_color[3] = 1.0f;
}


void idle()
{
	static past_state = 4;
#if USE_PARKING_SENSOR == 1
	// read the parking sensor values
	if (get_sensor_data(sensor_values, fd) == 0)
	{
		printf("Error when reading from UART!");
	}
#endif
	check_distance(FL_base_rectangle, display_thing, keycode, sensor_values);
	check_distance(FR_base_rectangle, display_thing, keycode, sensor_values);
	check_distance(BL_base_rectangle, display_thing, keycode, sensor_values);
	check_distance(BR_base_rectangle, display_thing, keycode, sensor_values);


#if USE_MP3 == 1

	if (FL_base_rectangle->distance == 3 && past_state != 3){
		past_state = 3;	
		snd_pcm_hw_params(pcm, hw_params_fast);
	} else if (FL_base_rectangle->distance == 2 && past_state != 2)
	{
		past_state = 2;
		snd_pcm_hw_params(pcm, hw_params_medium);
	} else if (FL_base_rectangle->distance == 1 && past_state != 1)
	{
		past_state = 1;
		snd_pcm_hw_params(pcm, hw_params_slow);
	} else
	{	

	}
	if (FL_base_rectangle->distance <= 3)
	{
		printf("noise %d\n", FL_base_rectangle->distance);
		snd_pcm_writei(pcm, samples, 9600);
	}
	if (FL_base_rectangle->distance == 4)
	{
		printf("shit up\n");
	}

#endif
	
}

void config_audio_settings(int rate, snd_pcm_hw_params_t *hw_params)
{
	
	snd_pcm_hw_params_any(pcm, hw_params);
	snd_pcm_hw_params_set_access(pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(pcm, hw_params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_channels(pcm, hw_params, 1);
	snd_pcm_hw_params_set_rate(pcm, hw_params, rate, 0);
	snd_pcm_hw_params_set_periods(pcm, hw_params, 1, 0);
	snd_pcm_hw_params_set_period_time(pcm, hw_params, 100000, 0); // 0.1 seconds	
}


int main(int argc, char** argv) {
	// allocate the base rectangles for each parking sensor/corner of the car
	FL_base_rectangle = (struct rectangle*)malloc(sizeof(struct rectangle));
	FL_base_rectangle->keys = (struct keymap*)malloc(sizeof(struct keymap));

	FR_base_rectangle = (struct rectangle*)malloc(sizeof(struct rectangle));
	FR_base_rectangle->keys = (struct keymap*)malloc(sizeof(struct keymap));
	
	BL_base_rectangle = (struct rectangle*)malloc(sizeof(struct rectangle));
	BL_base_rectangle->keys = (struct keymap*)malloc(sizeof(struct keymap));
	
	BR_base_rectangle = (struct rectangle*)malloc(sizeof(struct rectangle));
	BR_base_rectangle->keys = (struct keymap*)malloc(sizeof(struct keymap));
	
#if USE_PARKING_SENSOR == 1
	sensor_values  = (int*)calloc(sizeof(int), NUMBER_OF_SENSORS);
	config_uart(&fd);
	display_thing = XOpenDisplay(NULL);
#endif

	fill_base_rectangle(129.0f, -318.0f, 126.0f, FR_base_rectangle);
	FR_base_rectangle->keys->far_key = 'q';
	FR_base_rectangle->keys->middle_key = 'w';
	FR_base_rectangle->keys->close_key = 'e';
	FR_base_rectangle->keys->clear_key = 'r';

	fill_base_rectangle(405.0f, 105.0f, 54.0f, FL_base_rectangle);
	FL_base_rectangle->keys->far_key = 'a';
	FL_base_rectangle->keys->middle_key = 's';
	FL_base_rectangle->keys->close_key = 'd';
	FL_base_rectangle->keys->clear_key = 'f';
	

	fill_base_rectangle(-610.0f, 260.0f, 233.0f, BR_base_rectangle);
	BR_base_rectangle->keys->far_key = 'z';
	BR_base_rectangle->keys->middle_key = 'u';
	BR_base_rectangle->keys->close_key = 'i';
	BR_base_rectangle->keys->clear_key = 'o';

	fill_base_rectangle(25.0f, 693.0f, 310.0f, BL_base_rectangle);
	BL_base_rectangle->keys->far_key = 'h';
	BL_base_rectangle->keys->middle_key = 'j';
	BL_base_rectangle->keys->close_key = 'k';
	BL_base_rectangle->keys->clear_key = 'l';

	snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);


	
	snd_pcm_hw_params_alloca(&hw_params_slow);
	snd_pcm_hw_params_alloca(&hw_params_medium);
	snd_pcm_hw_params_alloca(&hw_params_fast);
	config_audio_settings(16000, hw_params_slow);
	config_audio_settings(32000, hw_params_medium);
	config_audio_settings(48000, hw_params_fast);


	static FILE* fp;
	fp = fopen("beep-07a.wav","rb");
	fread(samples, sizeof(short), 9600, fp);
	fclose(fp);

	/* 1) INITIALIZATION */
	// initialize GLUT
	glutInit(&argc, argv);
	// set window position and size
	glutInitWindowPosition(545, 180);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// set the combination of predefined values for display mode
	// set color space (Red, Green, Blue - RGB)
	// alocate depth buffer
	// set the size of the buffer (double)
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// create window
	glutCreateWindow("Pero");
	/* 2) REGISTRATION OF CALLBACK FUNCTION */
	// function called when new window need to be drawn
	glutDisplayFunc(display);
	// function called when window changes the size
	glutReshapeFunc(reshape);
	// function called when nothing else is executing and CPU is free
	glutIdleFunc(idle);
	// enable user input
	glutKeyboardFunc(button_pressed);
	initGL();
	loadTexture();   //enable this to load image
	
	/* 3) START GLUT PROCESSING CYCLE */
	glutMainLoop();


	return 0;
}