#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include <freetype2/ft2build.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <IL/il.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>

using namespace std;

/* Defining the width and height of the screen */
#define WIDTH 1366
#define HEIGHT 768

#define TYPE_GREEN 4
#define TYPE_RED 5
#define TYPE_BLUE 6
#define POS_TOP 0
#define POS_BOTTOM 1
#define POS_LEFT 2
#define POS_RIGHT 3
#define WINDOW_STATE_MENU 7
#define WINDOW_STATE_GAME_OVER 8
#define WINDOW_STATE_PLAYING 9
#define PARTICLE_SIZE 50
#define SQUARE_SIZE 100
#define TYPE_GREEN_TIME_INT 100
#define TYPE_RED_TIME_INT 200
#define TYPE_BLUE_TIME_INT 1000

/* Defining the vertex struct that represents the square (the player) */
typedef struct {
	GLfloat coords[3];
	GLfloat text[2];
} Vertex;

typedef struct {
	int top;
	int bottom;
	int left;
	int right;
} Screen;

#endif