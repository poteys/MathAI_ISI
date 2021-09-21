#pragma once

#include <vector>
#include "Node.h"
#include "Grid.h"

using namespace std;

class ListNodes {
private:
	vector<Node*> nodes;

public:
	Node* findNode(Cell* cell);
	Node* findMinF();
	void addNode(Node* node);
	void removeNode(Node* node);
	bool isEmpty();
	vector<Node*> getNodes();
	void draw(Grid* grid, SDL_Color color);

	void free();
};

