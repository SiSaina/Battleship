#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <ctime>
#include "Grid.h"

using namespace std;

class AI {
private:
    vector<pair<int, int>> vecAvailableTargets; // All remaining valid targets
    vector<pair<int, int>> vecTargetQueue;      // Targets to prioritize after a hit
    bool bHuntMode;                             // True if AI is hunting for new ships

    // Initialize all possible targets (0-9 rows, 0-9 cols) and shuffle them
    void InitializeTargets();
    // Add adjacent cells to the target queue after a hit
    void AddAdjacentTargets(int iRow, int iCol);
    // Check if a coordinate is valid and within bounds
    bool IsValidCoordinate(int iRow, int iCol) const; // mark const

public:
    AI();
    
    // Determine next target based on last hit info and tracking grid
    pair<int, int> GetNextTarget(bool bLastHit, int iLastRow, int iLastCol, const CGrid& gridTracking);
};
