#pragma once

#include <SDL.h>
#include <vector>
#include "../lib_Point/Point.h"
#include "../lib_Font/Font.h"
using namespace std;

class IButtonListener {
public:
	virtual void buttonPushed(void *source) = 0;
};

class Button {
private:
	string name, label;
	int x, y, width, height;

	Color textColor;
	Color insideColor;
	Color borderColor;

	Font font;

	bool isPressed;
	vector<IButtonListener *> listeners;

	bool isMouseInside(int mouseX, int mouseY);
	void notifyListeners();

public:
	Button(string name, string label, int x, int y, int width, int height,
		Color textColor = { 0, 0, 255, SDL_ALPHA_OPAQUE }, Color insideColor = { 0, 255, 0 ,0 }, Color borderColor = { 255, 255, 255, 0 },
		const char *fontPath = "C:\\Windows\\Fonts\\arial.ttf", int size = 12);
	string getName();
	void update(SDL_Event &evt);
	void draw(SDL_Renderer *renderer);
	void addListener(IButtonListener *listener);
	void removeListener(IButtonListener *listener);
};