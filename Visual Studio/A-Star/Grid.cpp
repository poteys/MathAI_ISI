#include "Grid.h"
#include <stdlib.h>

#define max(a, b) ((a)>(b) ? (a) : (b))
#define min(a, b) ((a)<(b) ? (a) : (b))

const SDL_Color Grid::nonWallColor = { 200, 200, 200, SDL_ALPHA_OPAQUE };
const SDL_Color Grid::wallColor = { 200, 100, 50, SDL_ALPHA_OPAQUE };
const SDL_Color Grid::treasureColor = { 255, 255, 0, SDL_ALPHA_OPAQUE };

Grid::Grid(SDL_Renderer* renderer, SDL_Rect area, int nbRows, int nbColumns) {
	this->renderer = renderer;
	this->area = area;
	this->nbRows = nbRows;
	this->nbColumns = nbColumns;

	this->widthCell = (int)((double)this->area.w / this->nbColumns);
	this->heightCell = (int)((double)this->area.h / this->nbRows);
	this->area.w = this->nbColumns * this->widthCell;
	this->area.h = this->nbRows * this->heightCell;

	this->createAllCells();
}

int Grid::getNbRows() const {
	return this->nbRows;
}

int Grid::getNbColumns() const {
	return this->nbColumns;
}

//	managing cells
void Grid::createAllCells() {
	this->allCells = new Cell*[this->nbRows * this->nbColumns];
	int i = 0;
	for (int row = 0; row < this->nbRows; row++) {
		for (int col = 0; col < this->nbColumns; col++) {
			this->allCells[i++] = new Cell(row, col);
		}
	}
}

int Grid::getIdCell(int row, int col) const {
	return row * this->nbColumns + col;
}

int Grid::getIdCell(Cell* cell) const {
	return this->getIdCell(cell->getRow(), cell->getCol());
}

Cell* Grid::getCell(int row, int column) {
	int id = this->getIdCell(row, column);
	return this->allCells[id];
}

bool Grid::isPointInGrid(Point* screenPoint) const {
	return screenPoint->x >= this->area.x && screenPoint->x < (double)this->area.x + this->area.w - 1 ||
		screenPoint->y >= this->area.y && screenPoint->y < (double)this->area.y + this->area.h - 1;
}

bool Grid::isInGrid(int row, int col) const {
	return row >= 0 && row < this->nbRows && col >= 0 && col < this->nbColumns;
}

Cell* Grid::pointToCell(Point* screenPoint) {
	Cell* cell = nullptr;

	if (this->isPointInGrid(screenPoint)) {
		int column = (int)((screenPoint->x - this->area.x) / this->widthCell);
		int row = (int)((screenPoint->y - this->area.y) / this->heightCell);
		cell = getCell(row, column);
	}

	return cell;
}

Point Grid::cellToPoint(Cell *cell) const {
	return Point(this->area.x + this->widthCell * (cell->getCol() + 0.5),
		this->area.y + this->heightCell * (cell->getRow() + 0.5));
}

Cell* Grid::getRandomEmptyCell() {
	int row, column;
	do {
		row = rand() % this->nbRows;
		column = rand() % this->nbColumns;
	} while (this->allCells[this->getIdCell(row, column)]->getType() != CellType::EMPTY);

	return this->getCell(row, column);
}

Cell* Grid::getRandomEmptyNonTreasureCell() {
	Cell* cell;
	do {
		cell = this->getRandomEmptyCell();
	} while (treasures.count(this->getIdCell(cell)) == 1);

	return cell;
}

vector<Cell*> Grid::getNeighbours(Cell* cell, bool fourNeighboursMode) {
	vector<Cell*> neighbours;
	int dRow[] = { -1, 0, 0, 1, -1, -1, +1, +1 };
	int dCol[] = { 0, -1, +1, 0, -1, +1, -1, +1 };

	int nbNeighboursToCheck = 4;
	if (!fourNeighboursMode) {
		nbNeighboursToCheck = 8;
	}

	for (int i = 0; i < nbNeighboursToCheck; i++) {
		int row = cell->getRow() + dRow[i];
		int col = cell->getCol() + dCol[i];

		if (this->isInGrid(row, col) && this->allCells[this->getIdCell(row, col)]->getType() != CellType::WALL) {
			neighbours.push_back(this->getCell(row, col));
		}
	}

	return neighbours;
}

int Grid::getSizeCell() const {
	return min(this->widthCell, this->heightCell);
}

//	managing walls
void Grid::eraseWalls() {
	for (int i = 0; i < this->nbRows * this->nbColumns; i++) {
		this->allCells[i]->setType(CellType::EMPTY);
	}
}

void Grid::createWalls(double percent) {
	this->eraseWalls();

	int nbWalls = (int)(this->nbRows * this->nbColumns * percent / 100);
	for (int i = 0; i < nbWalls; i++) {
		this->allCells[this->getIdCell(this->getRandomEmptyNonTreasureCell())]->setType(CellType::WALL);
	}
}

bool Grid::isWall(int row, int col) const {
	return this->allCells[this->getIdCell(row, col)]->getType() == CellType::WALL;
}

//	managing treasures
void Grid::createTreasures(int nbTreasures) {
	this->treasures.clear();
	this->addTreasures(nbTreasures);
}

void Grid::addTreasures(int nbTreasures) {
	for (int i = 0; i < nbTreasures; i++) {
		Cell* cell = this->getRandomEmptyNonTreasureCell();
		treasures[this->getIdCell(cell)] = cell;
	}
}

void Grid::removeTreasure(Cell * cell) {
	this->treasures.erase(this->getIdCell(cell));
}

bool Grid::isTreasure(Cell* cell) const {
	return this->treasures.count(this->getIdCell(cell)) == 1;
}

vector<Cell*> Grid::getTreasuresInRange(Cell* cell, int radius) const {
	vector<Cell*> treasures;

	for (auto pair : this->treasures) {
		Cell* treasure = pair.second;
		if (cell->getDistance(treasure) < radius) {
			treasures.push_back(treasure);
		}
	}

	return treasures;
}

int Grid::treasuresLeft() const {
	return (int)this->treasures.size();
}

//	drawing
void Grid::draw() {
	this->drawCells();
	this->drawTreasures();
}

void Grid::drawCells() {
	int idCell = 0;
	for (int row = 0; row < this->nbRows; row++) {
		for (int column = 0; column < this->nbColumns; column++) {
			SDL_Color color = this->allCells[idCell]->getType() == CellType::WALL ? wallColor : nonWallColor;
			this->drawCell(this->allCells[idCell], color);
			idCell++;
		}
	}
}

void Grid::drawCell(Cell* cell, SDL_Color colorInside, SDL_Color borderColor) const {
	int x = this->area.x + cell->getCol() * this->widthCell;
	int y = this->area.y + cell->getRow() * this->heightCell;
	SDL_Rect rect = { x, y,this->widthCell + 1, this->heightCell + 1 };
	SDL_SetRenderDrawColor(this->renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
	SDL_RenderDrawRect(this->renderer, &rect);

	rect.x++;
	rect.y++;
	rect.w -= 2;
	rect.h -= 2;
	SDL_SetRenderDrawColor(this->renderer, colorInside.r, colorInside.g, colorInside.b, colorInside.a);
	SDL_RenderFillRect(this->renderer, &rect);
}

void Grid::drawTreasures() const {
	for (auto pair : treasures) {
		Cell* cell = pair.second;
		this->drawCell(cell, treasureColor);
	}
}
