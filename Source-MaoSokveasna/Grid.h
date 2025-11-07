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
    CellState eState;
    CShip* pShip;
};

class CGrid {
private:
    CCell m_arrCells[GRID_SIZE][GRID_SIZE]; // TF: Array
    vector<CShip*> m_vecpShips;

    // Check if a ship can be placed at a given position
    bool IsValidPlacement(CShip* pShip, int iRow, int iCol, bool bHorizontal) const;
public:
    CGrid();
    CGrid(const CGrid& gridOther); // TF: Copy Constructor
    ~CGrid();

    // Mark tracking cell, return hit or miss
    void MarkTrackingCell(int iRow, int iCol, bool bHit);
    void Display(bool bShowShips, int iStartX, int iStartY) const;
    // Place ship at a valid position
    bool PlaceShip(CShip* pShip, int iRow, int iCol, bool bHorizontal);
    // Fire at a cell, return hit/sunk status
    bool FireAt(int iRow, int iCol, bool& bHit, bool& bSunk, string& strShipName);
    // Check if all ships are sunk
    bool IsAllShipsSunk() const;
    // Check if cell has not been fired at yet
    bool IsCellUntouched(int iRow, int iCol) const;
};