#pragma once
#include <vector>
#include "Cell.h"
#include "Grid.h"
#include "List.h"

using namespace std;

class AStar {
private:
	static List closedList;
	static List openList;

	static void freeLists();
public:
	static vector<Cell*> shortestPath(Grid* grid, Cell* start, Cell* end);
};

