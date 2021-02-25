#include "../Headers/Render.h"
#include <stdio.h>
#include <stdlib.h>

#define SCR_WIDTH 20
#define SCR_HEIGHT 20
#define MAX_SCORE 30

int main() {

	bool quit = false;
	bool reset = false;

	System sys((double) 4, MAX_SCORE, 'w', 's', 'd', 'a'); //ticks per second, score max, move up, down, right, left
	Render ren(SCR_WIDTH, SCR_HEIGHT, '|', '_', ' '); // screen sizes, border and blank textures
	Snake snk(254, ren.getWidth(), ren.getHeight(), MAX_SCORE); //snake texture, screen sizes, max score
	Food f('+', ren.getWidth(), ren.getHeight()); //food texture, screen sizes
	
	sys.startGame(&snk, &f); //wait for user press any key

	while (!quit) {

		sys.startTime(); //start time counter for this tick
		snk.updatePositions(); //update snk position

		// if user reset game
		if (reset) {

			reset = false;
			sys.resetGame();
			snk.reset(ren.getWidth(), ren.getHeight());
			sys.startGame(&snk, &f); //wait for user press any key
		}

		//if snake have been ate the food
		if (snk.isSnake(f.getX(), f.getY())) {

			snk.increaseTail(f.getX(), f.getY());
			f.spawnFood(&snk);
			sys.increasePoints();
		}

		ren.updateWindow(&snk, &f, &sys); //clear screen and print all elements again
		
		//if snake hits himself or screen border
		if (snk.areColliding(ren.getWidth(), ren.getHeight())) {

			//checks if user want to play more
			quit = sys.gameOver();
			if (quit == false)
				reset = true;
			else break;
		}
		//if user reached max points
		else if (sys.getPoints() == snk.getMaxTails()) {

			//checks if user want to play more
			quit = sys.winGame();
			if (quit == false)
				reset = true;
			else break;
		}

		while (sys.getTimePassed() < sys.getTickRate()) //get user move//wait the time necessary for the next tick
			sys.getMove(&snk);
	}

	//cleans memory
	ren.destruct();
	snk.destruct();
	return 0;
}
