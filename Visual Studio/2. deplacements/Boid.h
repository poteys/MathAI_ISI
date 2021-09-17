#pragma once

#include "SDL.h"
#include "../lib_Point/Point.h"
#include <time.h>
#include "Flock.h"

class Flock;

class Boid {
public:
	Point position;
	Vector speed;
	Vector acceleration;

	double maxSpeed = 200;
	double maxAcceleration = 200;

	double separationRadius = 50;
	double alignmentRadius = 80;
	double cohesionRadius = 100;

	clock_t lastUpdate = clock();

	static void constrainVector(Vector v, double max);
	static double lerp(double min, double max, double alpha);

	bool inRange(Point target, double radius);

public:
	Boid(Point position, Vector speed);
	void draw(SDL_Renderer* renderer, Color color, int width, int height, bool showRegions);
	void update(int width, int height);

	void lerp(Vector A, Vector B, double alpha);
	void followLerp(Point target);
	void followRealistic(Point target);

	Vector separationBehaviour(Flock aFlock);
	Vector alignmentBehaviour(Flock aFlock);
	Vector cohesionBehaviour(Flock aFlock);
};