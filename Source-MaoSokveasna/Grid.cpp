#include "Grid.h"

Grid::Grid() { // TF: Constructor
    for (int i = 0; i < GRID_SIZE; ++i) { // TF: Iteration Structure
        for (int j = 0; j < GRID_SIZE; ++j) {
            cells[i][j].state = EMPTY;
            cells[i][j].ship = nullptr;
        }
    }
}
Grid::Grid(const Grid& other) { // TF: Copy Constructor
    for (Ship* ship : other.ships) {
        ships.push_back(new Ship(*ship)); // TF: Dynamic Memory
    }
    for (int i = 0; i < GRID_SIZE; ++i) { // TF: Iteration Structure
        for (int j = 0; j < GRID_SIZE; ++j) {
            cells[i][j].state = other.cells[i][j].state;
            cells[i][j].ship = other.cells[i][j].ship;

            //Relink ship pointers to the copied ships
            Ship* original = other.cells[i][j].ship;
            if (original) {
                for (Ship* copy : ships) {
                    if (copy->GetName() == original->GetName() && copy->GetSize() == original->GetSize()) {
                        cells[i][j].ship = copy;
                        break;
                    }
                }
            }

        }
    }
}
Grid::~Grid() { // TF: Destructor
    for (Ship* ship : ships) {
        delete ship; // TF: Dynamic Memory
    }
}
//Check the ship if it can be placed at the given position
bool Grid::IsValidPlacement(Ship* ship, int row, int col, bool horizontal) const {
    int length = ship->GetSize();
    if (horizontal) {
        if (col + length > GRID_SIZE) return false; // TF: Relational Operator
        for (int i = 0; i < length; ++i) {
            if (cells[row][col + i].state != EMPTY) return false;
        }
    }
    else {
        if (row + length > GRID_SIZE) return false;
        for (int i = 0; i < length; ++i) {
            if (cells[row + i][col].state != EMPTY) return false;
        }
    }
    return true;
}

bool Grid::PlaceShip(Ship* ship, int row, int col, bool horizontal) {
    if (!IsValidPlacement(ship, row, col, horizontal)) return false;

    int length = ship->GetSize();
    for (int i = 0; i < length; ++i) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        cells[r][c].state = SHIP;
        cells[r][c].ship = ship; // TF: Pointer Dereferenced
    }

    ships.push_back(ship); // TF: Array
    return true;
}

// Fire at the specified cell and update its state
bool Grid::FireAt(int row, int col, bool& hit, bool& sunk, string& shipName) {
    Cell& cell = cells[row][col];
    if (cell.state == HIT || cell.state == MISS) return false;

    if (cell.state == SHIP) {
        cell.state = HIT;
        cell.ship->RegisterHit();
        hit = true;
        sunk = cell.ship->IsSunk();
        shipName = cell.ship->GetName();
    }
    else {
        cell.state = MISS;
        hit = false;
        sunk = false;
    }
    return true;
}

// Display the grid
void Grid::Display(bool showShips, int startX, int startY) const {
    GotoXY(startX, startY);
    cout << "  ";
    for (int i = 0; i < GRID_SIZE; ++i) cout << setw(2) << char('A' + i);
    cout << "\n";

    for (int row = 0; row < GRID_SIZE; ++row) {
        GotoXY(startX, startY + row + 1);
        cout << setw(2) << row + 1;
        for (int col = 0; col < GRID_SIZE; ++col) {
            char symbol = '.';
            const Cell& cell = cells[row][col];
            if (cell.state == HIT) { SetRgb(COLOUR_RED_ON_BLACK); symbol = 'X'; }
            else if (cell.state == MISS) { SetRgb(COLOUR_GREEN_ON_BLACK); symbol = 'o'; }
            else if (cell.state == SHIP && showShips) { SetRgb(COLOUR_YELLOW_ON_BLACK); symbol = 'S'; }
            else SetRgb(COLOUR_WHITE_ON_BLACK);

            cout << setw(2) << symbol;
            SetRgb(COLOUR_WHITE_ON_BLACK);
        }
        cout << "\n";
    }
}

bool Grid::IsAllShipsSunk() const {
    for (Ship* ship : ships) {
        if (!ship->IsSunk()) return false;
    }
    return true;
}

// Mark the tracking grid cell as hit or miss
void Grid::MarkTrackingCell(int row, int col, bool hit) {
    cells[row][col].state = hit ? HIT : MISS; // TF: Conditional Statement
}

bool Grid::IsCellUntouched(int row, int col) const {
    CellState state = cells[row][col].state;
    return state != HIT && state != MISS;
}