#pragma once
#include "Grid.h"
#include "AI.h"
#include "Utility.h"

const string SHIP_NAMES[] = { "Aircraft Carrier", "Battleship", "Destroyer", "Submarine", "Patrol Boat" };
const int SHIP_SIZES[] = { 5, 4, 3, 3, 2 };

class CPlayer {
private:
    string strName;          // Player name
    bool isComputer;        // Flag for AI player
    CGrid gridOwn;           // Player's own grid (ships)
    CGrid gridTracking;      // Grid to track hits/misses on opponent

    void ManualShipPlacement();
    void RandomShipPlacement();

public:
    CPlayer();                                     // Default constructor
    CPlayer(string _strName, bool _bIsComputer);   // Param constructor with name and AI flag
    CPlayer(const CPlayer& other);

    string GetName() const;
    void SetupShips(bool bManual);
    bool TakeTurn(CPlayer& opponent, int iStartX, int iStartY);
    bool HasLost() const;
    void ShowGrids(bool bDebugMode, int iStartX, int iStartY) const;
};
