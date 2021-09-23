#pragma once

#include "Cell.h"
#include <vector>
using namespace std;

class ListCells {
private:
	vector<Cell*> cells;
public:
	void addCell(Cell* cell);
	int getLength() const;
	bool isEmpty() const;
	Cell* getCellAt(int index) const;
	Cell* getAndRemoveNextCell();
	void clear();
};

