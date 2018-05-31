#include "Player.h"
#include <GL/gl.h>

Player::Player() {
	health = 500;
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
