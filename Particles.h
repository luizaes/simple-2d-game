#include "Definitions.h"

class Particles {

private:
	double x, y, velocity;
	int type, startPosition;
	bool alive;
	Screen screen;
	GLuint vertexBuff, textureBuffer;

public:
	Particles(double numX, double numY, double veloc, int ty, int start);
	double getX();
	double getY();
	double getVelocity();
	int getType();
	bool isAlive();
	void setState(bool state);
	void setX(double number);
	void setY(double number);
	void draw();
};