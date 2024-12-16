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


void draw_rectangle(struct rectangle*);
void draw_parking_sensors(struct rectangle*);
void draw_all_parking_sensors(struct rectangle*, struct rectangle*, struct rectangle*, struct rectangle*);


#endif