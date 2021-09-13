#pragma once

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

using namespace std;

class Font {
private:
	string str = "";
	SDL_Surface * surface = nullptr;
	SDL_Texture * texture = nullptr;
	int texW = 0, texH = 0;
	SDL_Rect dstrect = { 0, 0, 0, 0 };

public:
	static TTF_Font * font;
	static void init(const char *fontPath = "C:\\Windows\\Fonts\\arial.ttf", int size = 16);
	static void quit();

	void print(SDL_Renderer *renderer, int x, int y, string str, SDL_Color color = { 255, 255, 255, SDL_ALPHA_OPAQUE }, bool center= true);
	~Font();
};