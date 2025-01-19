/*
Notes by the creator:
Each parking sensor has 3 individual rectangles that indicate the distance to the external object that is about to make contact with the car. 

Instead of having 3 individual struct rectangle members for each sensor, I have only one, the base rectangle of the parking sensor and the other two are drawn based on the base one. that is why I have offsets defined with #define. In this way, instead of having 12 struct rectangle members. In this way, I am saving on memory and it is easier to handle.

Each struct rectangle member has a distance attribute which is used in the FSM tasked with drawing the required outlines and the active rectangles.

*/

#ifndef DRAW_H
#define DRAW_H

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"

#define WIDTH_OFFSET 38
#define HEIGHT_OFFSET -1
#define X_OFFSET -16
#define Y_OFFSET 21
#define TRANSPARENCY_OFFSET -30 // in percentages
#define ORDER_OFFSET 1

#define DEBUG_DRAW 1

struct keymap
{
	char far_key;
	char middle_key;
	char close_key;
	char clear_key;
};

struct rectangle{
	int x;
	int y;
	int distance;
	float width;
	float height;
	float angle;
	float rgba_color[4];
	struct keymap* keys;
};

unsigned char* loadPPM(const char* filename, int* width, int* height);
void load_background();

// Call this function when you want a specific rectangle to be drawn. All of the required info is contained inside the struct rectangle
void draw_rectangle(struct rectangle*);

// This function is used to calculate the parameters for the second and third rectangle (read the beginning of this header file)
//  of a parking sensor.
void calculate_rectangle_offset(struct rectangle* rectangle);

// This function draws the parking sensor rectangle outlines/ inactive rectangles.
// @ param struct rectangle* is the base rectangle pointer
void draw_parking_sensor_outline(struct rectangle*);

// This function draws one active rectangle. Which one is determined by the distance attribute.
// @ param struct rectangle* is the base rectangle pointer
void draw_active_rectangle(struct rectangle*);

// This function draws the background (display()), draws the active rectangles (draw_active_rectangle)
// and writes the changes to the screen(glutSwapoutBuffers())
void render_graphics(struct rectangle*, struct rectangle*, struct rectangle*, struct rectangle*);

void populate_base_rectangle(float x, float y, float angle, struct rectangle* base_rectangle);

void loadTexture();

#endif