#include "Cell.h"
#include <math.h>

Cell::Cell(int row, int column) {
	this->row = row;
	this->column = column;
}

int Cell::getRow() {
	return this->row;
}

int Cell::getCol() {
	return this->column;
}

double Cell::getDistance(Cell* cell) {
	int dRow = this->row - cell->row;
	int dColumn = this->column - cell->column;
	return sqrt(dRow * dRow + dColumn * dColumn);
}
