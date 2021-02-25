#include "../headers/Render.h"

Render::Render(int width_i, int height_i, char limit_right_left, char limit_top_bottom, char blank_space) {

	width = width_i;
	height = height_i;

	lr_limit = limit_right_left;
	tb_limit = limit_top_bottom;
	blank = blank_space;

	data = (char**)malloc(sizeof(char*) * height);
	for (int i = 0; i < height; i++)
		data[i] = (char*)malloc(sizeof(char) * width);
}

 void Render::destruct() {

	for (int i = 0; i < height; i++)
		free(data[i]);

	free(data);
}

void Render::updateWindow(Snake* snk, Food* food, System* eng) {

	system("cls"); 

	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {
			if (j == 0 || j == width - 1)
				printf("%c ", lr_limit);

			else if (i == 0 || i == height - 1) 
				printf("%c%c", tb_limit, tb_limit);

			else if (snk->isSnake(j, i))
				printf("%c ", snk->getTexture());

			else if (food->isFood(j, i))
				printf("%c ", food->getTexture());

			else
				printf("%c ", blank); 

		}
		printf("\n");
	}
	printf("Your score: %d\n", eng->getPoints());
}

int Render::getWidth() {
	return width;
}

int Render::getHeight() {
	return height;
}