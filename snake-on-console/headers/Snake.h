#ifndef SNAKE_H
#define SNAKE_H

class Snake {
private:

	struct Head { //part of snake responsible for the movement and for eat food

		int x;
		int y;

		//defines movement direction
		int vel_x;
		int vel_y;
	};
	struct Tails { //part of snake that follow Head

		int x;
		int y;

		int vel_x;
		int vel_y;
	};

	Head head;
	Tails* tail;

	char texture; //char that will be draw on the screen to represent Snake (Head an Tails)
	int existing_tails; //number of tails at moment
	int max_tails; //number max of tails
	int last_move; //indicates the last movement direction

public:

	//Snake constructor 
	Snake(char snake_texture, int screen_width, int screen_height, int max_score);
	//Reset Snake's values for intials
	void reset(int screen_width, int screen_height);
	//Snake destructor
	void destruct();

	//Returns Snake texture for rendering
	char getTexture();
	//Returns number max of tails
	int getMaxTails();
	//Checks if px and py are coordenates of any Snake's Parts (Head or Tails)
	bool isSnake(int px, int py);
	//Checks if Snake's Head are colliding with any Tail, or Screen's borders
	bool areColliding(int screen_width, int screen_height);

	//Increases in 1 the number of tails, and give for it the position of last tails and its velocity.
	//x and y are positions that head have to move for
	void increaseTail(int x, int y);
	//Updates Positions and Velocities of all Snake's parts 
	void updatePositions();

	//sets Head's movement direction for Up
	void snakeUp();
	//sets Head's movement direction for Down
	void snakeDown();
	//sets Head's movement direction for Right
	void snakeRight();
	//sets Head's movement direction for Left
	void snakeLeft();
};
#endif 

