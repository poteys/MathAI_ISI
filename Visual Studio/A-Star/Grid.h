#pragma once

#include <SDL.h>
#include "../lib_Point/Point.h"
#include "Cell.h"
#include <vector>
#include <map>
using namespace std;

class Grid {
private:
	SDL_Renderer* renderer;
	SDL_Rect area;
	int nbRows, nbColumns;

	bool* walls;
	map<int, Cell*> cells;

public:
	Grid(SDL_Renderer* renderer, SDL_Rect area, int nbRows, int nbColumns, int nbWalls);


	int getIdCell(int row, int col);
	void createWalls(int nbWalls);
	bool isWall(int row, int col);
	void draw();
	void drawCell(int row, int column, SDL_Color colorInside = { 0, 0, 0, SDL_ALPHA_OPAQUE }, SDL_Color borderColor = { 255, 255, 255, SDL_ALPHA_OPAQUE });
	Cell* getCell(int row, int column);
	Cell* getCell(Point* screenPoint);
	vector<Cell* > getNeighbours(Cell* cell);
};

