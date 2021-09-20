#include "AStar.h"
#include "List.h"
#include "Node.h"
#include <vector>
using namespace std;

List AStar::closedList;
List AStar::openList;

void AStar::freeLists() {
	AStar::closedList.free();
	AStar::openList.free();
}

vector<Cell*> AStar::shortestPath(Grid* grid, Cell* start, Cell* end) {

	openList.addNode(new Node(start, nullptr, end));

	while (!openList.isEmpty()) {
		Node* n = openList.findMinF();

		openList.removeNode(n);
		closedList.addNode(n);

		if (n->getCell() == end) {
			//	draw open list
			openList.draw(grid, { 0, 100, 0, SDL_ALPHA_OPAQUE });

			//	draw closed list
			closedList.draw(grid, { 0, 200, 0, SDL_ALPHA_OPAQUE });

			//	draw path found
			while (n->getPredecessor() != nullptr) {
				grid->drawCell(n->getCell()->getRow(), n->getCell()->getCol(),
					{ 0, 0, 255, SDL_ALPHA_OPAQUE });
				n = n->getPredecessor();
			}
			grid->drawCell(n->getCell()->getRow(), n->getCell()->getCol(),
				SDL_Color{ 0, 0, 255, SDL_ALPHA_OPAQUE });

			AStar::freeLists();
			return vector<Cell*>();
		}

		vector<Cell*> neighbours = grid->getNeighbours(n->getCell());

		for (Cell* neighbour : neighbours) {
			if (closedList.findNode(neighbour) == nullptr) {
				double newG = n->getG() + n->getCell()->getDistance(neighbour);

				//	neighbour is not in closed list
				Node* nodeInOpenList = openList.findNode(neighbour);
				if (nodeInOpenList != nullptr) {
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
	return vector<Cell*>();		//	no path from start to end
}
