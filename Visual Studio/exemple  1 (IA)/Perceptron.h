#pragma once

#include <vector>
#include "../lib_Point/Point.h"
using namespace std;

class Perceptron {
public:
	double* W;
	int nbInputs;
	double teta;

	static double getRandomRange(double min, double max);

public:
	Perceptron(int nbInputs);

	double computeSum(double X[]);
	double activationFunction(double x);
	double getOutput(double X[]);
	bool learn(double X[], double expected);

	vector<Point> getTwoPointsOfLine(double xMin, double xMax, double yMin, double yMax);
};