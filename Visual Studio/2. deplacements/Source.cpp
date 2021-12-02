#include <iostream>
using namespace std;


#include <SDL.h>
#include <time.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 0, POS_Y = 50;
constexpr auto WIDTH = 800, HEIGHT = 800;

SDL_Renderer* renderer;

//	include desired header files for libraries

#include "../lib_Slider/Slider.h"
#include "../lib_Point/Point.h"
#include "Boid.h"
#include "Flock.h"
#include "../lib_Button/Button.h"

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

void testPartI() {
	/*	prepare useful objects here	*/
	Boid object(Point(WIDTH / 2, HEIGHT / 2), Vector(0, 0));

	double alpha = 0;	//	for Lerp

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
		int x, y;
		SDL_GetMouseState(&x, &y);
		Vector target(x, y);

		//	simple Lerp movement (starting point doesn't change)
		/*object.lerp(A, target, alpha);
		alpha += 0.0001;
		if (alpha > 1) {
			alpha = 0;
		}*/

		//	Lerp movement (starting point is always the position of the object)
		//object.followLerp(target);

		//	following the target (with speed and acceleration)
		object.followRealistic(target);
		object.update(WIDTH, HEIGHT);

		object.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), WIDTH, HEIGHT, true);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');

		SDL_Delay(15);
	}
}

void testPartII() {
	/*	prepare useful objects here	*/
	Flock theFlock(WIDTH, HEIGHT);

	Slider sliderSeparation(20, 20, 150, 0, 2, 0.5);
	Slider sliderAlignment(200, 20, 150, 0, 2, 0.5);
	Slider sliderCohesion(380, 20, 150, 0, 2, 0.5);
	Slider sliderPopulation(20, 40, 150, 0, 100, 0);
	Button btnShowRegions("show", "show regions", WIDTH - 200, 50, 100, 20);
	btnShowRegions.addListener(&theFlock);

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
		int x, y;
		SDL_GetMouseState(&x, &y);
		Point target(x, y);

		theFlock.setPopulation((int)sliderPopulation.getValue());
		theFlock.draw(renderer,
			sliderSeparation.getValue(), sliderAlignment.getValue(), sliderCohesion.getValue(),
			WIDTH, HEIGHT);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		sliderSeparation.draw(renderer, event);
		sliderAlignment.draw(renderer, event);
		sliderCohesion.draw(renderer, event);
		sliderPopulation.draw(renderer, event);
		btnShowRegions.draw(renderer);
		btnShowRegions.update(event);

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');

		SDL_Delay(15);
	}
}

//	entry point of application
int main(int argc, char** argv) {
	renderer = init_SDL("SLD template");	//	this object will draw in our window

	//testPartI();
	testPartII();

	quit_SDL();

	return 0;
}