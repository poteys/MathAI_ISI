#pragma once

#include "../lib_Point/Point.h"
#include "ListCells.h"
#include "Grid.h"
#include "IActor.h"
#include "ValueBT.h"
#include "../lib_Timer/Timer.h"

class Droid : public IActor {
private:
	Grid *grid;
	Point position;

	ListCells path;
	
	//	Lerp management
	Point p1, p2;
	bool isRunningLerp;
	double alphaLerp, deltaAlphaLerp;

	Timer timerToMove;

public:
	static int MOVE, IS_BUSY, WANDER;

	Droid(Grid* grid, Point position, int delay = 20, double deltaAlphaLerp = 0.1);
	bool isBusy();
	Point getPosition();
	void setPosition(Point position);
	void setPath(ListCells *path);
	void lerp();
	void move();
	void draw(SDL_Renderer *renderer);

	void drawTarget();

	//	actions
	ValueBT action(int idAction);
};

