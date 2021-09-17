#pragma once

#include "Boid.h"
#include <time.h>
#include <vector>
#include "../lib_Button/Button.h"

using namespace std;

class Boid;

class Flock : public IButtonListener {
private:
	bool showRegions = false;

public:
	vector<Boid*> boids;

	static double rangeRandom(double min, double max);

	int width, height;

	Flock(int width, int height);
	void setPopulation(int nbBoids);
	void draw(SDL_Renderer* renderer,
		double separationCoeff, double alignmentCoeff, double cohesionCoeff,
		int width, int height);
	void buttonPushed(void *source);
};
