#include "AStar.h"
#include "ListNodes.h"
#include "ListCells.h"
#include "Node.h"
#include <vector>
using namespace std;

ListNodes AStar::closedList;
ListNodes AStar::openList;

void AStar::freeLists() {
	AStar::closedList.free();
	AStar::openList.free();
}

ListCells AStar::shortestPath(Grid* grid, Cell* start, Cell* end) {

	openList.addNode(new Node(start, nullptr, end));

	while (!openList.isEmpty()) {
		Node* n = openList.findMinF();

		openList.removeNode(n);
		closedList.addNode(n);

		if (n->getCell() == end) {
			//	create path from end to start, using predecessor information
			ListCells path;
			while (n->getPredecessor() != nullptr) {
				path.addCell(n->getCell());
				n = n->getPredecessor();
			}
			//path.addCell(n->getCell());

			//	draw open list
			//openList.draw(grid, { 0, 100, 0, SDL_ALPHA_OPAQUE });

			//	draw closed list
			//closedList.draw(grid, { 0, 200, 0, SDL_ALPHA_OPAQUE });

			//	draw path found
			//while (n->getPredecessor() != nullptr) {
			//	grid->drawCell(n->getCell()->getRow(), n->getCell()->getCol(),
			//		{ 0, 0, 255, SDL_ALPHA_OPAQUE });
			//	n = n->getPredecessor();
			//}
			//grid->drawCell(n->getCell()->getRow(), n->getCell()->getCol(),
			//	SDL_Color{ 0, 0, 255, SDL_ALPHA_OPAQUE });

			AStar::freeLists();
			return path;
		}

		vector<Cell*> neighbours = grid->getNeighbours(n->getCell());

		for (Cell* neighbour : neighbours) {
			if (closedList.findNode(neighbour) == nullptr) {

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

	AStar::freeLists();
	return ListCells();		//	no path from start to end
}
