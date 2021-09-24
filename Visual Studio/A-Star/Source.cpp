#include <iostream>
#include <time.h>
using namespace std;

#include <SDL.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 50, POS_Y = 50;
//constexpr auto WIDTH = 1800, HEIGHT = 900;
constexpr auto WIDTH = 1000, HEIGHT = 800;

//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "Grid.h"
#include "Cell.h"
#include "ListCells.h"
#include "AStar.h"
#include "Droid.h"
#include "BT.h"

SDL_Renderer* init_SDL(const char* title) {
#pragma region SDL initialization
	// SDL initialization
	SDL_Window* window = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : can't initialize SDL!" << endl;
		exit(0);
	}

	//SDL_ShowCursor(SDL_DISABLE);	//	hide mouse cursor
	SDL_ShowCursor(SDL_ENABLE);	//	show mouse cursor

	//	create the window and its associated renderer
	window = SDL_CreateWindow(title, POS_X, POS_Y, WIDTH, HEIGHT, 0);
	return SDL_CreateRenderer(window, 0, 0);
#pragma endregion
}

void clearWindow(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void showRenderingBuffer(SDL_Renderer* renderer) {
	SDL_RenderPresent(renderer);
}

SDL_Event getNextEvent() {
	SDL_Event event;
	SDL_PollEvent(&event);

	return event;
}

bool keypressed(SDL_Event& event, char key) {
	return event.type == SDL_KEYDOWN && event.key.keysym.sym == key;
}

void quit_SDL() {
#pragma region SDL quit
	//	quit SDL
	SDL_Quit();
#pragma endregion
}

//	entry point of application
int main(int argc, char** argv) {
	srand((unsigned int)time(nullptr));

	SDL_Renderer* renderer = init_SDL("SLD template");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	Grid grid(renderer, SDL_Rect{ 20, 20, WIDTH - 200, HEIGHT - 40 }, 30, 30);
	double precentOfWalls = 30.0;
	grid.createWalls(precentOfWalls);		//	percentage of walls
	grid.createTreasures(10);	//	number of treasures
	Timer timerSpawnTreasure(1000);

	Droid myDroid(&grid, grid.cellToPoint(grid.getRandomEmptyNonTreasureCell()), 15, 0.1);
	myDroid.getBehaviourTree();

	//	*********  //
	//	main loop  //
	//	*********  //
	bool endOfGame = false;
	while (!endOfGame) {
		//	******************************  //
		//	draw image in rendering buffer  //
		//	******************************  //
		clearWindow(renderer);

		/*	draw any desired graphical objects here	*/
		grid.draw();

		if (timerSpawnTreasure.isReady()) {
			grid.addTreasures(1);
		}

		myDroid.behave();
		myDroid.draw(renderer);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		if (keypressed(event, 'r')) {
			grid.createWalls(precentOfWalls);		//	percentage of walls
			grid.createTreasures(10);	//	number of treasures
		}
		endOfGame = keypressed(event, 'q');

		showRenderingBuffer(renderer);
	}

	quit_SDL();

	return 0;
}