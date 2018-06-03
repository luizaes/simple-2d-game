#include "Player.h"
#include "Definitions.h"

Player::Player() {
	health = 500;
	score = 0;
	/* Inicial position for the square */ 
    x = (WIDTH/2-50.0);
    y = (HEIGHT/2-50.0);
    moving = false;
}

int Player::getHealth() {
	return health;
}

double Player::getX() {
	return x;
}

double Player::getY() {
	return y;
}

void Player::setX(double number) {
	x = number;
}

void Player::setY(double number) {
	y = number;
}

void Player::setMoving(int dir, int act) {
	moving = true;
	direction = dir;
	action = act;
}

int Player::getScore() {
	return score;
}

void Player::setHealth(int number) {
	health += number;
}

void Player::setScore(int number) {
	score += number;
}

void Player::draw() {
	
	GLuint vertexBuffer, textureBuffer;
	ILuint image;
	ILboolean success;

    /* Rendering the square with dynamic values that change with the user's input */
    Vertex square[] = {
    	{{0.0f + x, 0.0f + y, 0.0f}, {1.0f, 1.0f}},
    	{{SQUARE_SIZE + x, 0.0f + y, 0.0f}, {0.0f, 1.0f}},
		{{SQUARE_SIZE + x, SQUARE_SIZE + y, 0.0f}, {0.0f, 0.0f}},
		{{0.0f + x, SQUARE_SIZE + y, 0.0f}, {1.0f, 0.0f}}
	};

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, coords));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, text));
 
    ilGenImages(1, &image); 
    ilBindImage(image); 

    if(action == GLFW_RELEASE) {
   		moving = false;
    }

    if(moving) {
    	if(direction == GLFW_KEY_LEFT) {
    		ilLoadImage("Images/adventurer_right_1.png");
    	} else if(direction == GLFW_KEY_RIGHT) {
    		ilLoadImage("Images/adventurer_left_1.png");
    	} else if(direction == GLFW_KEY_UP) {	
    		ilLoadImage("Images/adventurer_up_1.png");
    	} else if(direction == GLFW_KEY_DOWN) {
    		ilLoadImage("Images/adventurer_down_1.png");
    	}
    } else {
    	ilLoadImage("Images/adventurer_idle.png");
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glGenTextures(1, &textureBuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
    
    glDrawArrays(GL_QUADS, 0, 4);
    ilDeleteImages(1, &image);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}