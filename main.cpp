#include "Player.h"
#include "Particles.h"

/* Functions prototypes */
void renderGame();
void renderMenu();
void drawTitle();
void renderOverScreen();
void updateGame();
void drawGameOverText();
void displayScore();
void drawBackground();
void detectCollision(vector<Particles *>::iterator it);
void addParticle(int type);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/* Defining global variables*/
GLFWwindow* window;
Player *player1;
vector<Particles *> green, red, blue;
vector< vector<Particles *>::iterator > deleteLater;
Screen screen;
GLuint vertexBuffMain, textureBufferMain;
ALuint sources[4];
int gameWindowState;

int main() {

	cout << "[Loading game]" << endl;

	srand(time(NULL));

	player1 = new Player();

	/* Loading OpenAL sounds */
	ALCdevice *device;
	ALCcontext *context;
	ALuint buffers[4];
	ALint source_state;
	ALsizei size, freq;
	ALenum format;
	ALvoid *data;
	ALboolean loop = AL_FALSE;

	device = alcOpenDevice(NULL);
	if (!device) {
	    return -1;
	}

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		return -1;    
	}
	
	alGenSources((ALuint)4, sources);
	alGenBuffers((ALuint)4, buffers);

	alutLoadWAVFile((ALbyte*)"Sounds/Bit Quest.wav", &format, &data, &size, &freq, &loop);
	alBufferData(buffers[0], format, data, size, freq);
	alSourcei(sources[0], AL_BUFFER, buffers[0]);

	alutLoadWAVFile((ALbyte*)"Sounds/sfx_coin_double7.wav", &format, &data, &size, &freq, &loop);
	alBufferData(buffers[1], format, data, size, freq);
	alSourcei(sources[1], AL_BUFFER, buffers[1]);

	alutLoadWAVFile((ALbyte*)"Sounds/sfx_exp_shortest_soft7.wav", &format, &data, &size, &freq, &loop);
	alBufferData(buffers[2], format, data, size, freq);
	alSourcei(sources[2], AL_BUFFER, buffers[2]);

	alutLoadWAVFile((ALbyte*)"Sounds/sfx_sounds_powerup10.wav", &format, &data, &size, &freq, &loop);
	alBufferData(buffers[3], format, data, size, freq);
	alSourcei(sources[3], AL_BUFFER, buffers[3]);

    /* Initialize GLFW */
    if(!glfwInit()) {
        return -1;
    }

	ilInit();

    /* Create a fullscreen window (1366x768) and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Game", glfwGetPrimaryMonitor(), NULL);
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

    alSourcei(sources[0], AL_LOOPING, 1);
    alSourcePlay(sources[0]);

    gameWindowState = WINDOW_STATE_MENU;

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)) {	  

    	if(gameWindowState == WINDOW_STATE_MENU) {
    		while(gameWindowState == WINDOW_STATE_MENU) {
    			renderMenu();
    			glfwSwapBuffers(window);
		        glfwPollEvents();
    		}
    	} else if(gameWindowState == WINDOW_STATE_PLAYING) {
    		while(gameWindowState == WINDOW_STATE_PLAYING) {
    			renderGame();
		      	updateGame();
		      	displayScore();	
		        glfwSwapBuffers(window);
		        glfwPollEvents();
    		}
    	} else if(gameWindowState == WINDOW_STATE_GAME_OVER) {
    		renderOverScreen();
    		glfwSwapBuffers(window);
		    glfwPollEvents();
    	}

    }

    glDeleteBuffers(1, &vertexBuffMain);
	glDeleteTextures(1, &textureBufferMain);
    alDeleteSources(4, sources);
	alDeleteBuffers(4, buffers);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
    green.clear();
    red.clear();
    blue.clear();
    deleteLater.clear();
    glfwTerminate();

	return 0;
}
/*
void drawBackground() {

	ILuint image;
	ILboolean success;

	Vertex screen[] = {
    	{{0.0f, HEIGHT, 0.0f}, {0.0f, 0.0f}},
    	{{WIDTH, HEIGHT, 0.0f}, {1.0f, 0.0f}},
		{{WIDTH, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
	};

    glGenBuffers(1, &vertexBuffMain);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffMain);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screen), screen, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, coords));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, text));

	ilGenImages(1, &image); 
    ilBindImage(image);

    ilLoadImage("Images/background.png");

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glGenTextures(1, &textureBufferMain);
    glBindTexture(GL_TEXTURE_2D, textureBufferMain); 

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
*/
/* Render the menu screen */
void renderMenu() {

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //drawBackground();
    drawTitle();

}

void drawTitle() {

	char title[50];
	char message[50];

	sprintf(title, "DOTS");
	sprintf(message, "PRESS ENTER TO START");

	FTGLPixmapFont font("8-Bit.ttf");
	FTPoint coordTitle(420, 380, 0);
	FTPoint coordMessage(400, 330, 0);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);
	font.FaceSize(60);
	//font.Render(title, -1 , coordTitle);
	font.Render(message, -1 , coordMessage);

	glPopAttrib();

}

/* Render the game over screen */
void renderOverScreen() {

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //drawBackground();
    drawGameOverText();

}

void drawGameOverText() {

	char score[50];
	char message[50];

	sprintf(message, "GAME OVER");
	sprintf(score, "Final Score: %d", player1->getScore());

	FTGLPixmapFont font("8-Bit.ttf");
	FTPoint coordMessage(535, 380, 0);
	FTPoint coordScore(475, 330, 0);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);
	font.FaceSize(60);
	font.Render(message, -1 , coordMessage);
	font.Render(score, -1 , coordScore);

	glPopAttrib();
}

/* Render the player and the squares */
void renderGame() {

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
void updateGame() {

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

	FTGLPixmapFont font("8-Bit.ttf");
	FTPoint coordHealth(50, 100, 0);
	FTPoint coordScore(50, 150, 0);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);
	font.FaceSize(30);
	font.Render(health, -1 , coordHealth);
	font.Render(score, -1 , coordScore);

	glPopAttrib();
}

/* Detects collisions between squares and the player 
*/
void detectCollision(vector<Particles *>::iterator it) {
	if(!((*it)->getX() + PARTICLE_SIZE/2 <= player1->getX() - SQUARE_SIZE/2 || (*it)->getX() - PARTICLE_SIZE/2 >= player1->getX() + SQUARE_SIZE/2 || (*it)->getY() + PARTICLE_SIZE/2 <= player1->getY() - SQUARE_SIZE/2 || (*it)->getY() - PARTICLE_SIZE/2 >= player1->getY() + SQUARE_SIZE/2)) {
   		(*it)->setState(false);
   		if((*it)->getType() == TYPE_GREEN) {
   			alSourcePlay(sources[1]);
   			player1->setScore(100);
   			deleteLater.push_back(it);
   			(*it)->setState(false);
   		} else if((*it)->getType() == TYPE_RED) {
   			alSourcePlay(sources[2]);
   			player1->setHealth(-100);
   			deleteLater.push_back(it);
   			(*it)->setState(false);
   		} else if((*it)->getType() == TYPE_BLUE) {
   			alSourcePlay(sources[3]);
   			player1->setHealth(100);
   			deleteLater.push_back(it);
   			(*it)->setState(false);
   		}

   		if(player1->getHealth() <= 0) {
   			//cout << "[GAME OVER]" << endl;
   			gameWindowState = WINDOW_STATE_GAME_OVER;
   			glDeleteBuffers(1, player1->getVertexBuffer());
			glDeleteTextures(1, player1->getTextureBuffer());
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
        gameWindowState = WINDOW_STATE_GAME_OVER;
    } else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT) && player1->getX() < screen.right) {
    	player1->setX(player1->getX()+10);
    	player1->setMoving(key, action);
    } else if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT) && player1->getX() >= screen.left) {
    	player1->setX(player1->getX()-10);
    	player1->setMoving(key, action);
    } else if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT) && player1->getY() < screen.top) {
    	player1->setY(player1->getY()+10);
    	player1->setMoving(key, action);
    } else if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT) && player1->getY() >= screen.bottom) {
    	player1->setY(player1->getY()-10);
    	player1->setMoving(key, action);
    } else if((key == GLFW_KEY_DOWN || key == GLFW_KEY_UP || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE) {
    	player1->setMoving(key, action);
    } else if(key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
    	gameWindowState = WINDOW_STATE_PLAYING;
    }

}
