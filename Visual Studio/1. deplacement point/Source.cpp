#include <iostream>
using namespace std;

#include <SDL.h>

constexpr auto MAX_OBJECTS = 150;

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = -1000;	// -1280 + 200;
constexpr auto POS_Y = 100;
constexpr auto WIDTH = 600, HEIGHT = 400;

//	include desired header files for libraries
#include "GameObject.h"
#include <time.h>

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
	GameObject object(5, Point(0, HEIGHT / 2), Vector(100, 0), WIDTH, HEIGHT);

	GameObject* tabObjects[MAX_OBJECTS];
	for (int i = 0; i < MAX_OBJECTS; i++) {
		Point position(rand() % WIDTH, rand() % HEIGHT);
		Vector speed(15 - rand() % 30, 15 - rand() % 30);
		tabObjects[i] = new GameObject(5, position, speed, WIDTH, HEIGHT);
	}

	long time = clock();

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
		object.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE));
		for (int i = 0; i < MAX_OBJECTS; i++) {
			tabObjects[i]->draw(renderer, Color(0, 255, 255, SDL_ALPHA_OPAQUE));
		}

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	time = clock() - time;
	cout << time << " ms" << endl;

	quit_SDL();

	return 0;
}