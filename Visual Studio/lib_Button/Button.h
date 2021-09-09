#pragma once

#include <SDL.h>
#include <vector>
#include "../lib_Point/Point.h"
using namespace std;

class IButtonListener {
public:
	virtual void buttonPushed(void *source) = 0;
};

class Button {
private:
	string name;
	int x, y, width, height;

	Color insideColor;
	Color borderColor;

	bool isPressed;
	vector<IButtonListener *> listeners;

	bool isMouseInside(int mouseX, int mouseY);
	void notifyListeners();

public:
	Button(string name, int x, int y, int width, int height, Color insideColor = { 0, 255, 0 ,0 }, Color borderColor = { 255, 255, 255, 0 });
	string getName();
	void update(SDL_Event &evt);
	void draw(SDL_Renderer *renderer);
	void addListener(IButtonListener *listener);
	void removeListener(IButtonListener *listener);
};