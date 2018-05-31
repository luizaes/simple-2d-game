#include <iostream>
#include <GLFW/glfw3.h>
#include "Player.h"

using namespace std;

/* Defining the width and height of the screen */
#define WIDTH 1366
#define HEIGHT 768

/* Functions prototypes */
void render();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/* Defining global variables*/
GLFWwindow* window;
Player Player1;

/* Defining the vertex struct that represents the square (the player) */
typedef struct {
	GLfloat coords[3];
} Vertex;

int main() {

	cout << "[Loading game]" << endl;

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

    /* Inicial position for the square */ 
    Player1.setX(WIDTH/2-50.0);
    Player1.setY(HEIGHT/2-50.0);

    glViewport(0.0f, 0.0f, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)) {
        
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

	return 0;
}

void render() {

	GLuint vertexBuffer;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Rendering the square with dynamic values that change with the user's input */
    Vertex square[] = {
    	{0.0f + Player1.getX(), 0.0f + Player1.getY(), 0.0f},
    	{100.0f + Player1.getX(), 0.0f + Player1.getY(), 0.0f},
		{100.0f + Player1.getX(), 100.0f + Player1.getY(), 0.0f},
		{0.0f + Player1.getX(), 100.0f + Player1.getY(), 0.0f}
	};

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, coords));

    glColor3f(0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_QUADS, 0, 4);
    
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	/* Input treatment:
		Escape key - close game;
		Arrow keys - move the square.
	 */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    	Player1.setX(Player1.getX()+10);
    } else if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    	Player1.setX(Player1.getX()-10);
    } else if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    	Player1.setY(Player1.getY()+10);
    } else if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    	Player1.setY(Player1.getY()-10);
    }

}