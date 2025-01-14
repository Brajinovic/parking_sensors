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
	
	char buf[3][IMAGE_BUFFER_SIZE];
	char* retval_fgets = DEFAULT_PTR;
	
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("error reading ppm file, could not locate %s", filename);
		*width = 0;
		*height = 0;

		return_value = FAIL;
	}
	retval_fgets = fgets(buf[0], IMAGE_BUFFER_SIZE, fp);
	do
	{
		retval_fgets = fgets(buf[0], IMAGE_BUFFER_SIZE, fp);
	} while (buf[0][0] == '#');
	
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	*width = atoi(buf[1]);
	*height = atoi(buf[2]);
	
	do
	{
		retval_fgets = fgets(buf[0], IMAGE_BUFFER_SIZE, fp);
	} while (buf[0][0] == '#');

	rawData = (char*)calloc(*width * *height, 3);
	read = fread(rawData, (*width) * (*height) * 3, 1, fp);
	fclose(fp);

	if (read != 1)
	{
		printf("error parsing ppm file, incomplete data\n");
		memset(rawData, 0, *width * *height * 3);
		*width = 0;
		*height = 0;

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
	glTexCoord2f(0, 1); glVertex3f(0, WINDOW_HEIGHT * 0.75, 0);
	glTexCoord2f(1, 1); glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT * 0.75, 0);
	glTexCoord2f(1, 0); glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT * 0.25, 0);
	glTexCoord2f(0, 0); glVertex3f(0, WINDOW_HEIGHT * 0.25, 0);
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
		glVertex3f(0.0f, 0.0f, 0.0f);   
		glVertex3f(rect->width, 0.0f, 0.0f);
		glVertex3f(rect->width, rect->height, 0.0f);
		glVertex3f(0.0f, rect->height, 0.0f);	
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

	if (rectangle.distance == 1)
	{
		draw_rectangle(&rectangle);
	} else
	{	
	}

	calculate_rectangle_offset(&rectangle);

	if (rectangle.distance == 2)
	{
		draw_rectangle(&rectangle);
	} else
	{
	}

	calculate_rectangle_offset(&rectangle);
	
	if (rectangle.distance == 3)
	{	
		draw_rectangle(&rectangle);
	} else
	{
	}
}

void draw_all_parking_sensors(struct rectangle* FL_base_rectangle, struct rectangle* FR_base_rectangle, struct rectangle* BL_base_rectangle, struct rectangle* BR_base_rectangle)
{
	load_background(FL_base_rectangle, FR_base_rectangle, BL_base_rectangle, BR_base_rectangle);

	draw_active_rectangle(FR_base_rectangle);
	draw_active_rectangle(FL_base_rectangle);
	draw_active_rectangle(BR_base_rectangle);
	draw_active_rectangle(BL_base_rectangle);

	glutSwapBuffers();
}

void loadTexture()
{
	GLuint texture[1]; // declaring space for one texture
	int twidth = 0, theight = 0; // declaring variable for width and height of an image
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
	// create the base rectangle structure and fill it with data
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