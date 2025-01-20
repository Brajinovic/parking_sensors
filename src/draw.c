#include "draw.h"


unsigned char* loadPPM(const char* filename, int* width, int* height) {
	// return value variable
	int return_value = SUCCESS;
	// picture file pointer
	FILE* fp = DEFAULT_PTR;
	
	// variable in which return values are going to be stored
	unsigned int read = DEFAULT_INT;
	// buffer for raw picture data
	unsigned char* rawData = DEFAULT_PTR;
	
	// file header data buffer
	char buf[3][IMAGE_BUFFER_SIZE];
	// return value pointer
	char* retval_fgets = DEFAULT_PTR;
	
	size_t retval_sscanf;

	// open the image
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("error reading ppm file, could not locate %s", filename);
		*width = DEFAULT_INT;
		*height = DEFAULT_INT;

		return_value = FAIL;
	}

	// load data into the buffer untill you reach '#', load header data
	retval_fgets = fgets(buf[0], IMAGE_BUFFER_SIZE, fp);
	do
	{
		retval_fgets = fgets(buf[0], IMAGE_BUFFER_SIZE, fp);
	} while (buf[0][0] == '#');
	
	// read the width and height values of the image
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	*width = atoi(buf[1]);
	*height = atoi(buf[2]);
	
	do
	{
		retval_fgets = fgets(buf[0], IMAGE_BUFFER_SIZE, fp);
	} while (buf[0][0] == '#');

	// read the image data 
	rawData = (char*)calloc(*width * *height, 3);
	read = fread(rawData, (*width) * (*height) * 3, 1, fp);
	fclose(fp);
	// check the return value
	if (read != TRUE)
	{
		printf("error parsing ppm file, incomplete data\n");
		memset(rawData, DEFAULT_INT, *width * *height * 3);
		*width = DEFAULT_INT;
		*height = DEFAULT_INT;

		return_value = FAIL;
	}

	return return_value == FAIL ? DEFAULT_PTR : rawData;
}


// issue #5
void load_background(struct rectangle *FL_base_rectangle, struct rectangle* FR_base_rectangle, struct rectangle* BL_base_rectangle, struct rectangle* BR_base_rectangle)
{
	glLoadIdentity();
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	
	// this is a specific way to load the image
	// in here we are loading the image as a texture, drawing a rectangle and then 
	// we apply this texture to the rectangle

	// (TEXTURE_OFFSET_* / 100.0) calculates the texture offset in percenteges
	glTexCoord2f(0, 1); glVertex3f(START_COORDINATE_X, WINDOW_HEIGHT * (TEXTURE_OFFSET_1 / 100.0), BASE_LEVEL);
	glTexCoord2f(1, 1); glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT * (TEXTURE_OFFSET_1 / 100.0), BASE_LEVEL);
	glTexCoord2f(1, 0); glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT * (TEXTURE_OFFSET_2 / 100.0), BASE_LEVEL);
	glTexCoord2f(0, 0); glVertex3f(START_COORDINATE_X, WINDOW_HEIGHT * (TEXTURE_OFFSET_2 / 100.0), BASE_LEVEL);
	glEnd();
	
	// draw the outline for each parking sensor
	draw_parking_sensor_outline(FR_base_rectangle);
	draw_parking_sensor_outline(FL_base_rectangle);
	draw_parking_sensor_outline(BR_base_rectangle);
	draw_parking_sensor_outline(BL_base_rectangle);
}

void draw_rectangle(struct rectangle* rect)
{
	// clear previous settings regarding the view matrix (used for rotations)
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
		glVertex3f(START_COORDINATE_X, START_COORDINATE_Y, BASE_LEVEL);   
		glVertex3f(rect->width, START_COORDINATE_Y, BASE_LEVEL);
		glVertex3f(rect->width, rect->height, BASE_LEVEL);
		glVertex3f(START_COORDINATE_X, rect->height, BASE_LEVEL);	
	glEnd();
}

void calculate_rectangle_offset(struct rectangle* rectangle)
{
	rectangle->x = rectangle->x + X_OFFSET;
	// calculate the Y coordinate for the second rectangle
	rectangle->y = rectangle->y + Y_OFFSET;
	// calculate the second rectangle width
	rectangle->width = rectangle->width + WIDTH_OFFSET;
	// calculate the second rectangle height
	rectangle->height = rectangle->height + HEIGHT_OFFSET;
}

// function for drawing the 3 "parking sensors"
void draw_parking_sensor_outline(struct rectangle* base_rectangle)
{
	// create a local copy of the base rectangle
	// in order not to modify the real object
	struct rectangle rectangle = *base_rectangle;
	rectangle.rgba_color[3] = FRAME_TRANSPARENCY / 100.0f;

	// draw the base rectangle but with the transparency set to FRAME_TRANSPARENCY
	// in this way the rectanle is going to be transparent and it will serve as an outline
	draw_rectangle(&rectangle);
	// calculate the offset for the second rectangle and then draw it
	calculate_rectangle_offset(&rectangle);
	draw_rectangle(&rectangle);

	// calculate the offset for the third rectangle and then draw it
	calculate_rectangle_offset(&rectangle);	
	draw_rectangle(&rectangle);

}

void draw_active_rectangle(struct rectangle* base_rectangle)
{
	// create a local copy of the base rectangle
	// in order not to modify the main object
	struct rectangle rectangle = *base_rectangle;

	// each base rectangle object has an attribute designating
	// which of the three rectangles if and should be active
	// that attribute is the distance attribute
	// the following code is checking the value of the distance
	// attribute and calling the rectangle draw function accordingly

	if (rectangle.distance == CLOSE)
	{
		draw_rectangle(&rectangle);
	} else
	{	
	}

	// because of base rectangles, there is a need to calculate
	// the offsets for the second and third rectangle
	// the function calculate_rectangle_offset servers that purpose
	// in order not to modify the base rectangle, a local copy is created
	// and the offset calculations are executed uppon that local
	// copy
	calculate_rectangle_offset(&rectangle);

	if (rectangle.distance == MIDDLE)
	{
		draw_rectangle(&rectangle);
	} else
	{
	}

	calculate_rectangle_offset(&rectangle);
	
	if (rectangle.distance == FAR)
	{	
		draw_rectangle(&rectangle);
	} else
	{
	}
}


void render_graphics(struct rectangle* FL_base_rectangle, struct rectangle* FR_base_rectangle, struct rectangle* BL_base_rectangle, struct rectangle* BR_base_rectangle)
{

	// when adding any new elements to the screen, 
	// one needs to draw the whole screen from scratch
	// load_background is tasked with that, clear the screen
	// draw the background image and the parking sensor outline
	load_background(FL_base_rectangle, FR_base_rectangle, BL_base_rectangle, BR_base_rectangle);

	// after the background has been drawn, draw
	// active parking sensor rectangles
	draw_active_rectangle(FR_base_rectangle);
	draw_active_rectangle(FL_base_rectangle);
	draw_active_rectangle(BR_base_rectangle);
	draw_active_rectangle(BL_base_rectangle);

	glutSwapBuffers();
}

void loadTexture()
{
	GLuint texture[N_OF_TEXTURES]; // declaring space for one texture
	int twidth = DEFAULT_INT, theight = DEFAULT_INT; // declaring variable for width and height of an image
	unsigned char* tdata = DEFAULT_PTR; // declaring pixel data
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

void populate_base_rectangle(float x, float y, float angle, struct rectangle* base_rectangle)
{
	// fill the base rectangle structure with data
	
	base_rectangle->width = BASE_RECTANGLE_WIDTH;
	base_rectangle->height = BASE_RECTANGLE_HEIGHT;
	base_rectangle->angle = angle;
	base_rectangle->x = x;
	base_rectangle->y = y;
	base_rectangle->distance = BASE_RECTANGLE_DEFAULT_DISTANCE;
	// using the RGBA color model, hence 4 bit array
	// R - red
	// G - green
	// B - blue
	// A - alpha (transparency)
	base_rectangle->rgba_color[0] = BASE_RECTANGLE_RED;
	base_rectangle->rgba_color[1] = BASE_RECTANGLE_GREEN;
	base_rectangle->rgba_color[2] = BASE_RECTANGLE_BLUE;
	base_rectangle->rgba_color[3] = BASE_RECTANGLE_ALPHA;
}