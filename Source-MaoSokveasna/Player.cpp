#include "Player.h"


CPlayer::CPlayer() : strName("Player"), isComputer(false) {
    srand(static_cast<unsigned>(time(nullptr))); // TF: Pesudo Random Number
}
CPlayer::CPlayer(string _strName, bool _bIsComputer)
    : strName(_strName), isComputer(_bIsComputer) {
	srand(static_cast<unsigned>(time(nullptr))); // TF: Pesudo Random Number
}
CPlayer::CPlayer(const CPlayer& other)
    : strName(other.strName), isComputer(other.isComputer),
    gridOwn(other.gridOwn), gridTracking(other.gridTracking) {
}

string CPlayer::GetName() const { return strName; }

void CPlayer::SetupShips(bool bManual) {
    if (bManual && !isComputer) ManualShipPlacement();
    else RandomShipPlacement();
}

void CPlayer::ManualShipPlacement() {
    const int kInputX = 48;
    const int kInputY = 8;
    const int kMessageY = 16;

    GotoXY(40, 2);
    cout << strName << ", place your ships manually.\n";

    for (int iShip = 0; iShip < 5; ++iShip) {
        bool bPlaced = false;

        while (!bPlaced) {
            // Display current grid
            SetRgbLine(COLOUR_WHITE_ON_BLACK, "Current Grid:", 20, 5);
            gridOwn.Display(true, 18, 6);

            GotoXY(kInputX, kInputY);
            cout << "Place " << SHIP_NAMES[iShip] << " (size " << SHIP_SIZES[iShip] << "): ";
			// Get inputs from user (Coordinate, Orientation, Direction)
            string strCoordinate = GetValidCoordinateInput(kInputX, kInputY + 2);
            char cOrientation = GetValidOrientationInput(kInputX, kInputY + 4);
            char cDirection = GetValidDirectionInput(cOrientation, kInputX, kInputY + 6);

            int iRow = stoi(strCoordinate.substr(1)) - 1;
            int iCol = toupper(strCoordinate[0]) - 'A';
            bool bHorizontal = (cOrientation == 'H');

			// Adjust starting position based on direction
            if ((bHorizontal && cDirection == 'L') || (!bHorizontal && cDirection == 'U')) {
                if (bHorizontal) iCol -= SHIP_SIZES[iShip] - 1;
                else iRow -= SHIP_SIZES[iShip] - 1;
            }
			// Check ships out of bounds
            if (!IsShipOutOfBound(iRow, iCol, SHIP_SIZES[iShip], bHorizontal)) {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Ship would be out of bounds. Try again.", kInputX, kMessageY);
                ClearInputArea(CONSOLE_WIDTH - 14, kInputX, kInputY, 8);
                continue;
            }

            CShip* pShip = new CShip(SHIP_NAMES[iShip], SHIP_SIZES[iShip]);
            bPlaced = gridOwn.PlaceShip(pShip, iRow, iCol, bHorizontal);

            if (!bPlaced) {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid placement (overlap or out of bounds). Try again.", kInputX, kMessageY);
                delete pShip;
            }
            else {
                ClearInputLine(CONSOLE_WIDTH - 50, kInputX, kMessageY);
                SetRgbLine(COLOUR_GREEN_ON_BLACK, "Ship placed successfully!", kInputX, kMessageY);
            }

            ClearInputArea(CONSOLE_WIDTH - 14, kInputX, kInputY, 8);
            SetRgb(COLOUR_WHITE_ON_BLACK);
        }
    }
}

void CPlayer::RandomShipPlacement() {
    for (int iShip = 0; iShip < 5; ++iShip) {
        bool bPlaced = false;
        while (!bPlaced) {
            int iRow = rand() % GRID_SIZE;
            int iCol = rand() % GRID_SIZE;
            bool bHorizontal = rand() % 2;

            CShip* pShip = new CShip(SHIP_NAMES[iShip], SHIP_SIZES[iShip]);
            bPlaced = gridOwn.PlaceShip(pShip, iRow, iCol, bHorizontal);
            if (!bPlaced) delete pShip;
        }
    }
}

// Take a turn against the opponent
bool CPlayer::TakeTurn(CPlayer& opponent, int iStartX, int iStartY) {
    string strCoordinate;
    int iRow = -1;
    int iCol = -1;
    bool bValid = false;

    static AI ai;
    static bool bLastHit = false;
    static int iLastRow = -1;
    static int iLastCol = -1;

    while (!bValid) {
        if (isComputer) {
            pair<int, int> target = ai.GetNextTarget(bLastHit, iLastRow, iLastCol, gridTracking);
            iRow = target.first;
            iCol = target.second;
        }
        else {
            GotoXY(iStartX, iStartY);
            cout << strName << ", enter target coordinate (e.g., B7): ";
            cin >> strCoordinate;
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);

            if (!ParseCoordinate(strCoordinate, iRow, iCol)) {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid format. Try again.", iStartX, iStartY + 1);
                continue;
            }
        }

        bool bHit = false;
        bool bSunk = false;
        string strShipName;

        bValid = opponent.gridOwn.FireAt(iRow, iCol, bHit, bSunk, strShipName);
        if (bValid) {
            gridTracking.MarkTrackingCell(iRow, iCol, bHit);
            ClearInputArea(CONSOLE_WIDTH, iStartX, iStartY + 1, 3);
            GotoXY(iStartX, iStartY + 2);
            cout << strName << " fired at " << char('A' + iCol) << iRow + 1 << ": ";

            if (bHit) {
                SetRgbLine(COLOUR_YELLOW_ON_BLACK, "Hit!", iStartX + 23, iStartY + 2);
                if (bSunk) {
                    if (!isComputer) {
                        SetRgbLine(COLOUR_GREEN_ON_BLACK, "You sunk the " + strShipName + "!", iStartX, iStartY + 1);
                    }
                    else {
                        SetRgbLine(COLOUR_RED_ON_BLACK, "Opponent sunk your ship!", iStartX, iStartY + 3);
                    }
                }
            }
            else {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Miss.", iStartX + 23, iStartY + 2);
            }

            if (isComputer) {
                bLastHit = bHit;
                iLastRow = iRow;
                iLastCol = iCol;
            }
        }
        else {
            if (!isComputer) {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid or repeated target. Try again.", iStartX, iStartY + 1);
            }
        }
    }
    return opponent.HasLost();
}

// Check if all ships are sunk
bool CPlayer::HasLost() const {
    return gridOwn.IsAllShipsSunk();
}

// Display grids (own and tracking) on screen
void CPlayer::ShowGrids(bool bDebugMode, int iStartX, int iStartY) const {
    int iGridSpacing = GRID_SIZE * 2 + 6;

	// In debug mode, show computer grid as well
    if (bDebugMode) {
        if (!isComputer) {
			// Display own grid on the left, tracking grid on the right for player
            GotoXY(iStartX - 22, iStartY);
            cout << strName << "'s Own Grid:\n";
            gridOwn.Display(true, iStartX - 23, iStartY + 1);

            GotoXY(iStartX + iGridSpacing - 22, iStartY);
            cout << strName << "'s Tracking Grid:\n";
            gridTracking.Display(false, iStartX + iGridSpacing - 23, iStartY + 1);
            return;
        }
        else {
			// Display tracking grid on the left, own grid on the right for computer
            GotoXY(iStartX + 17, iStartY);
            cout << strName << "'s Tracking Grid:\n";
            gridTracking.Display(false, iStartX + 16, iStartY + 1);

            GotoXY(iStartX + iGridSpacing + 17, iStartY);
            cout << strName << "'s Own Grid:\n";
            gridOwn.Display(true, iStartX + iGridSpacing + 16, iStartY + 1);
        }
    }
    else {
        if (!isComputer) {
            // Display tracking grid on the left, own grid on the right for player
            GotoXY(iStartX + 1, iStartY);
            cout << strName << "'s Tracking Grid:\n";
            gridTracking.Display(false, iStartX, iStartY + 1);

            GotoXY(iStartX + iGridSpacing + 1, iStartY);
            cout << strName << "'s Own Grid:\n";
            gridOwn.Display(true, iStartX + iGridSpacing, iStartY + 1);
        }
    }
}