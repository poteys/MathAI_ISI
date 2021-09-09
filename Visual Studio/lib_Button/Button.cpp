#include <iostream>
#include <SDL.h>

#include "Button.h"
#include "../lib_Point/Point.h"

bool Button::isMouseInside(int mouseX, int mouseY)
{
	return mouseX >= this->x && mouseX < this->x + this->width &&
		mouseY >= this->y && mouseY < this->y + this->height;
}

void Button::notifyListeners() {
	for (IButtonListener* l : listeners) {
		l->buttonPushed(this);
	}
}

Button::Button(string name, int x, int y, int width, int height, Color insideColor, Color borderColor) :
	insideColor(insideColor), borderColor(borderColor) {
	this->name = name;

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	this->isPressed = false;
}

string Button::getName() {
	return this->name;
}

void Button::update(SDL_Event & evt) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);	//	getting mouse information

	if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT && this->isMouseInside(mouseX, mouseY)) {
		this->isPressed = true;
		evt.type = 0;	//	set event to nothing

		//	notify listeners
		this->notifyListeners();
	}
	else if (evt.type == SDL_MOUSEBUTTONUP && this->isPressed) {
		this->isPressed = false;
		evt.type = 0;	//	set event to nothing
	}
}

void Button::draw(SDL_Renderer * renderer) {
	SDL_Rect rect{ this->x, this->y, this->width, this->height };

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	int alpha = (this->isPressed ? SDL_ALPHA_OPAQUE - 80 : SDL_ALPHA_OPAQUE);

	//	border
	SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, alpha);
	SDL_RenderDrawRect(renderer, &rect);

	rect.x--;
	rect.y--;

	//	inside
	SDL_SetRenderDrawColor(renderer, insideColor.r, insideColor.g, insideColor.b, alpha);
	SDL_RenderFillRect(renderer, &rect);
}

void Button::addListener(IButtonListener * listener) {
	this->listeners.push_back(listener);
}

void Button::removeListener(IButtonListener * listener) {
	int i = 0;
	while (i < this->listeners.size() && this->listeners[i] != listener) {
		i++;
	}

	if (i < this->listeners.size()) {
		this->listeners[i] = this->listeners[this->listeners.size() - 1];
		this->listeners.pop_back();
	}
}