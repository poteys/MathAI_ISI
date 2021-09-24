#pragma once

#include "../lib_Point/Point.h"
#include "AStar.h"
#include "ListCells.h"
#include "Grid.h"
#include "IActor.h"
#include "ValueBT.h"
#include "../lib_Timer/Timer.h"
#include "../lib_Button/Button.h"
#include "BT.h"

class Droid : public IActor, public IButtonListener {
private:
	AStar myAstar;
	Grid *grid;
	Point position;

	bool fourNeighboursMode = true;

	Path path;

	double radius = 5;

	bool blinking;
	double radiusMin, radiusMax, currentRadius;
	double deltaRadius, deltaRadiusMax = 0.3;

	//	Lerp management
	Point p1, p2;
	bool isRunningLerp;
	double alphaLerp, deltaAlphaLerp;

	Timer timerToMove;

	Cell* getCurrentCell();

	BT* behaviourTree;

public:
	static const int MOVE, IS_BUSY, WANDER, TARGET_TREASURE, IS_TREASURE_REACHED, BLINK, STOP_BLINK;

	Droid(Grid* grid, Point position, int delay = 20, double deltaAlphaLerp = 0.1);
	void setDeltaAlphaLerp(double deltaAlphaLerp);
	void setRadius(double radius);
	bool hasPathToGo();
	Point getPosition();
	void setPosition(Point position);
	void setPath(Path *path);
	void lerp();
	void move();
	void behave();
	void manageBlinking();
	void draw(SDL_Renderer *renderer);

	bool setTarget(Cell* target);
	void drawTarget();

	void buttonPushed(void *source);

	void getBehaviourTree();

	//	actions
	ValueBT action(int idAction);

	ValueBT moveAction();
	ValueBT isBusyAction();
	ValueBT wanderAction();
	ValueBT targetTreasureAction();
	ValueBT isTreasureReachedAction();
	ValueBT blinkAction();
	ValueBT stopBlinkAction();
};

