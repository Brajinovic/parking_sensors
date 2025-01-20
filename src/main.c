#include "keyboard_input_controller.h"


// front left base rectangle
static struct rectangle* FL_base_rectangle = DEFAULT_PTR;
// front right base rectangle
static struct rectangle* FR_base_rectangle = DEFAULT_PTR;
// back left base rectangle
static struct rectangle* BL_base_rectangle = DEFAULT_PTR;
// back right base rectangle
static struct rectangle* BR_base_rectangle = DEFAULT_PTR;

// there is a functionality built in to test the application using
// HC_SR04 ultrasonic sensors. Only if that testin option is enabled,
// initialize the following variables

#if USE_PARKING_SENSOR == 1
	static int* sensor_values;

	// uart handler initialisation
	static Display *display_thing = DEFAULT_PTR;
	static unsigned int keycode = DEFAULT_INT;
	static int fd = DEFAULT_INT;
#endif

// there is an option to mute the application
// i.e. to turn off the noise producing part
// that that option is used, there is no need for the 
// initialization of the variables seen underneath

#if USE_MP3 == 1
	// audio controller instance pointer
	static int pcm_open = PCM_INSTANCE_ID;
	static snd_pcm_t *pcm = DEFAULT_PTR;
	// creating different configuration
	// one for each playback sample rate to create
	// the illusion of faster or slower audio
	static snd_pcm_hw_params_t *hw_params_slow = DEFAULT_PTR;
	static snd_pcm_hw_params_t *hw_params_medium = DEFAULT_PTR;
	static snd_pcm_hw_params_t *hw_params_fast = DEFAULT_PTR;
	// array holding the audio samples
	static short samples[SAMPLE_SIZE];
#endif


// function used by Glut to initialize required prerequisites
void initGL()
{
	// init alpha blending function
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glShadeModel(GL_SMOOTH); // enable smooth shading
	glClearColor(FALSE, FALSE, FALSE, FALSE); // get clear background (black color)
	glClearDepth(TRUE); // color depth buffer
	glDepthFunc(GL_LEQUAL); // configuration of depth testing
							// enable additional options regarding: perspective correction, anti-aliasing, etc
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}


// this function is executed each time there is a 
// change in size of the window. This includes the initial
// creation as it changes the window size from non existant
// to the initial size set by the programmer
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


// this function is used for the initial background loading
void display() {
	// load the background (image and parking sensor outlines)
	load_background(FL_base_rectangle, FR_base_rectangle, BL_base_rectangle, BR_base_rectangle);

	// apply the drawings to the window
	glutSwapBuffers();
}


// this function handles key press events
// in case the button corresponds to the ones
// allocated to the base rectangles, the distance
// value of the rectangle could change (depends on the key pressed)
void on_button_pressed(unsigned char key, int x, int y)
{
	static int previous_key = DEFAULT_INT;

	// only go forward if the same key is not
	// pressed beeing repetedly
	if (key != previous_key){
		// aditionally, there are only 12 keys I need to act uppon, 3 of which for each parking sensor
		// so it doesn't make sense to call the check_pressesd_buttons function all the time (c.a. half the time), for each base rectangle
		// i.e. times 4. So I saved 12 x 4 function calls.

		// only if one of the keys allocated for front left parking sensor
		// is beeing pressed, check which button it is and update the distance
		// attribute (done in check_pressed_button function) 
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
		
		// reset the previous_key value to the current key
		previous_key = key;
	} else
	{

	}
}

// this function is used for playing sound effects
// and for when sensor inputs are beeing used
// for parking sensor activation
void idle()
{
	static int past_distance = BASE_RECTANGLE_DEFAULT_DISTANCE;
#if USE_PARKING_SENSOR == 1
	// read the parking sensor values
	if (get_sensor_data(sensor_values, fd) == FAIL)
	{
		printf("Error when reading from UART!");
	}
	// HC-SR04 distance checker
	// check the distance for each sensor and press the
	// correspoinding key
	check_distance_HC(FL_base_rectangle, display_thing, keycode, sensor_values);
	check_distance_HC(FR_base_rectangle, display_thing, keycode, sensor_values);
	check_distance_HC(BL_base_rectangle, display_thing, keycode, sensor_values);
	check_distance_HC(BR_base_rectangle, display_thing, keycode, sensor_values);
// end of USE_PARKING_SENSOR
#endif
	


#if USE_MP3 == 1
	// when the audio is not muted
	// check the distance for all parking sensors
	// take the lowest value and store it in distance
	// the lowest value represents the sensor that measures
	// the closest obsticale
	int distance = check_distance(FL_base_rectangle, FR_base_rectangle, BL_base_rectangle, BR_base_rectangle);

	// when the object is further away, create 
	// sound signals with lower frequency
	if (distance == FAR && past_distance != FAR){
		// reset the past distance to the current one
		past_distance = FAR;	
		// reset the hardware configuration to the 
		// relevant playback frequency
		snd_pcm_hw_params(pcm, hw_params_slow);

	// when the object is at the middle distance, 
	// use a little faster frequency to play the sound
	} else if (distance == MIDDLE && past_distance != MIDDLE)
	{
		past_distance =  MIDDLE;
		snd_pcm_hw_params(pcm, hw_params_medium);

	// when the object is close, choose the fastest frequency
	// to do audio playback
	} else if (distance == CLOSE && past_distance != CLOSE)
	{
		past_distance = CLOSE;
		snd_pcm_hw_params(pcm, hw_params_fast);
	
	// when the object is really far away ( F_FAR stands for fucking far)
	// just reset the past distance variable value
	} else if (distance == F_FAR && past_distance != F_FAR)
	{	
		past_distance = F_FAR;
	}

	// play audio if the distance is less or equal to FAR
	if (distance <= FAR)
	{
		snd_pcm_writei(pcm, samples, SAMPLE_SIZE);
	
	// otherwise, clean the output buffer
	} else
	{
		// clear the output buffer
		snd_pcm_drop(pcm);
	}
// end of USE_MP3
#endif
	// call the function for drawing the 3 rectangles (for each parking sensor) 
	// representing the distances in the parking sensors
	render_graphics(FL_base_rectangle, FR_base_rectangle, BL_base_rectangle, BR_base_rectangle);
		
}

#if USE_MP3 == 1
	// this function is used for setting the hardware configuration for
	// the audio playback
	void config_audio_settings(int rate, snd_pcm_hw_params_t *hw_params)
	{
		// set the active hw_params object
		snd_pcm_hw_params_any(pcm, hw_params);
		// set the access type
		snd_pcm_hw_params_set_access(pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
		// set the format
		snd_pcm_hw_params_set_format(pcm, hw_params, SND_PCM_FORMAT_S16_LE);
		// set the number of channels
		snd_pcm_hw_params_set_channels(pcm, hw_params, CHANNEL_COUNT);
		// set the playback rate
		snd_pcm_hw_params_set_rate(pcm, hw_params, rate, DEFAULT_INT);
		// set the number of periods
		snd_pcm_hw_params_set_periods(pcm, hw_params, NUMBER_OF_PERIODS, DEFAULT_INT);
		// set the period duration
		snd_pcm_hw_params_set_period_time(pcm, hw_params, PERIOD, DEFAULT_INT); // 0.1 seconds	
	}
// end of USE_MP3
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
	printf("init, config uart\n");
	sensor_values  = (int*)calloc(sizeof(int), NUMBER_OF_SENSORS);
	config_uart(&fd);
	display_thing = XOpenDisplay(NULL);
#endif

	// fill the base rectangles with data
	populate_base_rectangle(FR_X, FR_Y, FR_ROTATION, FR_base_rectangle);
	FR_base_rectangle->keys->far_key = FR_FAR_KEY;
	FR_base_rectangle->keys->middle_key = FR_MIDDLE_KEY;
	FR_base_rectangle->keys->close_key = FR_CLOSE_KEY;
	FR_base_rectangle->keys->clear_key = FR_F_FAR_KEY;

	populate_base_rectangle(FL_X, FL_Y, FL_ROTATION, FL_base_rectangle);
	FL_base_rectangle->keys->far_key = FL_FAR_KEY;
	FL_base_rectangle->keys->middle_key = FL_MIDDLE_KEY;
	FL_base_rectangle->keys->close_key = FL_CLOSE_KEY;
	FL_base_rectangle->keys->clear_key = FL_F_FAR_KEY;
	

	populate_base_rectangle(BR_X, BR_Y, BR_ROTATION, BR_base_rectangle);
	BR_base_rectangle->keys->far_key = BR_FAR_KEY;
	BR_base_rectangle->keys->middle_key = BR_MIDDLE_KEY;
	BR_base_rectangle->keys->close_key = BR_CLOSE_KEY;
	BR_base_rectangle->keys->clear_key = BR_F_FAR_KEY;

	populate_base_rectangle(BL_X, BL_Y, BL_ROTATION, BL_base_rectangle);
	BL_base_rectangle->keys->far_key = BL_FAR_KEY;
	BL_base_rectangle->keys->middle_key = BL_MIDDLE_KEY;
	BL_base_rectangle->keys->close_key = BL_CLOSE_KEY;
	BL_base_rectangle->keys->clear_key = BL_F_FAR_KEY;
	printf("fill base rectangles");
#if USE_MP3 == 1
	// connect to the speakers, and configure them for playback in non-blocking mode
	snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);
	// create structures for different audio hadrware configurations
	snd_pcm_hw_params_alloca(&hw_params_slow);
	snd_pcm_hw_params_alloca(&hw_params_medium);
	snd_pcm_hw_params_alloca(&hw_params_fast);
	// fill those structures with data
	config_audio_settings(SLOW_RATE, hw_params_slow);
	config_audio_settings(MEDIUM_RATE, hw_params_medium);
	config_audio_settings(FAST_RATE, hw_params_fast);

	// load the audio file into program memory
	static FILE* fp;
	fp = fopen("beep-07a.wav","rb");
	fread(samples, sizeof(short), SAMPLE_SIZE, fp);
	fclose(fp);
#endif

	/* 1) INITIALIZATION */
	// initialize GLUT
	glutInit(&argc, argv);
	printf("glut init\n");
	// set window position and size
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
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
	printf("start main proccess");
	glutMainLoop();



	return SUCCESS;
}