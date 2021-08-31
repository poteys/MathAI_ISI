#include <iostream>
using namespace std;

#include <SDL.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = -1000;	//	 1920 + 200;
constexpr auto POS_Y = 100;
constexpr auto WIDTH = 600, HEIGHT = 600;

//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"
#include "circle.h"

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
	Circle circle(Point(WIDTH / 2, HEIGHT / 2, true), 150);
	Point unPoint(WIDTH * 2 / 3, HEIGHT * 1 / 3, true);
	Slider sliderRadius(WIDTH / 2, HEIGHT - 100, 200, 0, 500, 150);	//	for the radius
	Slider sliderNbPoints(WIDTH / 2, HEIGHT - 50, 200, 0, 200, 100);	//	for the number of points

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
		circle.setRadius(sliderRadius.getValue());
		circle.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), sliderNbPoints.getValue());
		//circle.drawWithSqrt(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE));
		circle.drawWithSqrtSmart(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE));

		Color redColor(255, 0, 0, SDL_ALPHA_OPAQUE);
		Color greenColor(0, 255, 0, SDL_ALPHA_OPAQUE);
		unPoint.draw(renderer, (circle.contains(unPoint) ? greenColor : redColor), 5);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		circle.update(event);
		unPoint.update(event);
		sliderRadius.draw(renderer, event);
		sliderNbPoints.draw(renderer, event);

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();

	return 0;
}