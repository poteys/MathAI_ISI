#include "Flock.h"

double Flock::rangeRandom(double min, double max) {
	return min + (max - min)*(double)rand() / RAND_MAX;
}

Flock::Flock(int width, int height) {
	srand((unsigned int)time(NULL));

	this->width = width;
	this->height = height;

	double sizeArea = 50;
}

void Flock::setPopulation(int nbBoids) {
	while(this->boids.size() < nbBoids) {
		Point position(Flock::rangeRandom(0, this->width - 1), Flock::rangeRandom(0, this->height - 1));
		Vector speed(50 - Flock::rangeRandom(0, 100), 50 - Flock::rangeRandom(0, 100));
		this->boids.push_back(new Boid(position, speed));
	}
	while(this->boids.size() > nbBoids) {
		this->boids.pop_back();
	}
}

void Flock::draw(SDL_Renderer* renderer,
	double separationCoeff, double alignmentCoeff, double cohesionCoeff,
	int width, int height) {

	for (int i = 0; i < this->boids.size(); i++) {
		Vector separationAcceleration = this->boids[i]->separationBehaviour(*this);
		Vector alignmentAcceleration = this->boids[i]->alignmentBehaviour(*this);
		Vector cohesionAcceleration = this->boids[i]->cohesionBehaviour(*this);

		this->boids[i]->acceleration =
			separationCoeff * separationAcceleration +
			alignmentCoeff * alignmentAcceleration +
			cohesionCoeff * cohesionAcceleration;

		Boid::constrainVector(this->boids[i]->acceleration, this->boids[i]->maxAcceleration);
	}

	for (int i = 0; i < this->boids.size(); i++) {
		this->boids[i]->update(width, height);
		this->boids[i]->draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), width, height);
		/*this->boids[i]->position.drawCircle(renderer, cohesionRadius, Color(0, 255, 0, SDL_ALPHA_OPAQUE), true);
		this->boids[i]->position.drawCircle(renderer, alignmentRadius, Color(150, 150, 150, SDL_ALPHA_OPAQUE), true);
		this->boids[i]->position.drawCircle(renderer, separationRadius, Color(255, 0, 0, SDL_ALPHA_OPAQUE), true);*/
	}
}

void Flock::buttonPushed(void * source) {
	this->showRegions = !this->showRegions;
}
