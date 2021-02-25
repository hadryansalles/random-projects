#ifndef SYSTEM_H
#define SYSTEM_H

#include "Snake.h"
#include "Food.h"

#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

class System {
private:

	double t_r; //tick rate, number of updtes per second
	double _t_r; //static tick rate
	clock_t str; //time that last tick have beggining
	double time_passed; //time passed since last tick

	int points; //points reached
	int max_points; //score max

	char m_u; //key buttons for movements. Up
	char m_d; // Down
	char m_r; // Right
	char m_l; // Left

public:

	//Engine constructor, sets initial values
	System(double tick_rate, int score_max, char move_up, char move_down, char move_left, char move_right);
	//Waits user press any button for start the game and spawn first Food
	void startGame(Snake* snk, Food* food);
	//Resets all values for itials
	void resetGame();
	//Screen message for when user reach max points, return true if user quit the game 
	bool winGame();
	//Screen message for when user hits the wall or eat himself, return true if user quit the game
	bool gameOver();

	//start time counter
	void startTime();
	//returns time passed since start timer
	double getTimePassed();
	//returns tick rate
	double getTickRate();

	//returns user points at moment
	int getPoints();
	//increase in 1 number of user points and increase the tick rate for looks like snake is faster
	void increasePoints();

	//Recieve user movement
	void getMove(Snake* snk);
};
#endif 
