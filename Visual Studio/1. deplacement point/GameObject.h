#pragma once

#include "../lib_Point/Point.h"

class GameObject {
private:
	double mass;

	Point position;
	Vector speed;		//	in pixels per second
	Vector acceleration;	//	in pixels per second per second

	__time64_t lastUpdate;	//	date in nanoseconds since last update

	int width, height;	//	limit of box

	__time64_t getTimeInNanoSeconds();
	void update();

public:
	//	constructor
	GameObject(double mass, Point position, Vector speed, int width, int height);

	//	getters and setters
	Point getPosition();
	Vector getSpeed();
	void setSpeed(Vector newSpeed);

	void draw(SDL_Renderer* renderer, Color color);
};

