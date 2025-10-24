#include "Player.h"


Player::Player(string name, bool isComputer)
    : name(name), isComputer(isComputer) {
    srand(static_cast<unsigned>(time(nullptr))); // TF: Pseudo Random Number
}
Player::Player(const Player& other)
    : name(other.name), isComputer(other.isComputer),
    ownGrid(other.ownGrid), trackingGrid(other.trackingGrid) {
}
Player::~Player() {} // TF: Destructor

string Player::GetName() const { return name; }

void Player::SetupShips(bool manual) {
    if (manual && !isComputer) ManualShipPlacement();
    else RandomShipPlacement();
}

void Player::ManualShipPlacement() {
    const string shipNames[] = { "Aircraft Carrier", "Battleship", "Destroyer", "Submarine", "Patrol Boat" };
    const int shipSizes[] = { 5, 4, 3, 3, 2 };

    // Fixed layout for 120×30 console
    const int GRID_START_X = 8;     // where grid starts
    const int GRID_START_Y = 5;
    const int INPUT_X = 48;         // where inputs start (right side of grid)
    const int INPUT_Y = 8;
    const int MESSAGE_Y = 16;       // status messages appear below inputs

    // Title
    GotoXY(40, 2);
    cout << name << ", place your ships manually.\n";

    for (int i = 0; i < 5; ++i) {
        bool placed = false;

        while (!placed) {
            // Display current grid (left side)
            SetRgbLine(COLOUR_WHITE_ON_BLACK, "Current Grid:", GRID_START_X + 12, GRID_START_Y);
            ownGrid.Display(true, GRID_START_X + 10, GRID_START_Y + 1);

            // Input prompts (right side)
            GotoXY(INPUT_X, INPUT_Y);
            cout << "Place " << shipNames[i] << " (size " << shipSizes[i] << "): ";

            string coord = GetValidCoordinateInput(INPUT_X, INPUT_Y + 2);
            char orientation = GetValidOrientationInput(INPUT_X, INPUT_Y + 4);
            char direction = GetValidDirectionInput(orientation, INPUT_X, INPUT_Y + 6);

            // Parse coordinates
            int row = stoi(coord.substr(1)) - 1;
            int col = toupper(coord[0]) - 'A';
            bool horizontal = (orientation == 'H');

            // Adjust for direction
            if ((horizontal && direction == 'L') || (!horizontal && direction == 'U')) {
                if (horizontal) col -= shipSizes[i] - 1;
                else row -= shipSizes[i] - 1;
            }

            // Validate placement
            if (!IsShipOutOfBound(row, col, shipSizes[i], horizontal)) {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Ship would be out of bounds. Try again.", INPUT_X, MESSAGE_Y);
                ClearInputArea(CONSOLE_WIDTH - 14, INPUT_X, INPUT_Y, 8);
                continue;
            }

            Ship* ship = new Ship(shipNames[i], shipSizes[i]);
            placed = ownGrid.PlaceShip(ship, row, col, horizontal);

            if (!placed) {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid placement (overlap or out of bounds). Try again.", INPUT_X, MESSAGE_Y);
                delete ship;
            }
            else {
                ClearInputLine(CONSOLE_WIDTH - 50, INPUT_X, MESSAGE_Y);
                SetRgbLine(COLOUR_GREEN_ON_BLACK, "Ship placed successfully!", INPUT_X, MESSAGE_Y);
            }
            ClearInputArea(CONSOLE_WIDTH - 14, INPUT_X, INPUT_Y, 8);
            SetRgb(COLOUR_WHITE_ON_BLACK);
        }
    }
}


void Player::RandomShipPlacement() {
    const string shipNames[] = { "Aircraft Carrier", "Battleship", "Destroyer", "Submarine", "Patrol Boat" };
    const int shipSizes[] = { 5, 4, 3, 3, 2 };

    for (int i = 0; i < 5; ++i) {
        bool placed = false;
        while (!placed) {
            int row = rand() % GRID_SIZE;
            int col = rand() % GRID_SIZE;
            bool horizontal = rand() % 2;

            Ship* ship = new Ship(shipNames[i], shipSizes[i]);
            placed = ownGrid.PlaceShip(ship, row, col, horizontal);
            if (!placed) delete ship;
        }
    }
}

bool Player::TakeTurn(Player& opponent, int startX, int startY) {
    string coord;
    int row = -1, col = -1; // TF: Variable Type
    bool valid = false;

    static AI ai; // TF: Class Instance
    static bool lastHit = false;
    static int lastRow = -1, lastCol = -1;

    while (!valid) {
        if (isComputer) {
            pair<int, int> target = ai.GetNextTarget(lastHit, lastRow, lastCol, trackingGrid); // TF: Reference
            row = target.first;
            col = target.second;
        }
        else {
            GotoXY(startX, startY);
            cout << name << ", enter target coordinate (e.g., B7): ";
            cin >> coord;
            ClearInputLine(CONSOLE_WIDTH, startX, startY);

            if (!ParseCoordinate(coord, row, col)) {
                SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid format. Try again.", startX, startY + 1);
                continue;
            }
        }

        bool hit = false, sunk = false; // TF: Variable Type
        string shipName;

        valid = opponent.ownGrid.FireAt(row, col, hit, sunk, shipName); // TF: Function
        if (valid) {
            trackingGrid.MarkTrackingCell(row, col, hit);
            ClearInputArea(CONSOLE_WIDTH, startX, startY + 1, 3);
            GotoXY(startX, startY + 2);
            cout << name << " fired at " << char('A' + col) << row + 1 << ": ";
            ClearInputLine(CONSOLE_WIDTH, startX, startY + 1);
            ClearInputLine(CONSOLE_WIDTH, startX, startY + 3);

            if (hit) {
                SetRgbLine(COLOUR_YELLOW_ON_BLACK, "Hit!", startX + 23, startY + 2);
                if (sunk) {
                    if (!isComputer) SetRgbLine(COLOUR_GREEN_ON_BLACK, "You sunk the " + shipName + "!", startX, startY + 3);
                    else SetRgbLine(COLOUR_RED_ON_BLACK, "Opponent sunk your ship!", startX, startY + 3);
                }
            }
            else SetRgbLine(COLOUR_RED_ON_BLACK, "Miss.", startX + 23, startY + 2);
            if (isComputer) {
                lastHit = hit;
                lastRow = row;
                lastCol = col;
            }


        }
        else if (!isComputer) {
            SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid or repeated target. Try again.", startX, startY + 1);
        }   
    }
    return opponent.HasLost();
}

bool Player::HasLost() const {
    return ownGrid.IsAllShipsSunk();
}

void Player::ShowGrids(bool debugMode, int startX, int startY) const {
    if (debugMode || !isComputer) {
        int gridSpacing = GRID_SIZE * 2 + 6;
        GotoXY(startX + 1, startY);
        cout << name << "'s Tracking Grid:\n";
        trackingGrid.Display(false, startX, startY + 1);
        GotoXY(startX + gridSpacing + 1, startY);
        cout << name << "'s Own Grid:\n";
        ownGrid.Display(true, startX + gridSpacing, startY + 1);
    }
}