#include "Font.h"
#include <SDL_ttf.h>
#include <iostream>
using namespace std;

TTF_Font* Font::font = nullptr;
bool Font::initDone = false;
int Font::nbInstances = 0;

Font::Font(const char* fontPath, int size) {
	if (!Font::initDone) {
		TTF_Init();		//	init True Type Font librarie
		Font::initDone = true;
		Font::font = TTF_OpenFont(fontPath, size);

		if (Font::font == NULL) {
			throw new exception("unsupported font");
		}
	}


	Font::nbInstances++;
}

void Font::print(SDL_Renderer* renderer, int x, int y, string str, SDL_Color color, bool center) {
	if (this->str != str.c_str()) {
		this->str = str;

		//	free memory
		SDL_DestroyTexture(this->texture);
		SDL_FreeSurface(this->surface);

		this->surface = TTF_RenderUTF8_Blended(font, str.c_str(), color);
		this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);

		SDL_QueryTexture(texture, NULL, NULL, &this->texW, &this->texH);
	}
	if (!center) {

		this->dstrect = { x, y, texW, texH };
	}
	else {
		this->dstrect = { x - texW / 2, y - texH / 2, texW, texH };
	}

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}

Font::~Font() {
	//	free memory
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);

	Font::nbInstances--;
	if (Font::nbInstances == 0) {
		TTF_CloseFont(font);
		TTF_Quit();
	}
}

