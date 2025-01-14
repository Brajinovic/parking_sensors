#include "idle.h"
#include "uart_handler.h"


// front left base rectangle
static struct rectangle* FL_base_rectangle = DEFAULT_PTR;
// front right base rectangle
static struct rectangle* FR_base_rectangle = DEFAULT_PTR;
// back left base rectangle
static struct rectangle* BL_base_rectangle = DEFAULT_PTR;
// back right base rectangle
static struct rectangle* BR_base_rectangle = DEFAULT_PTR;

static int* sensor_values;

// uart handler initialisation
static Display *display_thing = DEFAULT_PTR;
static unsigned int keycode = DEFAULT_INT;
static int fd = DEFAULT_INT;

#if USE_MP3 == 1
	// audio controller instance pointer
	static int pcm_open = 1;
	static snd_pcm_t *pcm = DEFAULT_PTR;
	static snd_pcm_hw_params_t *hw_params_slow = DEFAULT_PTR;
	static snd_pcm_hw_params_t *hw_params_medium = DEFAULT_PTR;
	static snd_pcm_hw_params_t *hw_params_fast = DEFAULT_PTR;
	static short samples[SAMPLE_SIZE];
#endif



void initGL()
{
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


void display() {
	// load the background (image and parking sensor outlines)
	load_background(FL_base_rectangle, FR_base_rectangle, BL_base_rectangle, BR_base_rectangle);

	// apply the drawings to the window
	glutSwapBuffers();
}


void check_pressed_buttons(unsigned char key, struct rectangle* base_rectangle)
{
	// this is a final state machiene where I am checking how far away the obsticale is
	// and writing that information into the distance attribute of the base rectangle
	// later I am going to use this information in order to draw the proper active rectangle
	// (this is one part of my final state machine)


	// each parking sensor has unique keys on which he gets activated
	// the specific keys are written in the base rectangle in the shape of struct keymap structure
	struct keymap* keys = base_rectangle->keys;

	if (key == keys->far_key)
	{
		base_rectangle->distance = 3; // in case the close distance button has been pressed, set the order to 3

	} else if (key == keys->middle_key)
	{
		base_rectangle->distance = 2; // in case the middle distance button has been pressed, set the order to 2

	} else if (key == keys->close_key)	// in case the close distance button has been pressed, set the order to 1
	{
		base_rectangle->distance = 1;

	} else if (key == keys->clear_key) // in case the clear button has been pressed, set the order to 4
	{
		base_rectangle->distance = 4;

	} 
}


void on_button_pressed(unsigned char key, int x, int y)
{
	static int previous_key = DEFAULT_INT;
	// in case of the same consecutive input, there is no need to do any new computing

	if (key != previous_key){
		// aditionally, there are only 12 keys I need to act uppon, 3 of which for each parking sensor
		// so it doesn't make sense to call the check_pressesd_buttons function all the time (c.a. half the time), for each base rectangle
		// i.e. times 4. So I saved 12 x 4 function calls. 
		if (key == FL_base_rectangle->keys->far_key || key == FL_base_rectangle->keys->middle_key || key == FL_base_rectangle->keys->close_key || key == FL_base_rectangle->keys->clear_key)
		{
			check_pressed_buttons(key, FL_base_rectangle);
		} else if (key == FR_base_rectangle->keys->far_key || key == FR_base_rectangle->keys->middle_key || key == FR_base_rectangle->keys->close_key || key == FR_base_rectangle->keys->clear_key)
		{
			check_pressed_buttons(key, FR_base_rectangle);
		} else if (key == BL_base_rectangle->keys->far_key || key == BL_base_rectangle->keys->middle_key || key == BL_base_rectangle->keys->close_key || key == BL_base_rectangle->keys->clear_key)
		{ 
			check_pressed_buttons(key, BL_base_rectangle);
		} else if (key == BR_base_rectangle->keys->far_key || key == BR_base_rectangle->keys->middle_key || key == BR_base_rectangle->keys->close_key || key == BR_base_rectangle->keys->clear_key)
		{	
			check_pressed_buttons(key, BR_base_rectangle);
		} else
		{

		}
		
		// call the function for drawing the 3 rectangles (for each parking sensor) 
		// representing the distances in the parking sensors

		draw_all_parking_sensors(FL_base_rectangle, FR_base_rectangle, BL_base_rectangle, BR_base_rectangle);
		previous_key = key;
	} else
	{

	}
}


int checkState()
{
	int current_state = BASE_RECTANGLE_DEFAULT_DISTANCE;

	if (current_state > FL_base_rectangle->distance)
	{
		current_state = FL_base_rectangle->distance;
	}
	if (current_state > FR_base_rectangle->distance)
	{
		current_state = FR_base_rectangle->distance;
	}
	if (current_state > BL_base_rectangle->distance)
	{
		current_state = BL_base_rectangle->distance;
	}
	if (current_state > BR_base_rectangle->distance)
	{
		current_state = BR_base_rectangle->distance;
	}

	return current_state;
}

// this function is used for playing sound effects
// and for when I am using sensor inputs, not keyboard inputs
// for parking sensor activation
void idle()
{
	static int past_state = BASE_RECTANGLE_DEFAULT_DISTANCE;
#if USE_PARKING_SENSOR == 1
	// read the parking sensor values
	if (get_sensor_data(sensor_values, fd) == 0)
	{
		printf("Error when reading from UART!");
	}
#endif
	// I might be able to move these 4 function calls under the USE_PARKING_SENSOR == 1 part
	check_distance(FL_base_rectangle, display_thing, keycode, sensor_values);
	check_distance(FR_base_rectangle, display_thing, keycode, sensor_values);
	check_distance(BL_base_rectangle, display_thing, keycode, sensor_values);
	check_distance(BR_base_rectangle, display_thing, keycode, sensor_values);


#if USE_MP3 == 1
	int state = checkState();

	if (state == 3 && past_state != 3){
		past_state = 3;	
		snd_pcm_hw_params(pcm, hw_params_slow);
	} else if (state == 2 && past_state != 2)
	{
		past_state = 2;
		snd_pcm_hw_params(pcm, hw_params_medium);
	} else if (state == 1 && past_state != 1)
	{
		past_state = 1;
		snd_pcm_hw_params(pcm, hw_params_fast);
	} else if (state == 4 && past_state != 4)
	{	
		past_state = 4;
	}
	if (state <= 3)
	{
		// printf("noise %d\n", FL_base_rectangle->distance);
		snd_pcm_writei(pcm, samples, SAMPLE_COUNT);
	} else
	{
		// clear the output buffer
		// otherwise sound will keep playing untill the buffer is empty
		snd_pcm_drop(pcm);
	}
	
#endif
	
}

#if USE_MP3 == 1
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
#endif


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

	populate_base_rectangle(129.0f, -318.0f, 126.0f, FR_base_rectangle);
	FR_base_rectangle->keys->far_key = 'q';
	FR_base_rectangle->keys->middle_key = 'w';
	FR_base_rectangle->keys->close_key = 'e';
	FR_base_rectangle->keys->clear_key = 'r';

	populate_base_rectangle(405.0f, 105.0f, 54.0f, FL_base_rectangle);
	FL_base_rectangle->keys->far_key = 'a';
	FL_base_rectangle->keys->middle_key = 's';
	FL_base_rectangle->keys->close_key = 'd';
	FL_base_rectangle->keys->clear_key = 'f';
	

	populate_base_rectangle(-610.0f, 260.0f, 233.0f, BR_base_rectangle);
	BR_base_rectangle->keys->far_key = 'z';
	BR_base_rectangle->keys->middle_key = 'u';
	BR_base_rectangle->keys->close_key = 'i';
	BR_base_rectangle->keys->clear_key = 'o';

	populate_base_rectangle(25.0f, 693.0f, 310.0f, BL_base_rectangle);
	BL_base_rectangle->keys->far_key = 'h';
	BL_base_rectangle->keys->middle_key = 'j';
	BL_base_rectangle->keys->close_key = 'k';
	BL_base_rectangle->keys->clear_key = 'l';

#if USE_MP3 == 1
	// connect to the speakers, and configure them for playback in non-blocking mode
	snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

	snd_pcm_hw_params_alloca(&hw_params_slow);
	snd_pcm_hw_params_alloca(&hw_params_medium);
	snd_pcm_hw_params_alloca(&hw_params_fast);
	config_audio_settings(SLOW_RATE, hw_params_slow);
	config_audio_settings(MEDIUM_RATE, hw_params_medium);
	config_audio_settings(FAST_RATE, hw_params_fast);


	static FILE* fp;
	fp = fopen("beep-07a.wav","rb");
	fread(samples, sizeof(short), SAMPLE_COUNT, fp);
	fclose(fp);
#endif

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
	glutKeyboardFunc(on_button_pressed);
	initGL();
	loadTexture();   //enable this to load image
	
	/* 3) START GLUT PROCESSING CYCLE */
	glutMainLoop();


	return 0;
}