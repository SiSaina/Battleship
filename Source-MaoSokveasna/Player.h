#pragma once
#include "Grid.h"
#include "AI.h"
#include "Utility.h"

class Player {
private:
    string name;
    bool isComputer; // TF: Variable Type
    Grid ownGrid;
    Grid trackingGrid;

    void ManualShipPlacement();
    void RandomShipPlacement();
public:
    Player(string name, bool isComputer); // TF: Constructor
    Player(const Player& other); // TF: Copy Constructor Deleted
    ~Player(); // TF: Destructor

    void SetupShips(bool manual);
    bool TakeTurn(Player& opponent, int startX, int startY);
    bool HasLost() const;
    void ShowGrids(bool debugMode, int startX, int startY) const;
    string GetName() const;
};