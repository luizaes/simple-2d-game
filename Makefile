all:
	g++ main.cpp Player.cpp -o game -lGL -lglfw -DGL_GLEXT_PROTOTYPES 