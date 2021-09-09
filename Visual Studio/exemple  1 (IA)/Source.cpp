#include <iostream>
#include <vector>
#include <SDL.h>
#include "Perceptron.h"
using namespace std;

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr int POS_X = -1000, POS_Y = 100;
constexpr int WIDTH = 600, HEIGHT = 600;

constexpr int nbCandidates = 100;
constexpr double xMin = -10;
constexpr double xMax = +10;
constexpr double yMin = -10;
constexpr double yMax = +10;

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

Point fromPlanToScreen(Point p,
	double xMin, double xMax, double yMin, double yMax,
	double xMinS, double xMaxS, double yMinS, double yMaxS) {

	double x = xMinS + (p.x - xMin) * (xMaxS - xMinS) / (xMax - xMin);
	//double y = yMinS + (yMax - p.y) * (yMaxS - yMinS) / (yMax - yMin);
	double y = yMaxS - (p.y - yMin) * (yMaxS - yMinS) / (yMax - yMin);

	Point result(x, y);

	return result;
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
	Perceptron perceptron(2);

	vector<Point> candidates;
	for (int i = 0; i < nbCandidates; i++) {
		candidates.push_back(Point(Perceptron::getRandomRange(-9.5, 9.5),
			Perceptron::getRandomRange(-9.5, 9.5)));
	}
	double a = Perceptron::getRandomRange(-1, 1);
	double b = Perceptron::getRandomRange(-1, 1);
	double c = Perceptron::getRandomRange(-1, 1);

	vector<int> categories;
	for (auto candidate : candidates) {
		if (a*candidate.x + b * candidate.y + c < 0) {
			categories.push_back(-1);
		}
		else {
			categories.push_back(1);
		}
	}

	//	*********  //
	//	main loop  //
	//	*********  //
	int nbFramesBeforeTraining = 500;
	bool endOfGame = false;
	bool isPerceptronTrained = true;
	while (!endOfGame) {
		//	******************************  //
		//	draw image in rendering buffer  //
		//	******************************  //
		clearWindow(renderer);

		/*	draw any desired graphical objects here	*/

		//	draw X and Y axes
		Point p = fromPlanToScreen(Point(xMin, 0), xMin, xMax, yMin, yMax, 0, WIDTH - 1, 0, HEIGHT - 1);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 0, (int)p.y, WIDTH, (int)p.y);

		p = fromPlanToScreen(Point(0, yMax), xMin, xMax, yMin, yMax, 0, WIDTH - 1, 0, HEIGHT - 1);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, (int)p.x, 0, (int)p.x, HEIGHT);

		//	draw Perceptron characteristic line
		vector<Point> linePoints = perceptron.getTwoPointsOfLine(xMin, xMax, yMin, yMax);
		Point pScreen1 = fromPlanToScreen(linePoints[0], xMin, xMax, yMin, yMax, 0, WIDTH - 1, 0, HEIGHT - 1);
		Point pScreen2 = fromPlanToScreen(linePoints[1], xMin, xMax, yMin, yMax, 0, WIDTH - 1, 0, HEIGHT - 1);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, (int)pScreen1.x, (int)pScreen1.y, (int)pScreen2.x, (int)pScreen2.y);

		//	draw all candidates
		Color colorCategory1(255, 255, 0, SDL_ALPHA_OPAQUE);
		Color colorCategory2(0, 255, 255, SDL_ALPHA_OPAQUE);
		for (int i = 0; i < candidates.size(); i++) {
			Point pScreen = fromPlanToScreen(candidates[i], xMin, xMax, yMin, yMax, 0, WIDTH - 1, 0, HEIGHT - 1);
			pScreen.draw(renderer, categories[i] == 1 ? colorCategory1 : colorCategory2, 3);
			//pScreen.drawCircle(renderer, 10, categories[i] == 1 ? colorCategory1 : colorCategory2, true);
		}

		//	show candidates to Perceptron
		nbFramesBeforeTraining--;
		if (nbFramesBeforeTraining == 0) {
			nbFramesBeforeTraining = 100;

			isPerceptronTrained = true;
			double X[2];
			for (int i = 0; i < candidates.size(); i++) {
				X[0] = candidates[i].x;
				X[1] = candidates[i].y;
				if (!perceptron.learn(X, categories[i])) {
					isPerceptronTrained = false;
				}
			}
		}

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();

	return 0;
}