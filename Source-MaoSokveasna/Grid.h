#pragma once
#include "Ship.h"
#include "Utility.h"

const int GRID_SIZE = 10; // TF: Constant

enum CellState {
    CS_EMPTY,
    CS_SHIP,
    CS_HIT,
    CS_MISS
};

struct CCell {
    CellState eState; // TF: Variable Type
    CShip* pShip;      // TF: Pointer Initialised
};

class CGrid {
private:
    CCell arrCells[GRID_SIZE][GRID_SIZE]; // TF: Array
    vector<CShip*> vecpShips;

    bool IsValidPlacement(CShip* pShip, int iRow, int iCol, bool bHorizontal) const;
public:
    CGrid(); // TF: Constructor
    CGrid(const CGrid& gridOther); // TF: Copy Constructor
    ~CGrid(); // TF: Destructor

    // TF: Member Functions
    void MarkTrackingCell(int iRow, int iCol, bool bHit);
    void Display(bool bShowShips, int iStartX, int iStartY) const;
    bool PlaceShip(CShip* pShip, int iRow, int iCol, bool bHorizontal);
    bool FireAt(int iRow, int iCol, bool& bHit, bool& bSunk, string& strShipName);
    bool IsAllShipsSunk() const;
    bool IsCellUntouched(int iRow, int iCol) const;
};  