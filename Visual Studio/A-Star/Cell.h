#pragma once
class Cell {
private:
	int row, column;
public:
	Cell(int row, int column);

	int getRow() const;
	int getCol() const;

	double getDistance(Cell* cell) const;
};

