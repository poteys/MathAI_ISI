#include <iostream>
using namespace std;

#include <SDL.h>
#include "../lib_Font/Font.h"

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 200, POS_Y = 100;
constexpr auto WIDTH = 400, HEIGHT = 400;

//	include desired header files for libraries
#include "../lib_Point/Point.h"

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
	SDL_Renderer* renderer = init_SDL("SLD template");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	Point p1(WIDTH / 2, HEIGHT / 3, true);
	Point p2(WIDTH / 2, 2*HEIGHT / 3, true);
	Font font1("C:\\Windows\\Fonts\\Verdana.ttf", 24);
	Font font2;

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
		font1.print(renderer, (int)p1.x, (int)p1.y, "Move me!", { 255, 255, 255, SDL_ALPHA_OPAQUE }, true);
		p1.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 8);
		font2.print(renderer, (int)p2.x, (int)p2.y, "Move me!", { 255, 255, 255, SDL_ALPHA_OPAQUE }, false);
		p2.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 8);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		p1.update(event);
		p2.update(event);

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	//Font::quit();
	quit_SDL();

	return 0;
}