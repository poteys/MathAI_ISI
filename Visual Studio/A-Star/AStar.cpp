#include "AStar.h"
#include "ListCells.h"
#include "Node.h"
#include <vector>
using namespace std;


void AStar::freeLists() {
	this->openList.clear();
	this->closedList.clear();
}

Node* AStar::findBestInOpenList() {
	Node* bestNode = nullptr;
	for (auto pair : this->openList) {
		if (bestNode == nullptr || pair.second->getF() < bestNode->getF()) {
			bestNode = pair.second;
		}
	}
	return bestNode;
}

Path AStar::generateFinalPath(Node* n) {
	Path path;

	//	create path from end to start, using predecessor information
	while (n->getPredecessor() != nullptr) {
		path.addCell(n->getCell());
		n = n->getPredecessor();
	}
	path.addCell(n->getCell());

	return path;
}

void AStar::updateNode(Node *nodeToUpdate, Node* otherPredecessor, Cell* end) {
	double newG = otherPredecessor->getG() + otherPredecessor->getCell()->getDistance(nodeToUpdate->getCell());
	if (newG < nodeToUpdate->getG()) {
		nodeToUpdate->setG(newG);
		nodeToUpdate->setPredecessor(otherPredecessor);
	}
}

Node* AStar::getNodeFromPool(Cell* cell, Node* pred, Cell* end) {
	Node* node = this->allNodes[cell];

	node->setPredecessor(pred);
	if (pred != nullptr) {
		node->setG(pred->getG() + pred->getCell()->getDistance(cell));
	}
	else {
		node->setG(0);
	}
	node->setH(cell->getDistance(end));

	return node;
}

AStar::AStar(Grid * grid) {
	this->grid = grid;

	//	create all nodes now
	for (int row = 0; row < grid->getNbRows(); row++) {
		for (int col = 0; col < grid->getNbColumns(); col++) {
			Cell* cell = grid->getCell(row, col);
			this->allNodes[cell] = new Node(cell);
		}
	}
}

Path AStar::shortestPath(Cell* start, Cell* end) {
	Path path;
	bool targetReached = false;

	Node* node = this->getNodeFromPool(start, nullptr, end);
	this->openList[start] = node;

	while (!this->openList.empty() && !targetReached) {
		Node* n = this->findBestInOpenList();

		this->openList.erase(n->getCell());

		this->closedList[n->getCell()] = n;

		if (n->getCell() == end) {
			path = this->generateFinalPath(n);
			targetReached = true;
		}
		else {
			vector<Cell*> neighbours = grid->getNeighbours(n->getCell());

			for (Cell* neighbour : neighbours) {
				if (this->closedList.count(neighbour) == 0) {
					//	neighbour is not in closed list
					if (this->openList.count(neighbour) == 1) {
						Node* nodeInOpenList = this->openList[neighbour];
						this->updateNode(nodeInOpenList, n, end);
					}
					else {
						//	neighbour is not in open list
						Node* neighbourNode = this->getNodeFromPool(neighbour, n, end);
						this->openList[neighbour] = neighbourNode;
					}
				}
			}
		}
	}

	this->freeLists();
	return path;
}
