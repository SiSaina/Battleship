#include "Player.h"


CPlayer::CPlayer() : m_strName("Player"), m_isComputer(false) {
    srand(static_cast<unsigned>(time(nullptr))); // TF: Pesudo Random Number
}
CPlayer::CPlayer(string _m_strName, bool _m_isComputer) : m_strName(_m_strName), m_isComputer(_m_isComputer) {
	srand(static_cast<unsigned>(time(nullptr))); // TF: Pesudo Random Number
}

string CPlayer::GetName() const { return m_strName; }

void CPlayer::SetupShips(bool bManual) {
	// TF: Logical Operator
    if (bManual && !m_isComputer) ManualShipPlacement();
    else RandomShipPlacement();
}

void CPlayer::ManualShipPlacement() {
    const int kInputX = 48;
    const int kInputY = 8;
    const int kMessageY = 16;

    GotoXY(40, 2);
    cout << m_strName << ", place your ships manually.\n";

    for (int iShip = 0; iShip < 5; ++iShip) {
        bool bPlaced = false;

        while (!bPlaced) {
            SetRgbLine(COLOUR_WHITE_ON_BLACK, "Current Grid:", 20, 5);
            m_gridOwn.Display(true, 18, 6);

            GotoXY(kInputX, kInputY);
            cout << "Place " << SHIP_NAMES[iShip] << " (size " << SHIP_SIZES[iShip] << "): ";
			
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
            bPlaced = m_gridOwn.PlaceShip(pShip, iRow, iCol, bHorizontal);

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
            bPlaced = m_gridOwn.PlaceShip(pShip, iRow, iCol, bHorizontal);

            if (!bPlaced) delete pShip;
        }
    }
}

bool CPlayer::TakeTurn(CPlayer& opponent, int iStartX, int iStartY) {
    int iRow = -1;
    int iCol = -1;
    bool bValid = false;

    static AI ai;
    static bool bLastHit = false;
    static int iLastRow = -1;
    static int iLastCol = -1;

    while (!bValid) {
        if (m_isComputer) {
			// AI determines the next target
            pair<int, int> target = ai.GetNextTarget(bLastHit, iLastRow, iLastCol, m_gridTracking);
            iRow = target.first;
            iCol = target.second;
        }
        else {
            string strCoordinate;
            GotoXY(iStartX, iStartY);
            cout << m_strName << ", enter target coordinate (e.g., B7): ";
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

        bValid = opponent.m_gridOwn.FireAt(iRow, iCol, bHit, bSunk, strShipName);
        if (bValid) {
            m_gridTracking.MarkTrackingCell(iRow, iCol, bHit);
            ClearInputArea(CONSOLE_WIDTH, iStartX, iStartY + 1, 3);
            GotoXY(iStartX, iStartY + 2);
            cout << m_strName << " fired at " << char('A' + iCol) << iRow + 1 << ": ";

            if (bHit) {
                SetRgbLine(COLOUR_YELLOW_ON_BLACK, "Hit!", iStartX + 23, iStartY + 2);
                if (bSunk) {
                    if (!m_isComputer) {
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

            if (m_isComputer) {
                bLastHit = bHit;
                iLastRow = iRow;
                iLastCol = iCol;
            }
        }
        else {
            if (!m_isComputer) {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid or repeated target. Try again.", iStartX, iStartY + 1);
            }
        }
    }
    return opponent.HasLost();
}

bool CPlayer::HasLost() const {
    return m_gridOwn.IsAllShipsSunk();
}

void CPlayer::ShowGrids(bool bDebugMode, int iStartX, int iStartY) const {
    int iGridSpacing = GRID_SIZE * 2 + 6;

	// Lambda function to show a grid
    auto ShowGrid = [&](const string& title, const CGrid& grid, bool bShowShips, int iOffset) {
        GotoXY(iStartX + iOffset, iStartY);
        cout << m_strName << "'s " << title << ":\n";
        grid.Display(bShowShips, iStartX + iOffset - 1, iStartY + 1);
    };

    // In debug mode ON, show computer grid as well
    if (bDebugMode) {
        // Player: Own Left, Tracking Right
        if (!m_isComputer) {
            ShowGrid("Own Grid", m_gridOwn, true, -22);
            ShowGrid("Tracking Grid", m_gridTracking, false, iGridSpacing - 22);
        }
        // Computer: Tracking Left, Own Right
        else {
            ShowGrid("Tracking Grid", m_gridTracking, false, 17);
            ShowGrid("Own Grid", m_gridOwn, true, iGridSpacing + 17);
        }
    }
    else {
        if (!m_isComputer) {
            ShowGrid("Tracking Grid", m_gridTracking, false, 1);
            ShowGrid("Own Grid", m_gridOwn, true, iGridSpacing + 1);
        }
    }
}