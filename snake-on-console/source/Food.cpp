#include "../Headers/Food.h"
Food::Food(char food_texture, int width, int height) {

	texture = food_texture;
	max_x = width;
	max_y = height;
}

void Food::spawnFood(Snake* snk) {

	srand(time(NULL));
	do {
		x = 1 + rand() % (max_x - 3);
		y = 1 + rand() % (max_y - 3);
	} while (snk->isSnake(x, y));
}

bool Food::isFood(int px, int py) {

	if (px == x && py == y)
		return true;

	return false;
}

char Food::getTexture() {
	return texture;
}

int Food::getX() {
	return x;
}

int Food::getY() {
	return y;
}