#include "GameObject.h"

void GameObject::constrainVector(Vector v, double max) {
	double mag = v.magnitude();
	if (mag > max) {
		v *= max / mag;
	}
}

double GameObject::lerp(double min, double max, double alpha) {
	return (1 - alpha) * min + alpha * max;
}

bool GameObject::inRange(Point target, double radius) {
	return this->position.sqrDist(target) <= radius * radius;
}

GameObject::GameObject(Point position) :speed(0, 0), acceleration(0, 0) {
	this->position = position;
}

void GameObject::draw(SDL_Renderer* renderer, Color color) {
	this->position.draw(renderer, color, 5);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, this->position.x, this->position.y,
		this->position.x + 1 * this->speed.x, this->position.y + 1 * this->speed.y);
}

void GameObject::update() {
	double elapsed = (double)(clock() - this->lastMove) / CLOCKS_PER_SEC;

	this->position += this->speed*elapsed;
	this->speed += this->acceleration*elapsed;

	GameObject::constrainVector(this->speed, this->maxSpeed);

	this->lastMove = clock();
}

void GameObject::lerp(Vector A, Vector B, double alpha) {
	this->position = (1 - alpha) * A + alpha * B;
}

void GameObject::followLerp(Point target) {
	Vector positionVect(this->position.x, this->position.y);
	Vector targetVect(target.x, target.y);
	double alpha = 0.001;
	this->lerp(positionVect, targetVect, alpha);
}

void GameObject::followRealistic(Point target) {
	if (this->inRange(target, 2)) {
		this->acceleration = Vector(0, 0);
		this->speed = Vector(0, 0);
	}
	else {
		Vector AB(this->position, target);
		/*AB.normalize();
		AB *= this->maxSpeed;*/
		GameObject::constrainVector(AB, this->maxSpeed);

		this->acceleration = AB - this->speed;
		//this->acceleration.normalize();
		//this->acceleration *= this->maxAcceleration;
		GameObject::constrainVector(this->acceleration, this->maxAcceleration);
	}
}