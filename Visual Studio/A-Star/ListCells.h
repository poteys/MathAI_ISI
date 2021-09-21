#pragma once

#include "Cell.h"
#include <vector>
using namespace std;

class ListCells {
private:
	vector<Cell*> cells;
public:
	void addCell(Cell* cell);
	int getLength();
	bool isEmpty();
	Cell* getCellAt(int index);
	Cell* getNextCell();
	void clear();
};

