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

    Vertex square[] = {
    	{{x, y, 0.0f}, {1.0f, 1.0f}},
    	{{x + PARTICLE_SIZE, y, 0.0f}, {0.0f, 1.0f}},
		{{x + PARTICLE_SIZE, y + PARTICLE_SIZE, 0.0f}, {0.0f, 0.0f}},
		{{x, y + PARTICLE_SIZE, 0.0f}, {1.0f, 0.0f}}
	};

    glGenBuffers(1, &vertexBuff);
    glGenTextures(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_DYNAMIC_DRAW);


    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, coords));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, text));

	ILuint image;

	ilGenImages(1, &image); 
    ilBindImage(image);

    if(type == TYPE_GREEN) {
    	ilLoadImage("Images/coin.png");
    } else if(type == TYPE_RED) {
    	ilLoadImage("Images/fire.png");
    } else if(type == TYPE_BLUE) {
    	ilLoadImage("Images/heart.png");
    }
    
    glBindTexture(GL_TEXTURE_2D, textureBuffer); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
    ilDeleteImages(1, &image);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, 4);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
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

	ILboolean success;
	ILuint image;

	Vertex square[] = {
    	{{x, y, 0.0f}, {1.0f, 1.0f}},
    	{{x + PARTICLE_SIZE, y, 0.0f}, {0.0f, 1.0f}},
		{{x + PARTICLE_SIZE, y + PARTICLE_SIZE, 0.0f}, {0.0f, 0.0f}},
		{{x, y + PARTICLE_SIZE, 0.0f}, {1.0f, 0.0f}}
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
	    	glDeleteBuffers(1, &vertexBuff);
	    	glDeleteTextures(1, &textureBuffer);
	    } else {
		    glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);
		    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_DYNAMIC_DRAW);

		    glEnableClientState(GL_VERTEX_ARRAY);
		    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, coords));
		    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	    	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, text));

	    	glBindTexture(GL_TEXTURE_2D, textureBuffer);

		    glEnable(GL_TEXTURE_2D);
		    glEnable(GL_BLEND);
		    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		   
		    
		    glDrawArrays(GL_QUADS, 0, 4);
		    
		    glDisable(GL_TEXTURE_2D);
		    glDisable(GL_BLEND);
	    }

	}
  
}