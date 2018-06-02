all:
	g++ main.cpp Particles.cpp Player.cpp -o game -lGL -lglfw -DGL_GLEXT_PROTOTYPES -I/usr/include/freetype2 -lftgl -lalut -lopenal