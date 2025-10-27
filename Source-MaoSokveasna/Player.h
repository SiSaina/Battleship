#pragma once
#include "Grid.h"
#include "AI.h"
#include "Utility.h"

const string SHIP_NAMES[] = { "Aircraft Carrier", "Battleship", "Destroyer", "Submarine", "Patrol Boat" };
const int SHIP_SIZES[] = { 5, 4, 3, 3, 2 };

class CPlayer {
private:
	// TF: Variable Types
    string strName;
    bool isComputer;
    CGrid gridOwn;
    CGrid gridTracking;

    void ManualShipPlacement();
    void RandomShipPlacement();

public:
    CPlayer();
	CPlayer(string _strName, bool _isComputer); // TF: Default Parameter Constructor

    string GetName() const;
    void SetupShips(bool bManual);

	// Player takes a turn firing at opponent
    bool TakeTurn(CPlayer& opponent, int iStartX, int iStartY);
    // Check if all ships are sunk
    bool HasLost() const;
    // Display grids (own and tracking) on screen
    void ShowGrids(bool bDebugMode, int iStartX, int iStartY) const;
};
