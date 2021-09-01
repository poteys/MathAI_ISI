#pragma once

#include "SDL.h"
#include "../lib_Point/Point.h"
#include <time.h>

class GameObject {
private:
	Point position;
	Vector speed;
	Vector acceleration;

	double maxSpeed = 10;	//	 100;
	double maxAcceleration = 0.1;	//	 300;

	clock_t lastMove = clock();

	static void constrainVector(Vector v, double max);
	static double lerp(double min, double max, double alpha);

	bool inRange(Point target, double radius);

public:
	GameObject(Point position);
	void draw(SDL_Renderer* renderer, Color color);
	void update();

	void lerp(Vector A, Vector B, double alpha);
	void followLerp(Point target);
	void followRealistic(Point target);
};