#include "../Headers/Snake.h"
#include <stdlib.h>

Snake::Snake(char snake_texture, int screen_width, int screen_height, int max_score) {

	head.x = screen_width / 2;
	head.y = screen_height / 2;

	head.vel_x = NULL; 
	head.vel_y = NULL;

	tail = (Tails*)malloc(sizeof(Tails) * (max_score));
	existing_tails = 0;
	max_tails = max_score;
	
	texture = snake_texture;
	last_move = NULL;
}

void Snake::reset(int screen_width, int screen_height) {

	head.x = screen_width / 2;
	head.y = screen_height / 2;

	head.vel_x = NULL;
	head.vel_y = NULL;

	free(tail);
	tail = (Tails*)malloc(sizeof(Tails) * (max_tails));
	existing_tails = 0;

	last_move = NULL;
}

void Snake::destruct() {

	free(tail);
}

char Snake::getTexture() {
	
	return texture;
}

int Snake::getMaxTails() {

	return max_tails;
}

bool Snake::isSnake(int px, int py) {

	if (px == head.x && py == head.y)
		return true;

	else if (existing_tails > 0) {
		
		for (int i = 0; i < existing_tails; i++) {

			if (px == tail[i].x && py == tail[i].y)
				return true;
		}
	}

	return false;
}

bool Snake::areColliding(int screen_width, int screen_height) {

	if (head.x == screen_width - 1 || head.y == screen_height - 1 || head.x == 0 || head.y == 0)
		return true;

	else if (existing_tails > 0) {

		for (int i = 0; i < existing_tails; i++) {

			if (head.x == tail[i].x && head.y == tail[i].y)
				return true;
		}
	}
	return false;
}

void Snake::increaseTail(int x, int y) {

	if (existing_tails == 0) {

		tail[0].x = head.x - head.vel_x;
		tail[0].y = head.y - head.vel_y;

		tail[0].vel_x = head.vel_x;
		tail[0].vel_y = head.vel_y;

		head.x = x;
		head.y = y;
	}
	else {

		tail[existing_tails].x = tail[existing_tails - 1].x - tail[existing_tails - 1].vel_x;
		tail[existing_tails].y = tail[existing_tails - 1].y - tail[existing_tails - 1].vel_y;

		tail[existing_tails].vel_x = tail[existing_tails - 1].vel_x;
		tail[existing_tails].vel_y = tail[existing_tails - 1].vel_y;

		head.x = x;
		head.y = y;
	}
	existing_tails++;
}

void Snake::updatePositions() {

	head.x += head.vel_x;
	head.y += head.vel_y;


	for (int i = 0; i < existing_tails; i++) {

		tail[i].x += tail[i].vel_x;
		tail[i].y += tail[i].vel_y;
	}

	if (existing_tails > 1) {

		for (int i = 0; i < existing_tails - 1; i++) {
			tail[existing_tails - 1 - i].vel_x = tail[existing_tails - 2 - i].vel_x;
			tail[existing_tails - 1 - i].vel_y = tail[existing_tails - 2 - i].vel_y;
		}
	}
	tail[0].vel_x = head.vel_x;
	tail[0].vel_y = head.vel_y;
}


void Snake::snakeUp() {

	if (last_move != 1) {

		head.vel_y = -1;
		head.vel_x = 0;
		last_move = 1;
	}
}

void Snake::snakeDown() {

	if (last_move != 1) {

		head.vel_y = 1;
		head.vel_x = 0;
		last_move = 1;
	}
}

void Snake::snakeRight() {

	if (last_move != 2) {

		head.vel_y = 0;
		head.vel_x = -1;
		last_move = 2;
	}
}

void Snake::snakeLeft() {

	if (last_move != 2) {

		head.vel_y = 0;
		head.vel_x = 1;
		last_move = 2;
	}
}
