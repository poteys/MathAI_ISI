#include "circle.h"

Circle::Circle(Point center, double radius) {
	this->center = center;
	this->radius = radius;
}

void Circle::setRadius(double newRadius) {
	this->radius = newRadius;
}

void Circle::update(SDL_Event& event) {
	this->center.update(event);
}

void Circle::draw(SDL_Renderer* renderer, Color color, int nbPoints) {
	this->center.draw(renderer, color, 5);
	double deltaAlpha = 360.0 / nbPoints;
	Point p;
	for (double alpha = 0; alpha < 360; alpha += deltaAlpha) {
		p.x = this->center.x + this->radius * cos(alpha * M_PI / 180);
		p.y = this->center.y - this->radius * sin(alpha * M_PI / 180);
		p.draw(renderer, color, 3);
	}
}

void Circle::drawWithSqrt(SDL_Renderer* renderer, Color color) {
	this->center.draw(renderer, color, 5);

	for (int x = (int)(this->center.x - this->radius); x <= this->center.x + this->radius; x++) {
		double y = this->center.y + sqrt(this->radius * this->radius - (x - this->center.x) * (x - this->center.x));
		Point pBottom(x, y);
		pBottom.draw(renderer, color, 1);
		y = this->center.y - sqrt(this->radius * this->radius - (x - this->center.x) * (x - this->center.x));
		Point pTop(x, y);
		pTop.draw(renderer, color, 1);
	}
}

void Circle::drawWithSqrtSmart(SDL_Renderer* renderer, Color color) {
	this->center.draw(renderer, color, 5);
	Point p;

	int x = 0, y;
	do {
		y = (int)sqrt(this->radius * this->radius - (double)x * x);
		y = (int)(y + 0.5);

		//	affichage dans le cadran nord-est
		p.x = this->center.x + x;
		p.y = this->center.y - y;
		p.draw(renderer, color, 1);

		p.x = this->center.x - x;
		p.y = this->center.y - y;
		p.draw(renderer, color, 1);

		p.x = this->center.x - y;
		p.y = this->center.y - x;
		p.draw(renderer, color, 1);

		p.x = this->center.x - y;
		p.y = this->center.y + x;
		p.draw(renderer, color, 1);

		p.x = this->center.x - x;
		p.y = this->center.y + y;
		p.draw(renderer, color, 1);

		p.x = this->center.x + x;
		p.y = this->center.y + y;
		p.draw(renderer, color, 1);

		p.x = this->center.x + y;
		p.y = this->center.y + x;
		p.draw(renderer, color, 1);

		p.x = this->center.x + y;
		p.y = this->center.y - x;
		p.draw(renderer, color, 1);

		x++;
	} while ((x - 1) < y);
}

bool Circle::contains(Point p)
{
	double d = sqrt((p.x - this->center.x) * (p.x - this->center.x) + (p.y - this->center.y) * (p.y - this->center.y));
	return d <= this->radius;
}
