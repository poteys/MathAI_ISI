#include <iostream>
#include <time.h>
using namespace std;

#include <SDL.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 200, POS_Y = 50;
constexpr auto WIDTH = 1200, HEIGHT = 900;

//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "Grid.h"
#include "Cell.h"
#include "AStar.h"

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
	Point startPoint(WIDTH / 2, HEIGHT / 2, true);
	Point endPoint(WIDTH / 2, HEIGHT / 2, true);
	Grid grid(renderer, SDL_Rect{ 20, 20, 1160, 860 }, 70, 70, 2300);
	AStar aStar;

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

		Cell* startCell = grid.getCell(&startPoint);
		Cell* endCell = grid.getCell(&endPoint);
		if (startCell != nullptr && !grid.isWall(startCell->getRow(), startCell->getCol()) &&
			endCell != nullptr && !grid.isWall(endCell->getRow(), endCell->getCol())) {
			vector<Cell*> path = aStar.shortestPath(&grid, startCell, endCell);
			/*vector<Cell*> neighbours = grid.getNeighbours(cellOfMouse);
			if (neighbours.size() != 0) {
				for (Cell* neighbour : neighbours) {
					grid.drawCell(neighbour->getRow(), neighbour->getCol(), { 0, 180, 0, SDL_ALPHA_OPAQUE });
				}
			}
			grid.drawCell(cellOfMouse->getRow(), cellOfMouse->getCol(), { 0,255,0,SDL_ALPHA_OPAQUE });*/
		}
		startPoint.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), 10);
		endPoint.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), 10);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		startPoint.update(event);
		endPoint.update(event);

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();

	return 0;
}