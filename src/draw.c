#include "draw.h"


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

	draw_rectangle(&rectangle);

	calculate_rectangle_offset(&rectangle);
	draw_rectangle(&rectangle);

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
	load_background();

	draw_active_rectangle(FR_base_rectangle);
	draw_active_rectangle(FL_base_rectangle);
	draw_active_rectangle(BR_base_rectangle);
	draw_active_rectangle(BL_base_rectangle);

	glutSwapBuffers();
}