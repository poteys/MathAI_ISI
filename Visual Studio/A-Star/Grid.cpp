#include "Grid.h"
#include <stdlib.h>

#define max(a, b) ((a)>(b) ? (a) : (b))
#define min(a, b) ((a)<(b) ? (a) : (b))

Grid::Grid(SDL_Renderer* renderer, SDL_Rect area, int nbRows, int nbColumns, int nbWalls) {
	this->renderer = renderer;
	this->area = area;
	this->nbRows = nbRows;
	this->nbColumns = nbColumns;

	int widthCell = (int)((double)this->area.w / this->nbColumns);
	int heightCell = (int)((double)this->area.h / this->nbRows);
	this->area.w = this->nbColumns * widthCell;
	this->area.h = this->nbRows * heightCell;

	this->walls = new bool[(long long)this->nbRows * this->nbColumns];
	this->createWalls(nbWalls);
}

int Grid::getIdCell(int row, int col) {
	return row * this->nbColumns + col;
}

void Grid::createWalls(int nbWalls) {
	for (int i = 0; i < this->nbRows * this->nbColumns; i++) {
		this->walls[i] = false;
	}
	for (int i = 0; i < nbWalls; i++) {
		int row = rand() % this->nbRows;
		int column = rand() % this->nbColumns;
		walls[this->getIdCell(row, column)] = true;
	}
}

bool Grid::isWall(int row, int col) {
	return this->walls[this->getIdCell(row, col)];
}

void Grid::draw() {
	for (int row = 0; row < this->nbRows; row++) {
		for (int column = 0; column < this->nbColumns; column++) {
			if (walls[row * this->nbColumns + column]) {
				this->drawCell(row, column, { 255, 0, 0, SDL_ALPHA_OPAQUE });
			}
			else {
				this->drawCell(row, column, { 50, 50, 50, SDL_ALPHA_OPAQUE });
			}
		}
	}
}

void Grid::drawCell(int row, int column, SDL_Color colorInside, SDL_Color borderColor) {
	int widthCell = (int)((double)this->area.w / this->nbColumns + 0.5);
	int heightCell = (int)((double)this->area.h / this->nbRows + 0.5);
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
	Cell* cell = nullptr;

	int id = row * this->nbColumns + column;
	cell = cells[id];
	/*auto iter = cells.find(id);*/
	if (cell == nullptr) {
		cell = new Cell(row, column);
		cells[id] = cell;
	}

	return cell;
}

Cell* Grid::getCell(Point* screenPoint) {
	if (screenPoint->x < this->area.x || screenPoint->x >= (double)this->area.x + this->area.w - 1 ||
		screenPoint->y < this->area.y || screenPoint->y >= (double)this->area.y + this->area.h - 1) {
		return nullptr;
	}
	int column = (int)((screenPoint->x - this->area.x) / (this->area.w / this->nbColumns));
	int row = (int)((screenPoint->y - this->area.y) / (this->area.h / this->nbRows));

	return getCell(row, column);
}

vector<Cell*> Grid::getNeighbours(Cell* cell) {
	vector<Cell*> neighbours;

	for (int row = max(0, cell->getRow() - 1); row <= min(this->nbRows - 1, cell->getRow() + 1); row++) {
		for (int col = max(0, cell->getCol() - 1); col <= min(this->nbColumns - 1, cell->getCol() + 1); col++) {
			if ((row != cell->getRow() || col != cell->getCol()) && !walls[this->getIdCell(row, col)]) {
				neighbours.push_back(this->getCell(row, col));
			}
		}
	}

	return neighbours;
}
