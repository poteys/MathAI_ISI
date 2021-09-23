#include "Node.h"
#include "Cell.h"

Node::Node(Cell* cell) {
	this->cell = cell;
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
