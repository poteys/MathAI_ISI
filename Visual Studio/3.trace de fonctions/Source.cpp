#include <iostream>
using namespace std;

#include <SDL.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 1920 + 200, POS_Y = 100;		//	ATTENTION au 1920 !!!!
constexpr auto WIDTH = 600, HEIGHT = 600;

//	specifications au the drawing window
constexpr auto xMinS = 50, xMaxS = 550;
constexpr auto yMinS = 50, yMaxS = 450;


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


Point fromPlanToScreen(Point p,
	double xMin, double xMax, double yMin, double yMax,
	double xMinS, double xMaxS, double yMinS, double yMaxS) {

	double x = xMinS + (p.x - xMin) * (xMaxS - xMinS) / (xMax - xMin);
	double y = yMinS + (yMax - p.y) * (yMaxS - yMinS) / (yMax - yMin);

	Point result(x, y);

	return result;
}


double f(double x, double a) {
	return sin(a * x) + cos(x);
}

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Trace de fonctions");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	double xMin = -10, xMax = 5;
	double yMin = -2, yMax = 2;

	Slider slider(50, 550, 200, -10, 10, 0);

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
		//	show drawing window
		SDL_Rect rect = { xMinS, yMinS, xMaxS - xMinS, yMaxS - yMinS };
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(renderer, &rect);

		//	draw the curve
		for (double x = xMin; x <= xMax; x += 0.1) {
			Point pPlan(x, f(x, slider.getValue()));

			if (pPlan.y >= yMin && pPlan.y <= yMax) {
				Point pScreen = fromPlanToScreen(pPlan,
					xMin, xMax, yMin, yMax,
					xMinS, xMaxS, yMinS, yMaxS);
				pScreen.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), 3);
			}
		}


		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		slider.draw(renderer, event);

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();

	return 0;
}