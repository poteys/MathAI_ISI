#include "Grid.h"
#include <stdlib.h>

Grid::Grid(SDL_Renderer * renderer, SDL_Rect area, int nbRows, int nbColumns, int nbWalls) {
	this->renderer = renderer;
	this->area = area;
	this->nbRows = nbRows;
	this->nbColumns = nbColumns;

	this->walls = new bool[nbRows*nbColumns];
	for (int i = 0; i < nbRows*nbColumns; i++) {
		this->walls[i] = false;
	}
}

void Grid::createWalls(int nbWalls) {
	for (int i = 0; i < nbRows*nbColumns; i++) {
		this->walls[i] = false;
	}
	for (int i = 0; i < nbWalls; i++) {
		int row = rand() % nbRows;
		int column = rand() % nbColumns;
		walls[row*nbColumns + column] = true;
	}
}

void Grid::draw() {
	for (int row = 0; row < this->nbRows; row++) {
		for (int column = 0; column < this->nbColumns; column++) {
			if (walls[row*nbColumns + column]) {
				this->drawCell(row, column, { 200, 0, 0, SDL_ALPHA_OPAQUE });
			}
			else {
				this->drawCell(row, column);
			}
		}
	}
}

void Grid::drawCell(int row, int column, SDL_Color colorInside, SDL_Color borderColor) {
	int widthCell = this->area.w / this->nbColumns;
	int heightCell = this->area.h / this->nbRows;
	int x = this->area.x + column * widthCell;
	int y = this->area.y + row * heightCell;
	SDL_Rect rect = { x, y, widthCell + 1, heightCell + 1 };
	SDL_SetRenderDrawColor(this->renderer, borderColor.r, borderColor.g, borderColor.g, borderColor.a);
	SDL_RenderDrawRect(this->renderer, &rect);

	rect.x++;
	rect.y++;
	rect.w -= 2;
	rect.h -= 2;
	SDL_SetRenderDrawColor(this->renderer, colorInside.r, colorInside.g, colorInside.g, colorInside.a);
	SDL_RenderFillRect(this->renderer, &rect);
}
