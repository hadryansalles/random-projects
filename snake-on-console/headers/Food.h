#ifndef FOOD_H
#define FOOD_H

#include "Snake.h"

#include <time.h>
#include <stdlib.h>

class Food {
private:

	int x; //position x of the food
	int y; //position y of the food

	int max_x; //max x possible to spawn food (screen width)
	int max_y; //max x possible to spawn food (screen height)

	char texture; //char that will be drawn on the screen to represent Food

public:

	//food constructor
	Food(char food_texture, int screen_width, int screen_height);
	//food random spawner (will not spawn food on the positions ocupped by Snake
	void spawnFood(Snake* snk);

	//returns true if x and y are coordenates of Food
	bool isFood(int x, int y);

	//returns food Texture
	char getTexture();
	//returns food position X
	int getX();
	//returns food positions y
	int getY();
};
#endif

