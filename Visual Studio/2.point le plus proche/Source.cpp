#include <iostream>
using namespace std;

#include <SDL.h>
#include <time.h>

//	****************
//	window attributs
//	****************
//	- position on screen
constexpr auto POS_X = 1920 + 200;
constexpr auto POS_Y = 100;

//	- size on screen
constexpr auto WIDTH = 400;
constexpr auto HEIGHT = 400;

//	nombre de points
constexpr auto NB_POINTS = 10;

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


double computeSquaredDistance(Point a, Point b) {
	double deltaX = b.x - a.x;
	double deltaY = b.y - a.y;

	/*
	le vecteur deplacement (deltaX, deltaY) permet de passer du
	point a au point b
	a.x + deltaX vaut a.x + b.x - a.x = b.x
	a.y + deltaY vaut a.y + b.y - a.y = b.y
	*/

	return deltaX * deltaX + deltaY * deltaY;
}

//	nbPoints n'est pas nul
Point computeClosestPoint(Point mainPoint, Point points[], int nbPoints) {
	double min = computeSquaredDistance(mainPoint, points[0]);
	Point closestPointToDate = points[0];

	for (int i = 1; i < nbPoints; i++) {
		double d = computeSquaredDistance(mainPoint, points[i]);

		if (d < min) {
			min = d;
			closestPointToDate = points[i];
		}
	}

	return closestPointToDate;
}



//	entry point of application
int main(int argc, char** argv) {
	// initialization of random number generator
	srand((unsigned int)time(NULL));

	SDL_Renderer* renderer = init_SDL("SLD template");	//	this object will draw in our window

	//	***********************  //
	//	prepare usefull objects  //
	//	***********************  //
	Point mainPoint(WIDTH / 2, HEIGHT / 2, true);
	Color mainPointColor(255, 0, 0, SDL_ALPHA_OPAQUE);

	Point pointsArray[NB_POINTS];
	Color pointsColor(200, 200, 200, SDL_ALPHA_OPAQUE);

	for (int i = 0; i < NB_POINTS; i++) {
		Point p(rand() % WIDTH, rand() % HEIGHT, true);
		pointsArray[i] = p;

		//pointsArray[i] = Point(rand() % WIDTH, rand() % HEIGHT, true);
	}

	//	*********  //
	//	main loop  //
	//	*********  //
	bool endOfGame = false;

	clock_t start, end;

	while (!endOfGame) {
		start = clock();

		//	******************************  //
		//	draw image in rendering buffer  //
		//	******************************  //
		clearWindow(renderer);

		//	- draw any desired graphical objects
		mainPoint.draw(renderer, mainPointColor, 11);

		for (int i = 0; i < NB_POINTS; i++) {
			pointsArray[i].draw(renderer, pointsColor, 7);
		}

		Point closestPoint = computeClosestPoint(mainPoint, pointsArray, NB_POINTS);
		closestPoint.drawCircle(renderer, 20, Color(255, 100, 255, SDL_ALPHA_OPAQUE), true);
		SDL_RenderDrawLine(renderer, (int)mainPoint.x, (int)mainPoint.y, (int)closestPoint.x, (int)closestPoint.y);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		//	- give event to objects for update if needed
		mainPoint.update(event);

		for (int i = 0; i < NB_POINTS; i++) {
			pointsArray[i].update(event);
		}

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');

		end = clock();
		clock_t ellapsed = end - start;
		int fps = (int)((double)CLOCKS_PER_SEC / ellapsed);
		cout << fps << " FPS" << endl;
	}

	quit_SDL();

	return 0;
}