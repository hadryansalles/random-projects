#ifndef RENDER_H
#define RENDER_H

#include "Snake.h"
#include "Food.h"
#include "System.h"

#include <stdio.h>

class Render {
private:

	int width; //screen width
	int height; //screen height

	char lr_limit; //texture of lateral border
	char tb_limit; //texture of top/bottom border
	char blank; //texture of blank space

	char** data; //"screen"

public:

	//Render constructor, initiates sizes and textures
	Render(int width_i, int height_i, char limit_right_left, char limit_top_bottom, char blank_space);
	//Render destructor, frees allocated memorie
	void destruct();

	//print all elementes in the screen on his positions
	void updateWindow(Snake* snk, Food* food, System* eng);

	//returns Screen Width
	int getWidth();
	//returns Screen Height
	int getHeight();
};
#endif 
