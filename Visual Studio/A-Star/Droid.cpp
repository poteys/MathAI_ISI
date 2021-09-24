#include "Droid.h"
#include "Grid.h"
#include <time.h>
#include "AStar.h"

const int Droid::MOVE = 0;
const int Droid::IS_BUSY = 1;
const int Droid::WANDER = 2;
const int Droid::TARGET_TREASURE = 3;
const int Droid::IS_TREASURE_REACHED = 4;
const int Droid::BLINK = 5;
const int Droid::STOP_BLINK = 6;

Cell * Droid::getCurrentCell() {
	return this->grid->pointToCell(&this->getPosition());
}

Droid::Droid(Grid* grid, Point position, int delay, double deltaAlphaLerp)
	:timerToMove(delay), myAstar(grid) {
	this->grid = grid;
	this->position = position;

	this->p1 = this->position;
	this->isRunningLerp = false;
	this->alphaLerp = 0;
	this->deltaAlphaLerp = deltaAlphaLerp;

	this->blinking = false;
	this->radiusMin = 5.0;
	this->radiusMax = this->grid->getSizeCell();
	this->deltaRadius = -0.5;
	this->currentRadius = this->radiusMax;
}

bool Droid::hasPathToGo() {
	return !this->path.isEmpty();
}

Point Droid::getPosition() {
	return this->isRunningLerp ? this->p2 : this->position;
}

void Droid::setPosition(Point position) {
	this->position = position;
}

void Droid::setPath(Path* path) {
	this->path = *path;
}

void Droid::lerp() {
	Vector v1(this->p1.x, this->p1.y);
	Vector v2(this->p2.x, this->p2.y);

	this->position = (1 - this->alphaLerp) * v1 + this->alphaLerp * v2;

	this->alphaLerp += this->deltaAlphaLerp;

	if (this->alphaLerp >= 1) {
		this->isRunningLerp = false;
		this->p1 = this->p2;
	}
}

void Droid::move() {
	if (this->timerToMove.isReady()) {
		if (this->isRunningLerp) {
			this->lerp();
		}
		else if (!this->path.isEmpty()) {
			this->p2 = this->grid->cellToPoint(this->path.getAndRemoveNextCell());
			this->isRunningLerp = true;
			this->alphaLerp = this->deltaAlphaLerp;
		}
	}
}

void Droid::manageBlinking() {
	if (this->blinking) {
		this->currentRadius += this->deltaRadius;
		if (this->currentRadius < this->radiusMin) {
			this->currentRadius = this->radiusMin;
			this->deltaRadius = this->deltaRadiusMax;
		}
		else if (this->currentRadius > this->radiusMax) {
			this->currentRadius = this->radiusMax;
			this->deltaRadius = -this->deltaRadiusMax;
		}
	}
}

void Droid::draw(SDL_Renderer* renderer) {
	manageBlinking();

	this->position.draw(renderer, { 0, 0, 255, SDL_ALPHA_OPAQUE }, (int)this->currentRadius);
	this->drawTarget();

	this->position.drawCircle(renderer, (int)this->radius*this->grid->getSizeCell(), { 255, 255, 255, SDL_ALPHA_OPAQUE }, true);
}

bool Droid::setTarget(Cell * target) {
	bool value = false;

	Cell* current = this->getCurrentCell();
	Path path = myAstar.shortestPath(current, target);
	if (!path.isEmpty()) {
		path.getAndRemoveNextCell();
		this->setPath(&path);
		value = true;
	}
	return value;
}

void Droid::drawTarget() {
	if (!this->path.isEmpty()) {
		Cell* target = this->path.getCellAt(0);
		grid->drawCell(target, { 0, 200, 50, SDL_ALPHA_OPAQUE });
	}
}

ValueBT Droid::action(int idAction) {
	ValueBT value = ValueBT::SUCCESS;

	if (idAction == Droid::MOVE) { value = this->moveAction(); }
	else if (idAction == Droid::IS_BUSY) { value = this->isBusyAction(); }
	else if (idAction == Droid::WANDER) { value = this->wanderAction(); }
	else if (idAction == Droid::TARGET_TREASURE) { value = this->targetTreasureAction(); }
	else if (idAction == Droid::IS_TREASURE_REACHED) { value = this->isTreasureReachedAction(); }
	else if (idAction == Droid::BLINK) { value = this->blinkAction(); }
	else if (idAction == Droid::STOP_BLINK) { value = this->stopBlinkAction(); }
	else {
		throw new exception("Bad action id!");
	}

	return value;
}

//	all actions for Droid
ValueBT Droid::moveAction() {
	this->move();
	return ValueBT::SUCCESS;
}

ValueBT Droid::isBusyAction() {
	//return this->hasPathToGo() || this->isRunningLerp ? ValueBT::SUCCESS : ValueBT::FAIL;
	return this->hasPathToGo() ? ValueBT::SUCCESS : ValueBT::FAIL;
}

ValueBT Droid::wanderAction() {
	Cell* randomTarget = this->grid->getRandomEmptyNonTreasureCell();
	this->setTarget(randomTarget);
	return ValueBT::SUCCESS;
}

ValueBT Droid::targetTreasureAction() {
	ValueBT value = ValueBT::FAIL;

	Cell* treasure = this->grid->getNearestTreasure(this->getCurrentCell(), (int)this->radius);
	if (treasure != nullptr) {
		if (this->setTarget(treasure)) {
			value = ValueBT::SUCCESS;
		}
		else {
			this->grid->removeTreasure(treasure);	//	remove inaccessible treasure
			value = ValueBT::FAIL;
		}
	}

	return value;
}

ValueBT Droid::isTreasureReachedAction() {
	ValueBT value = ValueBT::FAIL;

	Cell* current = this->getCurrentCell();
	Cell* treasure = this->grid->getNearestTreasure(current, (int)this->radius);
	if (treasure != nullptr && treasure == current) {
		this->grid->removeTreasure(treasure);
		value = ValueBT::SUCCESS;
	}

	return value;
}

ValueBT Droid::blinkAction() {
	this->blinking = true;
	return ValueBT::SUCCESS;
}

ValueBT Droid::stopBlinkAction() {
	this->currentRadius = this->radiusMax;
	this->deltaRadius = -this->deltaRadiusMax;
	this->blinking = false;
	return ValueBT::SUCCESS;
}