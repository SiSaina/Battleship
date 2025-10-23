#pragma once
#include "Grid.h"

class AI {
private:
    vector<pair<int, int>> availableTargets; // TF: Array
    vector<pair<int, int>> targetQueue; // TF: Array
    bool huntMode;

    void InitializeTargets();
    void AddAdjacentTargets(int row, int col);
    bool IsValidCoord(int row, int col);
public:
    AI(); // TF: Constructor
    ~AI(); // TF: Destructor

    pair<int, int> GetNextTarget(bool lastHit, int lastRow, int lastCol, const Grid& trackingGrid); // TF: Function
};