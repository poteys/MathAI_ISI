#pragma once

#include "../lib_Point/Point.h"

class Circle {
private:
	Point center;
	double radius;

public:
	Circle(Point center, double radius);
	void setRadius(double newRadius);
	void update(SDL_Event& event);
	void draw(SDL_Renderer* renderer, Color color, int nbPoints);
	void drawWithSqrt(SDL_Renderer* renderer, Color color);
	void drawWithSqrtSmart(SDL_Renderer* renderer, Color color);
	bool contains(Point p);
};