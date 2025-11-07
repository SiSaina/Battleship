#include "AI.h"

AI::AI() : m_bHuntMode(true) {
	srand(static_cast<unsigned>(time(nullptr))); // TF: Pseudo Random Number
    InitializeTargets();
}

void AI::InitializeTargets() {
    for (int iRow = 0; iRow < GRID_SIZE; ++iRow) {
        for (int iCol = 0; iCol < GRID_SIZE; ++iCol) {
            m_vecAvailableTargets.push_back({ iRow, iCol });
        }
    }
    random_shuffle(m_vecAvailableTargets.begin(), m_vecAvailableTargets.end());
}

bool AI::IsValidCoordinate(int iRow, int iCol) const {
    return iRow >= 0 && iRow < GRID_SIZE && iCol >= 0 && iCol < GRID_SIZE;
}

void AI::AddAdjacentTargets(int iRow, int iCol) {
    vector<pair<int, int>> vecDirections = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

    for (auto& dir : vecDirections) {
        int iNewRow = iRow + dir.first;
        int iNewCol = iCol + dir.second;
        if (IsValidCoordinate(iNewRow, iNewCol)) {
            m_vecTargetQueue.push_back({ iNewRow, iNewCol });
        }
    }
}

pair<int, int> AI::GetNextTarget(bool bLastHit, int iLastRow, int iLastCol, const CGrid& gridTracking) {
    if (bLastHit) {
        m_bHuntMode = false;
        AddAdjacentTargets(iLastRow, iLastCol); // Queue adjacent targets
    }

    // Check target queue first (targets around a hit)
    while (!m_vecTargetQueue.empty()) {
        pair<int, int> prTarget = m_vecTargetQueue.back();
        m_vecTargetQueue.pop_back();
        if (gridTracking.IsCellUntouched(prTarget.first, prTarget.second)) {
            return prTarget;
        }
    }

    // If no queued targets, return to hunting mode
    m_bHuntMode = true;
    while (!m_vecAvailableTargets.empty()) {
        pair<int, int> prTarget = m_vecAvailableTargets.back();
        m_vecAvailableTargets.pop_back();
        if (gridTracking.IsCellUntouched(prTarget.first, prTarget.second)) {
            return prTarget;
        }
    }

    // If all else fails (should not happen), return invalid
    return { -1, -1 };
}
