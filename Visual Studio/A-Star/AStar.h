#pragma once
#include <vector>
#include "Cell.h"
#include "Grid.h"
#include "ListCells.h"
#include "Node.h"

using namespace std;

class AStar {
private:
	Grid* grid;
	map<Cell*, Node*> allNodes;		//	remplace int by Cell*
	map<Cell*, Node*> closedList;
	map<Cell*, Node*> openList;

	void freeLists();
	Node* findBestInOpenList();
	Path generateFinalPath(Node* n);
	void updateNode(Node *nodeToUpdate, Node* predecessor, Cell* end);
	Node* getNodeFromPool(Cell* start, Node* pred, Cell* end);

public:
	AStar(Grid *grid);
	Path shortestPath(Cell* start, Cell* end, bool fourNeighboursMode);
};

