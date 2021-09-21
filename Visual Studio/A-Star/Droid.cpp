#include "Droid.h"
#include "Grid.h"
#include <time.h>
#include "AStar.h"

int Droid::MOVE = 0;
int Droid::IS_BUSY = 1;
int Droid::WANDER = 2;

Droid::Droid(Grid* grid, Point position, int delay, double deltaAlphaLerp)
	:timerToMove(delay) {
	this->grid = grid;
	this->position = position;
	this->p1 = this->position;
	this->isRunningLerp = false;
	this->alphaLerp = 0;
	this->deltaAlphaLerp = deltaAlphaLerp;
}

bool Droid::isBusy() {
	return !this->path.isEmpty();
}

Point Droid::getPosition() {
	return this->isRunningLerp ? this->p2 : this->position;
}

void Droid::setPosition(Point position) {
	this->position = position;
}

void Droid::setPath(ListCells* path) {
	this->path = *path;
}

void Droid::lerp() {
	Vector v1(this->p1.x, this->p1.y);
	Vector v2(this->p2.x, this->p2.y);
	this->position = (1 - this->alphaLerp) * v1 + this->alphaLerp * v2;
}

void Droid::move() {
	if (this->timerToMove.isReady()) {
		if (this->isRunningLerp) {
			this->lerp();
			this->alphaLerp += this->deltaAlphaLerp;
			if (this->alphaLerp >= 1) {
				this->isRunningLerp = false;
				this->p1 = this->p2;
			}
		}
		else {
			if (!this->path.isEmpty()) {
				this->p2 = this->grid->cellToPoint(this->path.getNextCell());
				this->isRunningLerp = true;
				this->alphaLerp = this->deltaAlphaLerp;
			}
		}
	}
}

void Droid::draw(SDL_Renderer* renderer) {
	this->position.draw(renderer, { 0, 0, 255, SDL_ALPHA_OPAQUE }, this->grid->getSizeCell());
	this->drawTarget();
}

void Droid::drawTarget() {
	if (!this->path.isEmpty()) {
		Cell* target = this->path.getCellAt(0);
		grid->drawCell(target->getRow(), target->getCol(), { 255, 0, 0, SDL_ALPHA_OPAQUE });
	}
}

ValueBT Droid::action(int idAction) {
	ValueBT value = ValueBT::SUCCESS;

	if (idAction == Droid::MOVE) {			//	move action
		this->move();
		value = ValueBT::SUCCESS;
	}
	else if (idAction == Droid::IS_BUSY) {		//	is busy?
		value = this->path.isEmpty() ? ValueBT::FAIL : ValueBT::SUCCESS;
	}
	else if (idAction == Droid::WANDER) {		//	wander
		Cell* current = this->grid->getCell(&this->getPosition());
		Cell* target = this->grid->getRandomCellNonWall();
		this->setPath(&AStar::shortestPath(this->grid, current, target));
		value = ValueBT::SUCCESS;
	}
	else {
		throw new exception("Bad action id!");
	}

	return value;
}
