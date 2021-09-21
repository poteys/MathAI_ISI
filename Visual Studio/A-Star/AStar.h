#pragma once
#include <vector>
#include "Cell.h"
#include "Grid.h"
#include "ListNodes.h"
#include "ListCells.h"

using namespace std;

class AStar {
private:
	static ListNodes closedList;
	static ListNodes openList;

	static void freeLists();
public:
	static ListCells shortestPath(Grid* grid, Cell* start, Cell* end);
};

