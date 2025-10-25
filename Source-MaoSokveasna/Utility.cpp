#include "Utility.h"

int GetValidMainMenuChoice(int startX, int startY) {
    int choice;
    while (true) {
        GotoXY(startX, startY);
        cout << "Enter your choice (1-3): ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ShowInputError("Please enter a number between 1 and 3.", startX, startY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, startX, startY);
            continue;
        }
        break;
    }
    return choice;
}

int GetValidPlacementMenuChoice(int startX, int startY) {
    int choice;
    while (true) {
        GotoXY(startX, startY);
        cout << "Enter your choice (1-2): ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 2) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ShowInputError("Invalid choice. Please enter 1 or 2.", startX, startY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, startX, startY);
            continue;
        }
        break;
    }
    return choice;
}

string GetValidCoordinateInput(int startX, int startY) {
    string coordinate;
    while (true) {
        GotoXY(startX, startY);
        cout << "Enter starting coordinate (e.g., A5): ";
        cin >> coordinate;

        if (!IsCoordinatorInputValid(coordinate)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ShowInputError("Invalid coordinate format. Try again.", startX, startY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, startX, startY);
            continue;
        }
        break;
    }
    return coordinate;
}

char GetValidOrientationInput(int startX, int startY) {
    string input;
    while (true) {
        GotoXY(startX, startY);
        cout << "Enter orientation (H for horizontal, V for vertical): ";
        cin >> input;

        if (input.length() != 1) {
            ShowInputError("Enter only one letter (H/V).", startX, startY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, startX, startY);
            continue;
        }

        char orientation = toupper(input[0]);
        if (!IsOrientationInputValid(orientation)) {
            ShowInputError("Invalid orientation input. Try again.", startX, startY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, startX, startY);
            continue;
        }

        return orientation;
    }
}

char GetValidDirectionInput(char orientation, int startX, int startY) {
    string input;
    while (true) {
        GotoXY(startX, startY);
        if (orientation == 'H')
            cout << "Direction (L for left, R for right): ";
        else
            cout << "Direction (U for up, D for down): ";
        cin >> input;

        if (input.length() != 1) {
            ShowInputError("Enter only one letter (L/R/U/D).", startX, startY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, startX, startY);
            continue;
        }

        char direction = toupper(input[0]);
        if (!IsDirectionInputValid(direction)) {
            ShowInputError("Invalid direction input. Try again.", startX, startY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, startX, startY);
            continue;
        }

        if ((orientation == 'H' && (direction == 'U' || direction == 'D')) ||
            (orientation == 'V' && (direction == 'L' || direction == 'R'))) {
            ShowInputError("Invalid direction for this orientation. Try again.", startX, startY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, startX, startY);
            continue;
        }

        return direction;
    }
}
bool ParseCoordinate(const string& input, int& row, int& col) {
    if (input.length() < 2) return false;

    char letter = toupper(input[0]); // TF: String Manipulation
    string numberPart = input.substr(1);

    if (letter < 'A' || letter > 'J') return false;
    try {
        row = stoi(numberPart) - 1;
        col = letter - 'A';
        return row >= 0 && row < 10 && col >= 0 && col < 10; // TF: Relational Operator
    }
    catch (...) {
        return false;
    }
}

bool IsCoordinatorInputValid(const string& input) {
    int row, col;
    return ParseCoordinate(input, row, col);
}
//Check if the ship placement is out of bound
bool IsShipOutOfBound(int row, int col, int shipSize, bool horizontal) {
    if (horizontal) return col >= 0 && col + shipSize <= 10;
    else return row >= 0 && row + shipSize <= 10;
}
bool IsOrientationInputValid(char orientation) {
    if (!isalpha(orientation)) return false;
    orientation = toupper(orientation);
    return (orientation == 'H' || orientation == 'V');
}
bool IsDirectionInputValid(char direction) {
    if (!isalpha(direction)) return false;
    direction = toupper(direction);
    return (direction == 'L' || direction == 'R' || direction == 'U' || direction == 'D');
}
// Draw a bordered box around the game area
void DrawBorder(int CONSOLE_WIDTH, int CONSOLE_HEIGHT) {
    SetRgb(COLOUR_BLUE_ON_BLACK);
    GotoXY(0, 0);
    cout << char(201);
    for (int i = 0; i < CONSOLE_WIDTH - 2; ++i) cout << char(205);
    cout << char(187);
    for (int i = 1; i < CONSOLE_HEIGHT - 1; ++i) {
        GotoXY(0, i);
        cout << char(186);
        GotoXY(CONSOLE_WIDTH - 1, i);
        cout << char(186);
    }
    GotoXY(0, CONSOLE_HEIGHT - 1);
    cout << char(200);
    for (int i = 0; i < CONSOLE_WIDTH - 2; ++i) cout << char(205);
    cout << char(188);
    SetRgb(COLOUR_WHITE_ON_BLACK);
}


void ClearScreen()
{
    system("cls");
}
void GotoXY(int _iX, int _iY)
{
    COORD point{};
    point.X = _iX;
    point.Y = _iY;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}
void ClearInputLine(int CONSOLE_WIDTH, int START_X, int START_Y) {
    GotoXY(START_X, START_Y);
    cout << string(CONSOLE_WIDTH - 2, ' ');
    GotoXY(START_X, START_Y);
}
void ClearInputArea(int CONSOLE_WIDTH, int startX, int startY, int lines) {
    for (int i = 0; i < lines; ++i) {
        GotoXY(startX, startY + i);
        cout << string(CONSOLE_WIDTH - startX, ' ');
    }
    GotoXY(startX, startY);
}
// Helper to center text horizontally
void CenterText(const string& text, int row, int totalWidth) {
    int startX = (totalWidth - static_cast<int>(text.size())) / 2;
    GotoXY(startX, row);
    cout << text;
}
// Helper to center text with color
void CenterTextColored(const string& text, int row, int totalWidth, EColour color) {
    SetRgb(color);
    int startX = (totalWidth - static_cast<int>(text.size())) / 2;
    GotoXY(startX, row);
    cout << text;
    SetRgb(COLOUR_WHITE_ON_BLACK); // reset color
}
static void ShowInputError(const string& message, int x, int y) {
    ClearInputLine(CONSOLE_WIDTH - 50, x, y);       // clear old message
    SetRgbLine(COLOUR_RED_ON_BLACK, message, x, y); // show colored message
}
// Print colored text (no positioning)
static void SetRgbLine(EColour colour, const string& text)
{
    SetRgb(colour);
    cout << text;
    SetRgb(COLOUR_WHITE_ON_BLACK);
}
// Print colored text at a specific console position
void SetRgbLine(EColour colour, const string& text, int x, int y)
{
    GotoXY(x, y);
    SetRgb(colour);
    cout << text;
    SetRgb(COLOUR_WHITE_ON_BLACK);
}
// Set console text color based on EColour enum
void SetRgb(EColour colour)
{
    switch (colour)
    {
    case COLOUR_WHITE_ON_BLACK: // White on Black.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case COLOUR_RED_ON_BLACK: // Red on Black.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
        break;
    case COLOUR_GREEN_ON_BLACK: // Green on Black.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        break;
    case COLOUR_YELLOW_ON_BLACK: // Yellow on Black.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        break;
    case COLOUR_BLUE_ON_BLACK: // Blue on Black.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        break;
    case COLOUR_MAGENTA_ON_BLACK: // Magenta on Black.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
        break;
    case COLOUR_CYAN_ON_BLACK: // Cyan on Black.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case COLOUR_BLACK_ON_GRAY: // Black on Gray.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_INTENSITY);
        break;
    case COLOUR_BLACK_ON_WHITE: // Black on White.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
            | BACKGROUND_GREEN | BACKGROUND_BLUE);
        break;
    case COLOUR_RED_ON_WHITE: // Red on White.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
            | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED);
        break;
    case COLOUR_GREEN_ON_WHITE: // Green on White.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
            | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN);
        break;
    case COLOUR_YELLOW_ON_WHITE: // Yellow on White.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
            | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
        break;
    case COLOUR_BLUE_ON_WHITE: // Blue on White.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
            | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE);
        break;
    case COLOUR_MAGENTA_ON_WHITE: // Magenta on White.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
            | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE);
        break;
    case COLOUR_CYAN_ON_WHITE: // Cyan on White.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
            | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case COLOUR_WHITE_ON_WHITE: // White on White.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
            | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    default: // White on Black.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN |
            FOREGROUND_BLUE);
        break;
    }
}