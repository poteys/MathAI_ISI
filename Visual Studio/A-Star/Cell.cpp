#include "Cell.h"
#include <math.h>

#define min(a, b) ((a)<(b) ? (a) : (b))

Cell::Cell(int row, int column) {
	this->row = row;
	this->column = column;
}

int Cell::getRow() const {
	return this->row;
}

int Cell::getCol() const {
	return this->column;
}

double Cell::getDistance(Cell* cell) const {
	int dRow = this->row - cell->row;
	int dColumn = this->column - cell->column;

	//return min(dRow, dColumn) * 1.4 + abs(dRow - dColumn) * 1;

	return sqrt(dRow * dRow + dColumn * dColumn);
}
