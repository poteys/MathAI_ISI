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
	map<int, Cell*> treasures;
	map<int, Cell*> cells;

public:
	Grid(SDL_Renderer* renderer, SDL_Rect area, int nbRows, int nbColumns);

	int getIdCell(int row, int col);
	int getIdCell(Cell* cell);
	void createWalls(double percent);
	void createTreasures(int nbTreasures);
	void addTreasures(int nbTreasures);
	bool isWall(int row, int col);
	void draw();
	void drawCell(int row, int column, SDL_Color colorInside = { 150, 150, 150, SDL_ALPHA_OPAQUE }, SDL_Color borderColor = { 255, 255, 255, SDL_ALPHA_OPAQUE });
	Cell* getCell(int row, int column);
	Cell* PointToCell(Point* screenPoint);
	Point cellToPoint(Cell *cell);
	Cell* getRandomCellNonWall();
	Cell* getRandomEmptyCell();
	Cell* getNearestTreasure(Cell* cell, int radius);
	int treasuresLeft();
	void removeTreasure(Cell* cell);
	vector<Cell* > getNeighbours(Cell* cell);
	int getSizeCell();
};

