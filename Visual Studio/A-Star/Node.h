#pragma once

#include "Cell.h"

class Node {
private:
	Cell* cell;
	double g, h;
	Node* predecessor;

public:
	//Node(Cell* cell, Node* pred, Cell* end);
	Node(Cell* cell);

	double getG();
	double getH();
	double getF();
	Node* getPredecessor();
	Cell* getCell();

	void setG(double g);
	void setH(double h);
	void setPredecessor(Node* predecessor);
};

