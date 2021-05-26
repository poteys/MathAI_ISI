#include <iostream>
using namespace std;

#include <SDL.h>

//	size of window on screen
constexpr auto WIDTH = 400;
constexpr auto HEIGHT = 400;

//	include desired header files for libraries
//	example : #include "../lib_Point/Point.h"

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

	//	create the window and its associated renderer
	fenetre = SDL_CreateWindow("SDL template", 200, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);
#pragma endregion

	//	****************************  //
	//	prepare usefull objects here  //
	//	****************************  //



	//	**************  //
	//	main loop here  //
	//	**************  //
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

#pragma region SDL quit
	//	destroy window and quit SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
#pragma endregion

	return 0;
}