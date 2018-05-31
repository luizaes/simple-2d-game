class Player {

private:
	int health;
	int score;
	double x, y;

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
	void draw();
};