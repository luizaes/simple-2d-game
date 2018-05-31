#include "Player.h"
#include "Definitions.h"

Player::Player() {
	health = 500;
	score = 0;
	/* Inicial position for the square */ 
    x = (WIDTH/2-50.0);
    y = (HEIGHT/2-50.0);
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
	
	GLuint vertexBuffer;

    /* Rendering the square with dynamic values that change with the user's input */
    Vertex square[] = {
    	{0.0f + x, 0.0f + y, 0.0f},
    	{SQUARE_SIZE + x, 0.0f + y, 0.0f},
		{SQUARE_SIZE + x, SQUARE_SIZE + y, 0.0f},
		{0.0f + x, SQUARE_SIZE + y, 0.0f}
	};

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, coords));

    glColor3f(0.5f, 0.5f, 0.5f);
    glDrawArrays(GL_QUADS, 0, 4);

}