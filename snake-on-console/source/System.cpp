#include "../Headers/System.h"

System::System(double tick_rate, int max_score, char move_up, char move_down, char move_left, char move_right) {

	t_r = 1000.0/tick_rate;
	_t_r = 1000.0/tick_rate;
	str = 0;
	time_passed = 0;
	max_points = max_score;
	points = 0;

	m_u = move_up;
	m_d = move_down;
	m_r = move_right;
	m_l = move_left;
}

void System::startGame(Snake* snk, Food* food) {

	printf("Press any button to start the game.");
	_getch();
	food->spawnFood(snk);
}

void System::resetGame() {
	
	t_r = _t_r;
	str = 0;
	time_passed = 0;
	points = 0;
}

bool System::winGame() {

	char press = 0;
	system("cls");
	printf("Congratulations, you reached the max score (%d points).\n", max_points);
	printf("Press Q to Quit or R to Restart.\n");

	while (press != 'q' && press != 'r' && press != 'Q' && press != 'R')
		press = _getch();

	if (press == 'q' || press == 'Q')
		return true;

	return false;
}

bool System::gameOver() {

	char press = 0;

	system("cls");
	printf("Game over, you loose with %d points.\n", points);
	printf("Press Q to Quit or R to Restart.\n");
	while (press != 'q' && press != 'r' && press != 'Q' && press != 'R')
		press = _getch();

	if (press == 'q' || press == 'Q')
		return true;

	return false;
}

void System::startTime() {

	str = clock();
}

double System::getTimePassed() {

	time_passed = (clock() - str) * 1000 / CLOCKS_PER_SEC;
	return(time_passed);
}

double System::getTickRate() {

	return t_r;
}

int System::getPoints() {

	return points;
}

void System::increasePoints() {

	points++;
	t_r -= 5;
}

void System::getMove(Snake* snk) {

	char user_move;
	if (_kbhit()) {

		user_move = _getch();

		if (user_move == m_u)
			snk->snakeUp();

		else if (user_move == m_d)
			snk->snakeDown();

		else if (user_move == m_r)
			snk->snakeRight();

		else if (user_move == m_l)
			snk->snakeLeft();
	}
}
