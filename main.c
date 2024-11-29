#include <stdio.h> // nešto drugo
#include <GL/glut.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#define SUCCESS 1
#define FAIL 0

#define DEBUG 1

#define WIDTH_OFFSET 38
#define HEIGHT_OFFSET -1
#define X_OFFSET -16
#define Y_OFFSET 21
#define TRANSPARENCY_OFFSET -30 // percentage
#define ORDER_OFFSET 1

#define START_COORDINATE_X 0
#define START_COORDINATE_Y 0
#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720
#define MIN_HEIGHT -4
#define MAX_HEIGHT 2

#define TONE_ONE_DURATION 50
#define TONE_TWO_DURATION 250
#define TONE_THREE_DURATION 500

struct rectangle{
	int x;
	int y;
	int order;
	float width;
	float height;
	float angle;
	float rgba_color[4];
};


struct rectangle* base_rectangle = NULL;

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

// function for drawing rectangles based on the rectangle structure
void draw_rectangle_struct(struct rectangle* rect)
{
	// clear previous settings regarding the view matrix
	glLoadIdentity();
	// rotate the view matrix for the desired angle
	glRotatef(rect->angle, 0.0f, 0.0f, 1.0f);
	// move the rotated object in the desired place
	glTranslatef(rect->x, rect->y, -1.0f);
	// set the color of the rectangle
	glColor4f(rect->rgba_color[0], rect->rgba_color[1], rect->rgba_color[2], rect->rgba_color[3]);

	// draw the rectangle in the corrdinate system origin 
	// with the width and height informations located inside the structure rect
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);   
		glVertex3f(rect->width, 0.0f, 0.0f);
		glVertex3f(rect->width, rect->height, 0.0f);
		glVertex3f(0.0f, rect->height, 0.0f);	
	glEnd();
}

// function for drawing the 3 "parking sensors"
void draw_parking_sensors()
{
	// create a local copy of the base rectangle
	// in order not to modify the real object
	glLoadIdentity();
	display();
	struct rectangle* rectangle = base_rectangle;
	if (rectangle->order <= 3)
	{
		// draw the first, base rectangle
		draw_rectangle_struct(rectangle);
	} 
	if (rectangle->order <= 2)
	{
		// calculate the X coordinate for the second rectangle
		// by using the offset that was above defined
		rectangle->x = rectangle->x + X_OFFSET;
		// calculate the Y coordinate for the second rectangle
		rectangle->y = rectangle->y + Y_OFFSET;
		// calculate the second rectangle width
		rectangle->width = rectangle->width + WIDTH_OFFSET;
		// calculate the second rectangle height
		rectangle->height = rectangle->height + HEIGHT_OFFSET;
		// calculate the second rectangle transparency
		rectangle->rgba_color[3] = rectangle->rgba_color[3] + (TRANSPARENCY_OFFSET / 100.0f);
		draw_rectangle_struct(rectangle);
	}
	if (rectangle->order <= 1)
	{
		// this code is similar to the code above, I am calculating new values for the third rectangle
		rectangle->x = rectangle->x + X_OFFSET;
		rectangle->y = rectangle->y + Y_OFFSET;
		rectangle->width = rectangle->width + WIDTH_OFFSET;
		rectangle->height = rectangle->height + HEIGHT_OFFSET;
		rectangle->rgba_color[3] = rectangle->rgba_color[3] + (TRANSPARENCY_OFFSET / 100.0f);
		draw_rectangle_struct(rectangle);
	}
	
	printf("\n swapout buffers\n");
	
	glutSwapBuffers();
}



void display() {
// debug printing
#if DEBUG == 1
	printf("\nDisplay\n");
#endif

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
	
	// apply the drawings to the window
	glutSwapBuffers();

}

void button_pressed(unsigned char key, int x, int y)
{
	// create the base rectangle structure and fill it with data
	base_rectangle->width = 45.0f;
	base_rectangle->height = 17.0f;
	base_rectangle->angle = 54.0f;
	base_rectangle->x = 405.0f;
	base_rectangle->y = 104.0f;
	// using the RGBA color model, hence 4 bit array
	// R - red
	// G - green
	// B - blue
	// A - alpha (transparency)
	base_rectangle->rgba_color[0] = 1.0f;
	base_rectangle->rgba_color[1] = 0.0f;
	base_rectangle->rgba_color[2] = 0.0f;
	base_rectangle->rgba_color[3] = 1.0f;


	switch(key){
		case 'q':
			base_rectangle->order = 1;
			break;
		case 'w':
			base_rectangle->order = 2;
			break;
		case 'e':
			base_rectangle->order = 3;
			break;
		case 'r':
			base_rectangle->order = 4;
			break;
		default:
			base_rectangle->order = 5;
			break;
	}
	// call the function for drawing the 3 rectangles representing the distances
	// in the parking sensors
	draw_parking_sensors(&base_rectangle);

}


void idle()
{
	static struct timeval time;
	static int current_time = 0;
	static int previous_time = 0;
	int factor = 0;

	// get current time
	if (gettimeofday(&time, NULL) == -1)
	{
		return FAIL;
	}

	// get the microsecond part of the time and divide it by 1000 to get miliseconds
	current_time = time.tv_usec / 1000;

#if DEBUG == 1
	// printf("\n%d\n", current_time);
#endif

	// calculate how mutch time has passed since the last time this function was called
	factor = current_time - previous_time;
	// given that the result can be negative, you need to check if it is negative and turn it to positive
	factor = factor > 0 ? factor : factor * -1;

#if DEBUG == 1
	// printf("\n%d\n", base_rectangle->order);
#endif
	printf(" ");
	// with the help of struct rectangle member order, determine which tone is playing
	// i.e. what should be the period of the audio sound
	if (base_rectangle->order == 3){
		
		if (factor > TONE_ONE_DURATION) // play a sound every 100 ms
		{
			printf("\a");
			previous_time = current_time;
		}
	} else if (base_rectangle->order == 2)
	{
		if (factor > TONE_TWO_DURATION) // play a sound every 300 ms
		{
			printf("\a");
			previous_time = current_time;
		}
	} else if (base_rectangle->order == 1)
	{
		if (factor > TONE_THREE_DURATION) // play a sound every 500 ms
		{
			printf("\a");
			previous_time = current_time;
		} else
		{

		}
	} else
	{
		
	}
}


int main(int argc, char** argv) {
	base_rectangle = (struct rectangle*)malloc(sizeof(struct rectangle));
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