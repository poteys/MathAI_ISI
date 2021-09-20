#pragma once
class Cell {
private:
	int row, column;
public:
	Cell(int row, int column);

	int getRow();
	int getCol();

	double getDistance(Cell* cell);
};

