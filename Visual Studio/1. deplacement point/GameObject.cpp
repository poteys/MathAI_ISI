#include "GameObject.h"
#include <time.h>

__time64_t Boid::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

Boid::Boid(double mass, Point position, Vector speed, int width, int height) : position(position), speed(speed), acceleration(0, 0) {
	this->mass = mass;
	this->lastUpdate = this->getTimeInNanoSeconds();
	this->width = width;
	this->height = height;
}

void Boid::update() {
	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;

	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->position.x = this->position.x + this->speed.x * deltaT;
	this->position.y = this->position.y + this->speed.y * deltaT;
	//	OR this->position = this->position + this->speed * deltaT;		//	using library operators

	//	compute new speed of object now
	this->speed.x = this->speed.x + this->acceleration.x * deltaT;
	this->speed.y = this->speed.y + this->acceleration.y * deltaT;
	//	OR this->speed = this->speed + this->acceleration * deltaT;		//	using library operators

	//	compute sum of forces applied on object
	Vector gravity(0, 9.81 * this->mass);
	Vector wind(-50, 0);
	Vector friction = -0.2 * this->speed;
	Vector spring(0, -this->position.y / 5);

	Vector sum(0, 0);
	sum.x = gravity.x + spring.x + friction.x;
	sum.y = gravity.y + spring.y + friction.y;
	//	OR Vector sum = gravity + spring + friction;		//	using library operators

	// compute new acceleration of object now
	this->acceleration.x = sum.x / mass;
	this->acceleration.y = sum.y / mass;
	//	OR this->acceleration = sum / mass;		//	using library operators
}

Point Boid::getPosition() {
	return this->position;
}

Vector Boid::getSpeed() {
	return this->speed;
}

void Boid::setSpeed(Vector newSpeed) {
	this->speed = newSpeed;
}

void Boid::draw(SDL_Renderer* renderer, Color color) {
	this->update();

	//	check borders of screen
	if ((this->position.x > width&& this->speed.x > 0) ||
		(this->position.x < 0 && this->speed.x < 0)) {
		this->speed.x *= -0.9;
	}
	if ((this->position.y > height&& this->speed.y > 0) ||
		(this->position.y < 0 && this->speed.y < 0)) {
		this->speed.y *= -0.9;
	}

	this->position.draw(renderer, color, 3);
	//this->position.drawCircle(renderer, 10, color, true);
}
