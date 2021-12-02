#pragma once

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

using namespace std;

class Font {
private:
	static TTF_Font* font;
	static bool initDone;
	static int nbInstances;

	string str = "";
	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;
	int texW = 0, texH = 0;
	SDL_Rect dstrect = { 0, 0, 0, 0 };

public:
	Font(const char* fontPath = "C:\\Windows\\Fonts\\arial.ttf", int size = 12);
	void print(SDL_Renderer* renderer, int x, int y, string str, SDL_Color color = { 255, 255, 255, SDL_ALPHA_OPAQUE }, bool center = true);
	~Font();
};