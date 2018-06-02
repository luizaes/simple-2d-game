#include "Player.h"
#include "Particles.h"

/* Functions prototypes */
void render();
void update();
void displayScore();
void detectCollision(vector<Particles *>::iterator it);
void addParticle(int type);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/* Defining global variables*/
GLFWwindow* window;
Player *player1;
vector<Particles *> green, red, blue;
vector< vector<Particles *>::iterator > deleteLater;
Screen screen;

int main() {

	cout << "[Loading game]" << endl;

	srand(time(NULL));

	player1 = new Player();

    /* Initialize GLFW */
    if(!glfwInit()) {
        return -1;
    }

    /* Create a fullscreen window (1366x768) and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Dots", glfwGetPrimaryMonitor(), NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Setting the function for the user's inputs */
    glfwSetKeyCallback(window, keyCallback);

    glViewport(0.0f, 0.0f, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    /* Set screen limitations to detect collision */
    screen.top = HEIGHT-SQUARE_SIZE;
    screen.bottom = 0;
    screen.left = 0;
    screen.right = WIDTH-SQUARE_SIZE;

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)) {	       
       	render();
      	update();
      	displayScore();	
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    green.clear();
    red.clear();
    blue.clear();
    glfwTerminate();

	return 0;
}

/* Render the player and the squares */
void render() {

	vector<Particles *>::iterator it;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    player1->draw();

    for(it = green.begin(); it != green.end(); it++) {
    	(*it)->draw();
    	detectCollision(it);
    }
    for(it = red.begin(); it != red.end(); it++) {
    	(*it)->draw();
    	detectCollision(it);
    }
    for(it = blue.begin(); it != blue.end(); it++) {
    	(*it)->draw();
    	detectCollision(it);
    }

}

/* Delete particles that collided with the player and create new particles */
void update() {

	static int updateGreen;
	static int updateRed;
	static int updateBlue;
	vector< vector<Particles *>::iterator >::iterator it;

	if(updateGreen >= TYPE_GREEN_TIME_INT) {
		addParticle(TYPE_GREEN);
		updateGreen = 0;
	}
	if(updateRed >= TYPE_RED_TIME_INT) {
		addParticle(TYPE_RED);
		updateRed = 0;
	}
	if(updateBlue >= TYPE_BLUE_TIME_INT) {
		addParticle(TYPE_BLUE);
		updateBlue = 0;
	}

	updateGreen++;
	updateRed++;
	updateBlue++;

	for(it = deleteLater.begin(); it != deleteLater.end(); it++) {
    	
		if((**it)->getType() == TYPE_GREEN) {
			green.erase(*it);
		} else if((**it)->getType() == TYPE_RED) {
			red.erase(*it);
		} else if((**it)->getType() == TYPE_BLUE) {
			blue.erase(*it);
		}
    }
    deleteLater.clear();

}

/* Displays the player's score and health in the screen */
void displayScore() {

	char health[50];
	char score[50];

	sprintf(health, "Health: %d", player1->getHealth());
	sprintf(score, "Score: %d", player1->getScore());

	FTGLPixmapFont font("FreeSans.ttf");
	FTPoint coordHealth(50, 100, 0);
	FTPoint coordScore(50, 150, 0);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);
	font.FaceSize(24);
	font.Render(health, -1 , coordHealth);
	font.Render(score, -1 , coordScore);

	glPopAttrib();
}

/* Detects collisions between squares and the player
   TODO: Fix this function, because in certain cases it doesn't work properly.  
*/
void detectCollision(vector<Particles *>::iterator it) {
	if(!((*it)->getX() + PARTICLE_SIZE/2 <= player1->getX() - SQUARE_SIZE/2 || (*it)->getX() - PARTICLE_SIZE/2 >= player1->getX() + SQUARE_SIZE/2 || (*it)->getY() + PARTICLE_SIZE/2 <= player1->getY() - SQUARE_SIZE/2 || (*it)->getY() - PARTICLE_SIZE/2 >= player1->getY() + SQUARE_SIZE/2)) {
   		(*it)->setState(false);
   		if((*it)->getType() == TYPE_GREEN) {
   			player1->setScore(100);
   			deleteLater.push_back(it);
   		} else if((*it)->getType() == TYPE_RED) {
   			player1->setHealth(-100);
   			deleteLater.push_back(it);
   		} else if((*it)->getType() == TYPE_BLUE) {
   			player1->setHealth(100);
   			deleteLater.push_back(it);
   		}

   		if(player1->getHealth() <= 0) {
   			cout << "GAME OVER" << endl;
   			glfwSetWindowShouldClose(window, GL_TRUE);
   		}
    }
}

/* Adds new particles in the game, 
   generating random values to choose which side and position of the screen the particle is gonna come from 
*/
void addParticle(int type) {

	int direction, velocity;
	double posX, posY;

	direction = rand() % 4;
	if(direction == POS_TOP) {
		posY = HEIGHT-PARTICLE_SIZE;
		posX = rand() % (WIDTH-PARTICLE_SIZE);
	} else if(direction == POS_BOTTOM) {
		posY = 0;
		posX = rand() % (WIDTH-PARTICLE_SIZE);
	} else if(direction == POS_LEFT) {
		posY = rand() % (HEIGHT-PARTICLE_SIZE);
		posX = 0;
	} else if(direction == POS_RIGHT) {
		posY = rand() % (HEIGHT-PARTICLE_SIZE);
		posX = WIDTH-PARTICLE_SIZE;
	}

	velocity = 5;

	if(type == TYPE_GREEN) {
		green.push_back(new Particles(posX, posY, velocity, type, direction));
	} else if(type == TYPE_RED) {
		red.push_back(new Particles(posX, posY, velocity, type, direction));
	} else if(type == TYPE_BLUE) {
		blue.push_back(new Particles(posX, posY, velocity, type, direction));
	}

}

/* Input treatment:
		Escape key - close game;
		Arrow keys - move the square.
*/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT) && player1->getX() < screen.right) {
    	player1->setX(player1->getX()+10);
    } else if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT) && player1->getX() >= screen.left) {
    	player1->setX(player1->getX()-10);
    } else if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT) && player1->getY() < screen.top) {
    	player1->setY(player1->getY()+10);
    } else if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT) && player1->getY() >= screen.bottom) {
    	player1->setY(player1->getY()-10);
    }

}
