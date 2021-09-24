#pragma once

enum CellType {
	EMPTY, WALL
};

class Cell {
private:
	int row, column;
	CellType type;

public:
	Cell(int row, int column);

	int getRow() const;
	int getCol() const;
	CellType getType() const;
	void setType(CellType type);

	double getDistance(Cell* cell) const;
};

