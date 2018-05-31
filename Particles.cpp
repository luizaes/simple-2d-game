#include "Particles.h"
#include "Definitions.h"

Particles::Particles(double numX, double numY, double veloc, int ty, int start) {
	velocity = veloc;
	type = ty;
	alive = true;
	startPosition = start;
	x = numX;
	y = numY;

	/* Set screen limitations to detect collision */
    screen.top = HEIGHT;
    screen.bottom = -10;
    screen.left = -10;
    screen.right = WIDTH;
}

bool Particles::isAlive() {
	return alive;
}

void Particles::setState(bool state) {
	alive = state;
}

double Particles::getVelocity() {
	return velocity;
}

double Particles::getX() {
	return x;
}

double Particles::getY() {
	return y;
}

void Particles::setX(double number) {
	x = number;
}

void Particles::setY(double number) {
	y = number;
}

int Particles::getType() {
	return type;
}

void Particles::draw() {

	GLuint vertexBuff;
	Vertex square[] = {
   		{x, y, 0.0f},
   		{x + PARTICLE_SIZE, y, 0.0f},
		{x + PARTICLE_SIZE, y + PARTICLE_SIZE, 0.0f},
		{x, y + PARTICLE_SIZE, 0.0f}
	};

	if(alive) {

		if(startPosition == POS_TOP) {
	    	y -= velocity;
	    } else if(startPosition == POS_BOTTOM) {
	    	y += velocity;
	    } else if(startPosition == POS_LEFT) {
	    	x += velocity;
	    } else if(startPosition == POS_RIGHT) {
	    	x -= velocity;
	    }

	    if(x < screen.left || x > screen.right || y < screen.bottom || y > screen.top) {
	    	setState(false);
	    }

	    glGenBuffers(1, &vertexBuff);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	    glEnableClientState(GL_VERTEX_ARRAY);
	    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, coords));

	    if(type == TYPE_GREEN) {
	    	glColor3f(0.0f, 1.0f, 0.0f);
	    } else if(type == TYPE_RED) {
	    	glColor3f(1.0f, 0.0f, 0.0f);
	    } else if(type == TYPE_BLUE) {
	    	glColor3f(0.0f, 0.0f, 1.0f);
	    }
	 	
	    glDrawArrays(GL_QUADS, 0, 4);
	}
  
}