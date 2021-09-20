#include "Node.h"
#include "Cell.h"

Node::Node(Cell* cell, Node* pred, Cell* end) {
	this->cell = cell;
	this->predecessor = pred;
	if (this->predecessor != nullptr) {
		this->g = pred->g + pred->cell->getDistance(cell);
	}
	else {
		this->g = 0;
	}
	this->h = cell->getDistance(end);
}

double Node::getG() {
	return this->g;
}

double Node::getH() {
	return this->h;
}

double Node::getF() {
	return this->g + this->h;
}

Node* Node::getPredecessor() {
	return this->predecessor;
}


Cell* Node::getCell() {
	return this->cell;
}

void Node::setG(double g) {
	this->g = g;
}

void Node::setH(double h) {
	this->h = h;
}

void Node::setPredecessor(Node* predecessor) {
	this->predecessor = predecessor;
}
