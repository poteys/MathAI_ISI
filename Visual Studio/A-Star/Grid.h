#pragma once

#include <SDL.h>
#include "../lib_Point/Point.h"
#include "Cell.h"
#include <vector>
#include <map>
using namespace std;

class Grid {
private:
	static const SDL_Color nonWallColor;
	static const SDL_Color wallColor;
	static const SDL_Color treasureColor;

	SDL_Renderer* renderer;
	SDL_Rect area;
	int nbRows, nbColumns;

	int widthCell, heightCell;

	map<int, Cell*> treasures;
	Cell** allCells;

public:
	Grid(SDL_Renderer* renderer, SDL_Rect area, int nbRows, int nbColumns);

	int getNbRows() const;
	int getNbColumns() const;

	//	managing cells
	void createAllCells();
	int getIdCell(int row, int col) const;
	int getIdCell(Cell* cell) const;
	Cell* getCell(int row, int column);
	bool isPointInGrid(Point* screenPoint) const;
	bool isInGrid(int row, int col) const;
	Cell* pointToCell(Point* screenPoint);
	Point cellToPoint(Cell *cell) const;
	Cell* getRandomEmptyCell();
	Cell* getRandomEmptyNonTreasureCell();
	vector<Cell* > getNeighbours(Cell* cell, bool fourNeighboursMode);
	int getSizeCell() const;

	//	managing walls
	void eraseWalls();
	void createWalls(double percent);
	bool isWall(int row, int col) const;

	//	managing treasures
	void createTreasures(int nbTreasures);
	void addTreasures(int nbTreasures);
	void removeTreasure(Cell* cell);
	bool isTreasure(Cell* cell) const;
	vector<Cell*> getTreasuresInRange(Cell* cell, int radius) const;
	int treasuresLeft() const;

	//	drawing
	void draw();
	void drawCells();
	void drawCell(Cell* cell, SDL_Color colorInside = { 150, 150, 150, SDL_ALPHA_OPAQUE }, SDL_Color borderColor = { 255, 255, 255, SDL_ALPHA_OPAQUE }) const;
	void drawTreasures() const;
};

