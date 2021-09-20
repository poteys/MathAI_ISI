#include "List.h"

Node* List::findNode(Cell* cell) {
	for (Node* node : nodes) {
		if (node->getCell() == cell) {
			return node;
		}
	}
	return nullptr;
}

Node* List::findMinF() {
	Node* bestNode = nodes[0];
	for (Node* node : nodes) {
		if (node->getF() < bestNode->getF()) {
			bestNode = node;
		}
	}
	return bestNode;
}

void List::addNode(Node* node) {
	this->nodes.push_back(node);
}


void List::removeNode(Node* node) {
	for (int i = 0; i < nodes.size(); i++) {
		if (this->nodes[i] == node) {
			this->nodes.erase(this->nodes.cbegin() + i);
		}
	}
}

bool List::isEmpty() {
	return this->nodes.size() == 0;
}

vector<Node*> List::getNodes() {
	return this->nodes;
}

void List::draw(Grid* grid, SDL_Color color) {
	for (Node* node : this->getNodes()) {
		grid->drawCell(node->getCell()->getRow(), node->getCell()->getCol(), color);
	}
}

void List::free() {
	for (Node* n : this->nodes) {
		delete n;
	}
	this->nodes.clear();
}
