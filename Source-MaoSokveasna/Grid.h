#pragma once
#include "Ship.h"
#include "Utility.h"

const int GRID_SIZE = 10; // TF: Constant

enum CellState {
    EMPTY,
    SHIP,
    HIT,
    MISS
};

struct Cell {
    CellState state; // TF: Variable Type
    Ship* ship;      // TF: Pointer Initialised
};

class Grid {
private:
    Cell cells[GRID_SIZE][GRID_SIZE]; // TF: Array
    vector<Ship*> ships;

    bool IsValidPlacement(Ship* ship, int row, int col, bool horizontal) const;
public:
    Grid(); // TF: Constructor
    Grid(const Grid& other); // TF: Copy Constructor
    ~Grid(); // TF: Destructor

    bool PlaceShip(Ship* ship, int row, int col, bool horizontal);
    bool FireAt(int row, int col, bool& hit, bool& sunk, string& shipName);
    void Display(bool showShips, int startX, int startY) const;
    bool IsAllShipsSunk() const;
    void MarkTrackingCell(int row, int col, bool hit); // TF: Function
    bool IsCellUntouched(int row, int col) const;
};  