#include "AI.h"

AI::AI() : huntMode(true) { // TF: Constructor
    srand(static_cast<unsigned>(time(nullptr))); // TF: Pseudo Random Number
    InitializeTargets();
}

AI::~AI() {} // TF: Destructor

void AI::InitializeTargets() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            availableTargets.push_back({ row, col });
        }
    }
    random_shuffle(availableTargets.begin(), availableTargets.end());
}

bool AI::IsValidCoord(int row, int col) {
    return row >= 0 && row < 10 && col >= 0 && col < 10;
}

void AI::AddAdjacentTargets(int row, int col) {
    vector<pair<int, int>> directions = {
        { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }
    };

    for (auto& dir : directions) {
        int newRow = row + dir.first;
        int newCol = col + dir.second;
        if (IsValidCoord(newRow, newCol)) {
            targetQueue.push_back({ newRow, newCol });
        }
    }
}

pair<int, int> AI::GetNextTarget(bool lastHit, int lastRow, int lastCol, const Grid& trackingGrid) {
    if (lastHit) {
        huntMode = false;
        AddAdjacentTargets(lastRow, lastCol);
    }

    while (!targetQueue.empty()) {
        pair<int, int> target = targetQueue.back();
        targetQueue.pop_back();
        if (trackingGrid.IsCellUntouched(target.first, target.second)) {
            return target;
        }
    }

    huntMode = true;
    while (!availableTargets.empty()) {
        pair<int, int> target = availableTargets.back();
        availableTargets.pop_back();
        if (trackingGrid.IsCellUntouched(target.first, target.second)) {
            return target;
        }
    }


    return { -1,-1 };
}