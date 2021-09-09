#include "Perceptron.h"
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "../lib_Point/Point.h"

using namespace std;
 
double Perceptron::getRandomRange(double min, double max) {
	return min + (double)rand()/RAND_MAX * (max - min);
}

Perceptron::Perceptron(int nbInputs) {
	srand((unsigned int)time(NULL));
	this->nbInputs = nbInputs;
	this->W = new double[nbInputs];
	for (int i = 0; i < this->nbInputs; i++) {
		this->W[i] = getRandomRange(-5, 5);
	}
	this->teta = getRandomRange(-1, 1);

	/*this->W[0] = 0.5;
	this->W[1] = 0.5;
	this->teta = -2;*/
}

double Perceptron::computeSum(double X[]) {
	double sum = 0;

	for (int i = 0; i < this->nbInputs; i++) {
		sum += this->W[i] * X[i];
	}

	sum += this->teta;

	return sum;
}

double Perceptron::activationFunction(double x) {
	return x >= 0 ? +1 : -1;
}

double Perceptron::getOutput(double X[]) {
	return this->activationFunction(this->computeSum(X));
}

bool Perceptron::learn(double X[], double expected) {
	double output = getOutput(X);

	if (output != expected) {
		for (int i = 0; i < this->nbInputs; i++) {
			this->W[i] += (expected - output) * X[i];
		}
		this->teta += (expected - output);
	}

	return output == expected;
}

vector<Point> Perceptron::getTwoPointsOfLine(double xMin, double xMax, double yMin, double yMax) {
	vector<Point> result;
	if (this->W[1] != 0) {
		result.push_back(Point(xMin, -(this->W[0] * xMin + this->teta) / this->W[1]));
		result.push_back(Point(xMax, -(this->W[0] * xMax + this->teta) / this->W[1]));
	}
	else {
		result.push_back(Point(-(this->W[1] * yMin + this->teta) / this->W[0], yMin));
		result.push_back(Point(-(this->W[1] * yMax + this->teta) / this->W[0], yMax));
	}
	return result;
}