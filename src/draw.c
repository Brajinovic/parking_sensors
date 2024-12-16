#include "draw.h"



// function for drawing rectangles based on the rectangle structure
void draw_rectangle(struct rectangle* rect)
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
	glLoadIdentity();
}

// function for drawing the 3 "parking sensors"
void draw_parking_sensor_outline(struct rectangle* base_rectangle)
{
#if DEBUG == 1
	printf("draw parking sensors\n");
#endif
	// create a local copy of the base rectangle
	// in order not to modify the real object

	struct rectangle rectangle = *base_rectangle;
	rectangle.rgba_color[3] = FRAME_TRANSPARENCY / 100.0f;
	draw_rectangle(&rectangle);

	// draw the first, base rectangle
#if DEBUG_DRAW == 1
	printf(" draw first rectangle\n");
	printf(" x pos: %d\n", base_rectangle->x);
#endif		

	// calculate the X coordinate for the second rectangle
	// by using the offset that was above defined
	rectangle.x = rectangle.x + X_OFFSET;
	// calculate the Y coordinate for the second rectangle
	rectangle.y = rectangle.y + Y_OFFSET;
	// calculate the second rectangle width
	rectangle.width = rectangle.width + WIDTH_OFFSET;
	// calculate the second rectangle height
	rectangle.height = rectangle.height + HEIGHT_OFFSET;
	

#if DEBUG_DRAW == 1
	printf(" draw second rectangle\n");
	printf(" x pos: %d\n", rectangle.x);
#endif	

	draw_rectangle(&rectangle);

	// this code is similar to the code above, I am calculating new values for the third rectangle
	rectangle.x = rectangle.x + X_OFFSET;
	rectangle.y = rectangle.y + Y_OFFSET;
	rectangle.width = rectangle.width + WIDTH_OFFSET;
	rectangle.height = rectangle.height + HEIGHT_OFFSET;
#if DEBUG_DRAW == 1
	printf(" draw third rectangle\n");
	printf(" x pos: %d", rectangle.x);
#endif			
	draw_rectangle(&rectangle);

}

void draw_parking_sensors(struct rectangle* base_rectangle)
{
#if DEBUG == 1
	printf("draw parking sensors\n");
#endif
	// create a local copy of the base rectangle
	// in order not to modify the real object

	struct rectangle rectangle = *base_rectangle;

	if (rectangle.distance == 3)
	{
		// draw the first, base rectangle
#if DEBUG_DRAW == 1
	printf(" draw first rectangle\n");
	printf(" x pos: %d\n", base_rectangle->x);
#endif		
		draw_rectangle(&rectangle);
	} else
	{
		
	}

	// calculate the X coordinate for the second rectangle
	// by using the offset that was above defined
	rectangle.x = rectangle.x + X_OFFSET;
	// calculate the Y coordinate for the second rectangle
	rectangle.y = rectangle.y + Y_OFFSET;
	// calculate the second rectangle width
	rectangle.width = rectangle.width + WIDTH_OFFSET;
	// calculate the second rectangle height
	rectangle.height = rectangle.height + HEIGHT_OFFSET;

	if (rectangle.distance == 2)
	{

#if DEBUG_DRAW == 1
	printf(" draw second rectangle\n");
	printf(" x pos: %d\n", rectangle.x);
#endif	

		draw_rectangle(&rectangle);
	} else
	{

	}
	// this code is similar to the code above, I am calculating new values for the third rectangle
	rectangle.x = rectangle.x + X_OFFSET;
	rectangle.y = rectangle.y + Y_OFFSET;
	rectangle.width = rectangle.width + WIDTH_OFFSET;
	rectangle.height = rectangle.height + HEIGHT_OFFSET;
	if (rectangle.distance == 1)
	{
		
#if DEBUG_DRAW == 1
	printf(" draw third rectangle\n");
	printf(" x pos: %d", rectangle.x);
#endif			
		draw_rectangle(&rectangle);
	} else
	{

	}
}

void draw_all_parking_sensors(struct rectangle* FL_base_rectangle, struct rectangle* FR_base_rectangle, struct rectangle* BL_base_rectangle, struct rectangle* BR_base_rectangle)
{
	display();

	draw_parking_sensors(FR_base_rectangle);
	draw_parking_sensors(FL_base_rectangle);
	draw_parking_sensors(BR_base_rectangle);
	draw_parking_sensors(BL_base_rectangle);

	glutSwapBuffers();
}