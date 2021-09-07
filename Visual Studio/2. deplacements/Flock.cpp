#include "Flock.h"

double Flock::rangeRandom(double min, double max) {
	return min + (max - min)*(double)rand() / RAND_MAX;
}

Flock::Flock(int nbBoids, int width, int height) {
	srand(time(NULL));

	double sizeArea = 50;
	for (int i = 0; i < nbBoids; i++) {
		Point position(Flock::rangeRandom(0, width - 1), Flock::rangeRandom(0, height - 1));
		Vector speed(50 - Flock::rangeRandom(0, 100), 50 - Flock::rangeRandom(0, 100));
		this->boids.push_back(new Boid(position, speed));
	}
}

void Flock::draw(SDL_Renderer* renderer,
	double separationCoeff, double alignmentCoeff, double cohesionCoeff,
	int width, int height) {
	double separationRadius = 50;
	double alignmentRadius = 80;
	double cohesionRadius = 100;

	for (int i = 0; i < this->boids.size(); i++) {
		Vector separationAcceleration = this->boids[i]->separationBehaviour(*this, separationRadius);
		Vector alignmentAcceleration = this->boids[i]->alignmentBehaviour(*this, separationRadius, alignmentRadius);
		Vector cohesionAcceleration = this->boids[i]->cohesionBehaviour(*this, alignmentRadius, cohesionRadius);

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
