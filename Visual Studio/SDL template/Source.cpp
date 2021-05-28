#include <iostream>
using namespace std;

#include <SDL.h>

//	********************
//	* window attributs *
//	********************
//	- position on screen
constexpr auto POS_X = 200;
constexpr auto POS_Y = 100;

//	- size on screen
constexpr auto WIDTH = 400;
constexpr auto HEIGHT = 400;

//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"

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

void quit_SDL() {
#pragma region SDL quit
	//	quit SDL
	SDL_Quit();
#pragma endregion
}

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("SLD template");

	//	****************************  //
	//	prepare usefull objects here  //
	//	****************************  //



	//	*********  //
	//	main loop  //
	//	*********  //
	while (true) {
		//	******************************  //
		//	draw image in rendering buffer  //
		//	******************************  //

		//	- clear window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical objects here



		//	****************  //
		//	event management  //
		//	****************  //

		//	- remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		//	- give event to objects for update if needed



		//	*********************  //
		//	show rendering buffer  //
		//	*********************  //
		SDL_RenderPresent(renderer);

		//	***********************  //
		//	check keypress for exit  //
		//	***********************  //
		if (event.type == SDL_KEYDOWN) {
			break;
		}
	}

	quit_SDL();

	return 0;
}