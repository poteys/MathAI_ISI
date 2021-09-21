#include "ListNodes.h"

Node* ListNodes::findNode(Cell* cell) {
	for (Node* node : nodes) {
		if (node->getCell() == cell) {
			return node;
		}
	}
	return nullptr;
}

Node* ListNodes::findMinF() {
	Node* bestNode = nodes[0];
	for (Node* node : nodes) {
		if (node->getF() < bestNode->getF()) {
			bestNode = node;
		}
	}
	return bestNode;
}

void ListNodes::addNode(Node* node) {
	this->nodes.push_back(node);
}


void ListNodes::removeNode(Node* node) {
	for (int i = 0; i < nodes.size(); i++) {
		if (this->nodes[i] == node) {
			this->nodes.erase(this->nodes.cbegin() + i);
		}
	}
}

bool ListNodes::isEmpty() {
	return this->nodes.size() == 0;
}

vector<Node*> ListNodes::getNodes() {
	return this->nodes;
}

void ListNodes::draw(Grid* grid, SDL_Color color) {
	for (Node* node : this->getNodes()) {
		grid->drawCell(node->getCell()->getRow(), node->getCell()->getCol(), color);
	}
}

void ListNodes::free() {
	for (Node* n : this->nodes) {
		delete n;
	}
	this->nodes.clear();
}
