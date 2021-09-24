#include "ListCells.h"

void Path::addCell(Cell* cell) {
	this->cells.push_back(cell);
}

int Path::getLength() const {
	return (int)this->cells.size();
}

bool Path::isEmpty() const {
	return this->cells.size() == 0;
}

Cell * Path::getCellAt(int index) const {
	return this->cells[index];
}

Cell* Path::getAndRemoveNextCell() {
	Cell* cell = nullptr;

	if (this->cells.size() != 0) {
		cell = this->cells[this->cells.size() - 1];
		this->cells.pop_back();
	}

	return cell;
}

void Path::clear() {
	this->cells.clear();
}