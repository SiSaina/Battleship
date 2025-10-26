#include "Grid.h"

CGrid::CGrid() {
    // Initialize all cells to empty and no ship
    for (int iRow = 0; iRow < GRID_SIZE; ++iRow) {
        for (int iCol = 0; iCol < GRID_SIZE; ++iCol) {
            arrCells[iRow][iCol].eState = CS_EMPTY;
            arrCells[iRow][iCol].pShip = nullptr;
        }
    }
}
// Copy constructor
CGrid::CGrid(const CGrid& gridOther) {
    // Deep copy ships
    for (CShip* pShip : gridOther.vecpShips) {
        vecpShips.push_back(new CShip(*pShip));
    }

    // Copy cell states and relink ship pointers
    for (int iRow = 0; iRow < GRID_SIZE; ++iRow) {
        for (int iCol = 0; iCol < GRID_SIZE; ++iCol) {
            arrCells[iRow][iCol].eState = gridOther.arrCells[iRow][iCol].eState;
            arrCells[iRow][iCol].pShip = nullptr;

            CShip* pOriginalShip = gridOther.arrCells[iRow][iCol].pShip;
            if (pOriginalShip) {
                for (CShip* pCopyShip : vecpShips) {
                    if (pCopyShip->GetName() == pOriginalShip->GetName() &&
                        pCopyShip->GetSize() == pOriginalShip->GetSize()) {
                        arrCells[iRow][iCol].pShip = pCopyShip;
                        break;
                    }
                }
            }
        }
    }
}

CGrid::~CGrid() {
    for (CShip* pShip : vecpShips) {
        delete pShip;
    }
}

// Check if a ship can be placed at a given position
bool CGrid::IsValidPlacement(CShip* pShip, int iRow, int iCol, bool bHorizontal) const {
    int iLength = pShip->GetSize();
    if (bHorizontal) {
        if (iCol + iLength > GRID_SIZE) return false;
        for (int i = 0; i < iLength; ++i) {
            if (arrCells[iRow][iCol + i].eState != CS_EMPTY) return false;
        }
    }
    else {
        if (iRow + iLength > GRID_SIZE) return false;
        for (int i = 0; i < iLength; ++i) {
            if (arrCells[iRow + i][iCol].eState != CS_EMPTY) return false;
        }
    }
    return true;
}

// Place ship at a valid position
bool CGrid::PlaceShip(CShip* pShip, int iRow, int iCol, bool bHorizontal) {
    if (!IsValidPlacement(pShip, iRow, iCol, bHorizontal)) return false;

    int iLength = pShip->GetSize();
    for (int i = 0; i < iLength; ++i) {
        int r = iRow + (bHorizontal ? 0 : i);
        int c = iCol + (bHorizontal ? i : 0);
        arrCells[r][c].eState = CS_SHIP;
        arrCells[r][c].pShip = pShip;
    }

    vecpShips.push_back(pShip);
    return true;
}

// Fire at a cell, return hit/sunk status
bool CGrid::FireAt(int iRow, int iCol, bool& bHit, bool& bSunk, string& strShipName) {
    CCell& cell = arrCells[iRow][iCol];

    if (cell.eState == CS_HIT || cell.eState == CS_MISS) return false;

    if (cell.eState == CS_SHIP) {
        cell.eState = CS_HIT;
        cell.pShip->RegisterHit();
        bHit = true;
        bSunk = cell.pShip->IsSunk();
        strShipName = cell.pShip->GetName();
    }
    else {
        cell.eState = CS_MISS;
        bHit = false;
        bSunk = false;
    }

    return true;
}

// Display grid on console
void CGrid::Display(bool bShowShips, int nStartX, int nStartY) const {
    GotoXY(nStartX, nStartY);
    cout << "  ";
    for (int i = 0; i < GRID_SIZE; ++i) cout << setw(2) << char('A' + i);

    for (int iRow = 0; iRow < GRID_SIZE; ++iRow) {
        GotoXY(nStartX, nStartY + iRow + 1);
        cout << setw(2) << iRow + 1;

        for (int iCol = 0; iCol < GRID_SIZE; ++iCol) {
            char cSymbol = '.';
            const CCell& cell = arrCells[iRow][iCol];

            if (cell.eState == CS_HIT) {
                SetRgb(COLOUR_RED_ON_BLACK); 
                cSymbol = 'X'; 
            }
            else if (cell.eState == CS_MISS) { 
                SetRgb(COLOUR_GREEN_ON_BLACK); 
                cSymbol = 'o'; 
            }
            else if (cell.eState == CS_SHIP && bShowShips) { 
                SetRgb(COLOUR_YELLOW_ON_BLACK); 
                cSymbol = 'S'; 
            }
            else {
                SetRgb(COLOUR_WHITE_ON_BLACK);
            }

            cout << setw(2) << cSymbol;
            SetRgb(COLOUR_WHITE_ON_BLACK);
        }
    }
}

// Check if all ships are sunk
bool CGrid::IsAllShipsSunk() const {
    for (CShip* pShip : vecpShips) {
        if (!pShip->IsSunk()) return false;
    }
    return true;
}

// Mark tracking cell as hit or miss
void CGrid::MarkTrackingCell(int iRow, int iCol, bool bHit) {
    arrCells[iRow][iCol].eState = bHit ? CS_HIT : CS_MISS;
}

// Check if cell has not been fired at yet
bool CGrid::IsCellUntouched(int iRow, int iCol) const {
    CellState eState = arrCells[iRow][iCol].eState;
    return eState != CS_HIT && eState != CS_MISS;
}