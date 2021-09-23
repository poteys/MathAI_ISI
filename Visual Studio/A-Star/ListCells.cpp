#include "ListCells.h"

void ListCells::addCell(Cell* cell) {
	this->cells.push_back(cell);
}

int ListCells::getLength() const {
	return (int)this->cells.size();
}

bool ListCells::isEmpty() const {
	return this->cells.size() == 0;
}

Cell * ListCells::getCellAt(int index) const {
	return this->cells[index];
}

Cell* ListCells::getNextCell() {
	if (this->cells.size() == 0) {
		return nullptr;
	}
	Cell* cell = this->cells[this->cells.size() - 1];
	this->cells.pop_back();
	return cell;
}

void ListCells::clear() {
	this->cells.clear();
}