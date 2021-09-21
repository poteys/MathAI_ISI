#include "AStar.h"
#include "ListNodes.h"
#include "ListCells.h"
#include "Node.h"
#include <vector>
using namespace std;

//ListNodes AStar::closedList;
map<int, Node*> AStar::closedList;
ListNodes AStar::openList;

void AStar::freeLists() {
	//AStar::closedList.free();
	for (auto pair : AStar::closedList) {
		delete pair.second;
	}
	AStar::closedList.clear();

	AStar::openList.free();
}

ListCells AStar::shortestPath(Grid* grid, Cell* start, Cell* end) {
	ListCells path;
	bool targetReached = false;

	openList.addNode(new Node(start, nullptr, end));

	while (!openList.isEmpty() && !targetReached) {
		Node* n = openList.findMinF();

		openList.removeNode(n);
		//AStar::closedList.addNode(n);
		AStar::closedList[grid->getIdCell(n->getCell())] = n;


		if (n->getCell() == end) {
			//	create path from end to start, using predecessor information
			while (n->getPredecessor() != nullptr) {
				path.addCell(n->getCell());
				n = n->getPredecessor();
			}
			path.addCell(n->getCell());

			targetReached = true;
		}
		else {
			vector<Cell*> neighbours = grid->getNeighbours(n->getCell());

			for (Cell* neighbour : neighbours) {
				//if (closedList.findNode(neighbour) == nullptr) {
				if (closedList[grid->getIdCell(neighbour)] == nullptr) {
					//	neighbour is not in closed list
					Node* nodeInOpenList = openList.findNode(neighbour);
					if (nodeInOpenList != nullptr) {
						double newG = n->getG() + n->getCell()->getDistance(neighbour);
						//	neighbour is already in open list
						if (nodeInOpenList->getG() > newG) {
							nodeInOpenList->setG(newG);
							nodeInOpenList->setPredecessor(n);
						}
					}
					else {
						//	neighbour is not in open list
						Node* neighbourNode = new Node(neighbour, n, end);
						openList.addNode(neighbourNode);
					}
				}
			}
		}
	}

	AStar::freeLists();
	return path;		//	no path from start to end
}
