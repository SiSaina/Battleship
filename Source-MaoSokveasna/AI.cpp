#include "AI.h"

AI::AI() : bHuntMode(true) {
	srand(static_cast<unsigned>(time(nullptr))); // TF: Pseudo Random Number
    InitializeTargets();
}

// Initialize all possible targets (0-9 rows, 0-9 cols) and shuffle them
void AI::InitializeTargets() {
    for (int iRow = 0; iRow < GRID_SIZE; ++iRow) {
        for (int iCol = 0; iCol < GRID_SIZE; ++iCol) {
            vecAvailableTargets.push_back({ iRow, iCol });
        }
    }
    random_shuffle(vecAvailableTargets.begin(), vecAvailableTargets.end());
}

// Check if coordinate is within the grid bounds
bool AI::IsValidCoordinate(int iRow, int iCol) const {
    return iRow >= 0 && iRow < GRID_SIZE && iCol >= 0 && iCol < GRID_SIZE;
}

// Add adjacent cells to the queue to target after a hit
void AI::AddAdjacentTargets(int iRow, int iCol) {
    vector<pair<int, int>> vecDirections = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

    for (auto& dir : vecDirections) {
        int iNewRow = iRow + dir.first;
        int iNewCol = iCol + dir.second;
        if (IsValidCoordinate(iNewRow, iNewCol)) {
            vecTargetQueue.push_back({ iNewRow, iNewCol });
        }
    }
}

// Determine next target based on last hit info and tracking grid
pair<int, int> AI::GetNextTarget(bool bLastHit, int iLastRow, int iLastCol, const CGrid& gridTracking) {
    if (bLastHit) {
        bHuntMode = false;
        AddAdjacentTargets(iLastRow, iLastCol); // Queue adjacent targets
    }

    // Check target queue first (targets around a hit)
    while (!vecTargetQueue.empty()) {
        pair<int, int> prTarget = vecTargetQueue.back();
        vecTargetQueue.pop_back();
        if (gridTracking.IsCellUntouched(prTarget.first, prTarget.second)) {
            return prTarget;
        }
    }

    // If no queued targets, return to hunting mode
    bHuntMode = true;
    while (!vecAvailableTargets.empty()) {
        pair<int, int> prTarget = vecAvailableTargets.back();
        vecAvailableTargets.pop_back();
        if (gridTracking.IsCellUntouched(prTarget.first, prTarget.second)) {
            return prTarget;
        }
    }

    // If all else fails (should not happen), return invalid
    return { -1, -1 };
}
