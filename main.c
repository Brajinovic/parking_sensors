#include <stdio.h>
#include <GL/glut.h>
#include <string.h>
#include <errno.h>


#define DEBUG 0
#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

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
	// specify the desired rectangle
	// the first two numbers are the coordinates of the top left corner
	glViewport(0, 0, width, height);
	// switch to matrix projection
	glMatrixMode(GL_PROJECTION);
	// clean projection matrix
	glLoadIdentity();
	// set camera view (orthographic projection with 4x4 unit square canvas)

	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 2, -4);
	// swith back to matrix
	glMatrixMode(GL_MODELVIEW);
}

void draw_canvas(int width, int height)
{

	// create the canvas over the whole window
	// pick the color (white in this case)
	glColor3f(1.0, 1.0, 1.0);
	// draw the canvas over the given area
	glTexCoord2f(0, WINDOW_HEIGHT); glVertex3f(0, 0, 0);
	glTexCoord2f(WINDOW_WIDTH, WINDOW_HEIGHT); glVertex3f(width, 0, 0);
	glTexCoord2f(WINDOW_WIDTH, 0); glVertex3f(width, height, 0);
	glTexCoord2f(0, 0); glVertex3f(0, height, 0);
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
	// printf("width: %d\n height: %d\n", twidth, theight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


void display() {
#if DEBUG == 1
	printf("\nDisplay\n");
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	// draw the canvas over the given area
	
	glTexCoord2f(0, 1); glVertex3f(0, WINDOW_HEIGHT * 0.75, 0);
	glTexCoord2f(1, 1); glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT * 0.75, 0);
	glTexCoord2f(1, 0); glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT * 0.25, 0);
	glTexCoord2f(0, 0); glVertex3f(0, WINDOW_HEIGHT * 0.25, 0);
	glEnd();
	
	glRotatef(54.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(405.0f, 104.0f, -1.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);   //choosing red color
	
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);   
		glVertex3f(45.00f, 0.0f, 0.0f);
		glVertex3f(45.00f, 17.00f, 0.0f);
		glVertex3f(0.0f, 17.00f, 0.0f);
		
	glEnd();

	
	glLoadIdentity();
	glRotatef(54.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(390.0f, 125.0f, -1.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 0.65f);   //choosing green color
		
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);   
		glVertex3f(80.0f, 0.0f, 0.0f);
		glVertex3f(80.0f, 18.0f, 0.0f);
		glVertex3f(0.0f, 18.0f, 0.0f);
	glEnd();
	
	glLoadIdentity();
	glRotatef(54.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(375.0f, 147.0f, -1.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);   //choosing red color
		
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);   
		glVertex3f(110.00f, 0.0f, 0.0f);
		glVertex3f(110.00f, 20.00f, 0.0f);
		glVertex3f(0.0f, 20.00f, 0.0f);
	glEnd();
	
	glutSwapBuffers();
}


void idle()
{
	// here comes the code which will be executed when program state is idle

}

int main(int argc, char** argv) {
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
	initGL();
	loadTexture();   //enable this to load image
	
	/* 3) START GLUT PROCESSING CYCLE */
	glutMainLoop();
	return 0;
}