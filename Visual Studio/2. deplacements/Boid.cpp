#include "Boid.h"
#include "Flock.h"

void Boid::constrainVector(Vector v, double max) {
	double mag = v.magnitude();
	if (mag > max) {
		v *= max / mag;
	}
}

double Boid::lerp(double min, double max, double alpha) {
	return (1 - alpha) * min + alpha * max;
}

bool Boid::inRange(Point target, double radius) {
	return this->position.sqrDist(target) <= radius * radius;
}

Boid::Boid(Point position, Vector speed) :speed(speed), acceleration(0, 0) {
	this->position = position;
}

void Boid::draw(SDL_Renderer* renderer, Color color, int width, int height) {
	this->position.draw(renderer, color, 5);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, this->position.x, this->position.y,
		this->position.x + 0.25 * this->speed.x, this->position.y + 0.25 * this->speed.y);
}

void Boid::update(int width, int height) {
	double elapsed = (double)(clock() - this->lastUpdate) / CLOCKS_PER_SEC;

	this->position += this->speed*elapsed;
	this->speed += this->acceleration*elapsed;

	Boid::constrainVector(this->speed, this->maxSpeed);

	this->lastUpdate = clock();

	if (this->position.x < 0) this->position.x += width;
	if (this->position.x > width) this->position.x -= width;
	if (this->position.y < 0) this->position.y += height;
	if (this->position.y > height) this->position.y -= height;
}

void Boid::lerp(Vector A, Vector B, double alpha) {
	this->position = (1 - alpha) * A + alpha * B;
}

void Boid::followLerp(Point target) {
	Vector positionVect(this->position.x, this->position.y);
	Vector targetVect(target.x, target.y);
	double alpha = 0.001;
	this->lerp(positionVect, targetVect, alpha);
}

void Boid::followRealistic(Point target) {
	if (this->inRange(target, 2)) {
		this->position = target;
		this->acceleration = Vector(0, 0);
		this->speed = Vector(0, 0);
	}
	else {
		Vector AB(this->position, target);
		AB.normalize();
		AB *= this->maxSpeed;
		//GameObject::constrainVector(AB, this->maxSpeed);

		this->acceleration = AB - this->speed;
		this->acceleration.normalize();
		this->acceleration *= this->maxAcceleration;
		//GameObject::constrainVector(this->acceleration, this->maxAcceleration);
	}
}


Vector Boid::separationBehaviour(Flock aFlock, double radius) {
	Vector targetAcceleration(0, 0);
	int nbNeighbours = 0;

	for (Boid* boid : aFlock.boids) {
		if (boid != this && boid->position.sqrDist(this->position) <= radius * radius) {
			targetAcceleration += Vector(this->position, boid->position);
			nbNeighbours++;
		}
	}

	if (nbNeighbours != 0) {
		targetAcceleration /= nbNeighbours;
		targetAcceleration = -targetAcceleration;

		targetAcceleration.normalize();
		targetAcceleration *= this->maxSpeed;

		targetAcceleration = targetAcceleration - this->speed;
		targetAcceleration.normalize();
		targetAcceleration *= this->maxAcceleration;
	}

	return targetAcceleration;
}

Vector Boid::alignmentBehaviour(Flock aFlock, double minRadius, double maxRadius) {
	Vector targetAcceleration(0, 0);
	int nbNeighbours = 0;

	for (Boid* boid : aFlock.boids) {
		if (boid != this) {
			double sqrDist = boid->position.sqrDist(this->position);
			if (sqrDist >= minRadius * minRadius && sqrDist < maxRadius * maxRadius) {
				targetAcceleration += boid->speed;
				nbNeighbours++;
			}
		}
	}
	if (nbNeighbours != 0) {
		targetAcceleration /= nbNeighbours;

		targetAcceleration.normalize();
		targetAcceleration *= this->maxSpeed;
		
		targetAcceleration = targetAcceleration - this->speed;
		targetAcceleration.normalize();
		targetAcceleration *= this->maxAcceleration;
	}

	return targetAcceleration;
}

Vector Boid::cohesionBehaviour(Flock aFlock, double minRadius, double maxRadius) {
	Vector targetAcceleration(0, 0);
	int nbNeighbours = 0;

	for (Boid* boid : aFlock.boids) {
		if (boid != this) {
			double sqrDist = boid->position.sqrDist(this->position);
			if (sqrDist >= minRadius * minRadius && sqrDist < maxRadius * maxRadius) {
				targetAcceleration += Vector(boid->position.x, boid->position.y);
				nbNeighbours++;
			}
		}
	}
	if (nbNeighbours != 0) {
		targetAcceleration /= nbNeighbours;
		targetAcceleration -= Vector(this->position.x, this->position.y);

		//if (targetAcceleration.magnitude() > this->maxSpeed) {
		targetAcceleration.normalize();
		targetAcceleration *= this->maxSpeed;
		//}

		targetAcceleration = targetAcceleration - this->speed;
		targetAcceleration.normalize();
		targetAcceleration *= this->maxAcceleration;
	}

	return targetAcceleration;
}

