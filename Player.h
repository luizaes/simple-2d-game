#include "Definitions.h"

class Player {

private:
	int health, score, action, direction;
	double x, y;
	bool moving;
	GLuint vertexBuffer, textureBuffer;

public:
	Player();
	int getHealth();
	int getScore();
	GLuint * getVertexBuffer();
	GLuint * getTextureBuffer();
	void setHealth(int number);
	void setScore(int number);
	double getX();
	double getY();
	void setX(double number);
	void setY(double number);
	void setMoving(int dir, int act);
	void draw();
};