#include <iostream>
using namespace std;

#include <SDL.h>

//	size of window on screen
constexpr auto WIDTH = 600;
constexpr auto HEIGHT = 600;

//	include desired libraries if needed
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"

//	entry point of application
int main(int argc, char** argv) {

#pragma region SDL initialization
	// SDL initialization
	SDL_Window* fenetre = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : can't initialize SDL!" << endl;
		exit(0);
	}

	//SDL_ShowCursor(SDL_DISABLE);	//	hide mouse cursor
	SDL_ShowCursor(SDL_ENABLE);	//	show mouse cursor

	//	create the window and its renderer
	fenetre = SDL_CreateWindow("exemple 1", 1920 + 200, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);
#pragma endregion

	//	prepare usefull objects here
	//	****************************
	Point point(WIDTH / 2, HEIGHT / 2, true);
	Slider slider(WIDTH - 250, HEIGHT - 50, 200, 20, 300, 100);

	//	main loop here
	//	**************
	while (true) {
		//	draw image
		//	**********

		//	- clear window
		SDL_SetRenderDrawColor(renderer, 50, 50, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical object
		Color color(0, 255, 0, SDL_ALPHA_OPAQUE);
		point.draw(renderer, color, 5);
		point.drawCircle(renderer, (int)slider.getValue(), color, true);

		//	event management
		//	****************

		//	remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		//	give event to objects for update
		point.update(event);
		slider.draw(renderer, event);

		//	show rendering buffer
		//	*********************
		SDL_RenderPresent(renderer);

		//	check keypress for exit
		//	***************************
		if (event.type == SDL_KEYDOWN) {
			break;
		}
	}

#pragma region SDL quit
	//	destroy window and quit SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
#pragma endregion

	return 0;
}