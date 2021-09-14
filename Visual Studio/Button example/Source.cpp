#include <iostream>
using namespace std;

#include <SDL.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 200, POS_Y = 100;
constexpr auto WIDTH = 400, HEIGHT = 400;

//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "../lib_Button/Button.h"
#include "../lib_Font/Font.h"

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


class MyPoint : public Point, public IButtonListener {
private:
	bool showCircle = false;
	bool showPoint = true;

public:
	MyPoint(int x, int y, bool moveable) : Point(x, y, moveable) {}

	void buttonPushed(void *source) {	//	event handler
		Button* btn = (Button *)source;
		cout << "Button \"" << btn->getName().c_str() << "\"" << endl;

		if (btn->getName() == "point") {
			this->showPoint = !this->showPoint;
		}
		else if (btn->getName() == "circle") {
			this->showCircle = !this->showCircle;
		}
	}

	void draw(SDL_Renderer *renderer, int radius, Color c) {
		if (this->showPoint) {
			this->Point::draw(renderer, c, 5);
		}

		if (this->showCircle) {
			this->Point::drawCircle(renderer, 100, { 255, 0, 0, SDL_ALPHA_OPAQUE }, true);
		}
	}
};

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("SLD template");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	Button btnShowPoint("point", "show point", 10, 10, 70, 20);
	Button btnShowCircle("circle", "show circle", 90, 10, 70, 20);

	MyPoint p(WIDTH / 2, HEIGHT / 2, true);
	btnShowPoint.addListener(&p);	//	register this point to first button
	btnShowCircle.addListener(&p);	//	register this point to second button

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
		p.draw(renderer, 100, { 255, 255, 255, SDL_ALPHA_OPAQUE });
		btnShowPoint.draw(renderer);
		btnShowCircle.draw(renderer);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		p.update(event);
		btnShowPoint.update(event);
		btnShowCircle.update(event);

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();

	return 0;
}

