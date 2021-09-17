#pragma once

#include <SDL.h>

class Grid {
private:
	SDL_Renderer *renderer;
	SDL_Rect area;
	int nbRows, nbColumns;

	bool *walls;

public:
	Grid(SDL_Renderer* renderer, SDL_Rect area, int nbRows, int nbColumns, int nbWalls);

	void createWalls(int nbWalls);
	void draw();
	void drawCell(int row, int column,
		SDL_Color colorInside = { 0, 0, 0, SDL_ALPHA_OPAQUE }, SDL_Color borderColor = { 255, 255, 255, SDL_ALPHA_OPAQUE });
};

