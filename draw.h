#ifndef DRAW_H
#define DRAW_H

#include <GL/glut.h>
#include <stdio.h>

#define WIDTH_OFFSET 38
#define HEIGHT_OFFSET -1
#define X_OFFSET -16
#define Y_OFFSET 21
#define TRANSPARENCY_OFFSET -30 // percentage
#define ORDER_OFFSET 1

#define DEBUG_DRAW 0

struct rectangle{
	int x;
	int y;
	int order;
	float width;
	float height;
	float angle;
	float rgba_color[4];
};

// located in main.c

extern struct rectangle* FR_base_rectangle;
extern struct rectangle* FL_base_rectangle;
extern struct rectangle* BR_base_rectangle;
extern struct rectangle* BL_base_rectangle;

void draw_rectangle_struct(struct rectangle*);
void draw_parking_sensors();

#endif