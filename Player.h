class Player {

private:
	int health, score, action, direction;
	double x, y;
	bool moving;

public:
	Player();
	int getHealth();
	int getScore();
	void setHealth(int number);
	void setScore(int number);
	double getX();
	double getY();
	void setX(double number);
	void setY(double number);
	void setMoving(int dir, int act);
	void draw();
};