#include "Grid.h"
#include <stdlib.h>

#define max(a, b) ((a)>(b) ? (a) : (b))
#define min(a, b) ((a)<(b) ? (a) : (b))

Grid::Grid(SDL_Renderer* renderer, SDL_Rect area, int nbRows, int nbColumns) {
	this->renderer = renderer;
	this->area = area;
	this->nbRows = nbRows;
	this->nbColumns = nbColumns;

	int widthCell = (int)((double)this->area.w / this->nbColumns);
	int heightCell = (int)((double)this->area.h / this->nbRows);
	this->area.w = this->nbColumns * widthCell;
	this->area.h = this->nbRows * heightCell;

	this->walls = new bool[(long long)this->nbRows * this->nbColumns];
}

int Grid::getIdCell(int row, int col) {
	return row * this->nbColumns + col;
}

int Grid::getIdCell(Cell* cell) {
	return this->getIdCell(cell->getRow(), cell->getCol());
}

void Grid::createWalls(double percent) {
	for (int i = 0; i < this->nbRows * this->nbColumns; i++) {
		this->walls[i] = false;
	}

	int nbWalls = this->nbRows * this->nbColumns * percent / 100;
	for (int i = 0; i < nbWalls; i++) {
		walls[this->getIdCell(this->getRandomCellNonWall())] = true;
	}
}

void Grid::createTreasures(int nbTreasures) {
	this->treasures.clear();

	for (int i = 0; i < nbTreasures; i++) {
		Cell* cell = this->getRandomEmptyCell();
		treasures[this->getIdCell(cell)] = cell;
	}
}

void Grid::addTreasures(int nbTreasures) {
	for (int i = 0; i < nbTreasures; i++) {
		Cell* cell = this->getRandomEmptyCell();
		treasures[this->getIdCell(cell)] = cell;
	}
}

bool Grid::isWall(int row, int col) {
	return this->walls[this->getIdCell(row, col)];
}

void Grid::draw() {
	SDL_Color nonWallColor = { 200, 200, 200, SDL_ALPHA_OPAQUE };
	SDL_Color wallColor = { 200, 100, 50, SDL_ALPHA_OPAQUE };
	SDL_Color treasureColor = { 255, 255, 0, SDL_ALPHA_OPAQUE };

	for (int row = 0; row < this->nbRows; row++) {
		for (int column = 0; column < this->nbColumns; column++) {
			int idCell = this->getIdCell(row, column);
			SDL_Color color = walls[idCell] ? wallColor : nonWallColor;
			this->drawCell(row, column, color);
		}
	}

	for (auto pair : treasures) {
		Cell* cell = pair.second;
		this->drawCell(cell->getRow(), cell->getCol(), treasureColor);
	}
}

void Grid::drawCell(int row, int column, SDL_Color colorInside, SDL_Color borderColor) {
	int widthCell = (int)((double)this->area.w / this->nbColumns);
	int heightCell = (int)((double)this->area.h / this->nbRows);
	int x = this->area.x + column * widthCell;
	int y = this->area.y + row * heightCell;
	SDL_Rect rect = { x, y, widthCell + 1, heightCell + 1 };
	SDL_SetRenderDrawColor(this->renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
	SDL_RenderDrawRect(this->renderer, &rect);

	rect.x++;
	rect.y++;
	rect.w -= 2;
	rect.h -= 2;
	SDL_SetRenderDrawColor(this->renderer, colorInside.r, colorInside.g, colorInside.b, colorInside.a);
	SDL_RenderFillRect(this->renderer, &rect);
}

Cell* Grid::getCell(int row, int column) {
	int id = this->getIdCell(row, column);

	if (this->cells[id] == nullptr) {
		this->cells[id] = new Cell(row, column);
	}

	return this->cells[id];
}

Cell* Grid::PointToCell(Point* screenPoint) {
	if (screenPoint->x < this->area.x || screenPoint->x >= (double)this->area.x + this->area.w - 1 ||
		screenPoint->y < this->area.y || screenPoint->y >= (double)this->area.y + this->area.h - 1) {
		return nullptr;
	}

	int column = (int)((screenPoint->x - this->area.x) / (this->area.w / this->nbColumns));
	int row = (int)((screenPoint->y - this->area.y) / (this->area.h / this->nbRows));

	return getCell(row, column);
}

Cell* Grid::getRandomCellNonWall() {
	int row, column;
	do {
		row = rand() % this->nbRows;
		column = rand() % this->nbColumns;
	} while (walls[this->getIdCell(row, column)]);

	return this->getCell(row, column);
}

Cell* Grid::getRandomEmptyCell() {
	Cell* cell;
	do {
		cell = this->getRandomCellNonWall();
	} while (treasures[this->getIdCell(cell)]);

	return cell;
}

Cell * Grid::getNearestTreasure(Cell* cell, int radius) {
	Cell* nearestTreasure = nullptr;

	for (auto pair : this->treasures) {
		Cell* treasure = pair.second;
		if (nearestTreasure == nullptr ||
			cell->getDistance(treasure) < cell->getDistance(nearestTreasure)) {
			nearestTreasure = treasure;
		}
	}
	return (nearestTreasure != nullptr && cell->getDistance(nearestTreasure) <= radius) ? nearestTreasure : nullptr;
}

int Grid::treasuresLeft() {
	return this->treasures.size();
}

void Grid::removeTreasure(Cell * cell) {
	this->treasures.erase(this->getIdCell(cell));
}

vector<Cell*> Grid::getNeighbours(Cell* cell) {
	vector<Cell*> neighbours;

	for (int row = max(0, cell->getRow() - 1); row <= min(this->nbRows - 1, cell->getRow() + 1); row++) {
		for (int col = max(0, cell->getCol() - 1); col <= min(this->nbColumns - 1, cell->getCol() + 1); col++) {
			if ((row - cell->getRow()) * (col - cell->getCol()) == 0) {
				if ((row != cell->getRow() || col != cell->getCol()) && !walls[this->getIdCell(row, col)]) {
					neighbours.push_back(this->getCell(row, col));
				}
			}
		}
	}

	return neighbours;
}

Point Grid::cellToPoint(Cell *cell) {
	int widthCell = (int)((double)this->area.w / this->nbColumns);
	int heightCell = (int)((double)this->area.h / this->nbRows);

	return Point(this->area.x + widthCell * cell->getCol() + widthCell / 2, this->area.y + heightCell * cell->getRow() + heightCell / 2);
}

int Grid::getSizeCell() {
	int sizeX = this->area.w / this->nbColumns;
	int sizeY = this->area.h / this->nbRows;
	return min(sizeX, sizeY);
}
