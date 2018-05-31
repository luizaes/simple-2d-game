class Player {

private:
	int health;
	int score;
	double x, y;

public:
	Player();
	int getHealth();
	double getX();
	double getY();
	void setX(double number);
	void setY(double number);
	void draw();
};