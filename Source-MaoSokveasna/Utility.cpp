#include "Utility.h"

int GetValidMainMenuChoice(int iStartX, int iStartY) {
    int iChoice;
    while (true) {
        GotoXY(iStartX, iStartY);
        cout << "Enter your choice (1-3): ";
        cin >> iChoice;
        if (cin.fail() || iChoice < 1 || iChoice > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ShowInputError("Please enter a number between 1 and 3.", iStartX, iStartY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);
            continue;
        }
        break;
    }
    return iChoice;
}

int GetValidPlacementMenuChoice(int iStartX, int iStartY) {
    int iChoice;
    while (true) {
        GotoXY(iStartX, iStartY);
        cout << "Enter your choice (1-2): ";
        cin >> iChoice;
        if (cin.fail() || iChoice < 1 || iChoice > 2) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ShowInputError("Invalid choice. Please enter 1 or 2.", iStartX, iStartY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);
            continue;
        }
        break;
    }
    return iChoice;
}

string GetValidCoordinateInput(int iStartX, int iStartY) {
    string strCoordinate;
    while (true) {
        GotoXY(iStartX, iStartY);
        cout << "Enter starting coordinate (e.g., A5): ";
        cin >> strCoordinate;

        if (!IsCoordinatorInputValid(strCoordinate)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ShowInputError("Invalid coordinate format. Try again.", iStartX, iStartY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);
            continue;
        }
        break;
    }
    return strCoordinate;
}

char GetValidOrientationInput(int iStartX, int iStartY) {
    string strInput;
    while (true) {
        GotoXY(iStartX, iStartY);
        cout << "Enter orientation (H for horizontal, V for vertical): ";
        cin >> strInput;

        if (strInput.length() != 1) {
            ShowInputError("Enter only one letter (H/V).", iStartX, iStartY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);
            continue;
        }

        char chOrientation = toupper(strInput[0]);
        if (!IsOrientationInputValid(chOrientation)) {
            ShowInputError("Invalid orientation input. Try again.", iStartX, iStartY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);
            continue;
        }

        return chOrientation;
    }
}

char GetValidDirectionInput(char chOrientation, int iStartX, int iStartY) {
    string strInput;
    while (true) {
        GotoXY(iStartX, iStartY);
        if (chOrientation == 'H')
            cout << "Direction (L for left, R for right): ";
        else
            cout << "Direction (U for up, D for down): ";
        cin >> strInput;

        if (strInput.length() != 1) {
            ShowInputError("Enter only one letter (L/R/U/D).", iStartX, iStartY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);
            continue;
        }

        char chDirection = toupper(strInput[0]);
        if (!IsDirectionInputValid(chDirection)) {
            ShowInputError("Invalid direction input. Try again.", iStartX, iStartY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);
            continue;
        }

        if ((chOrientation == 'H' && (chDirection == 'U' || chDirection == 'D')) ||
            (chOrientation == 'V' && (chDirection == 'L' || chDirection == 'R'))) {
            ShowInputError("Invalid direction for this orientation. Try again.", iStartX, iStartY + 2);
            ClearInputLine(CONSOLE_WIDTH - 50, iStartX, iStartY);
            continue;
        }

        return chDirection;
    }
}

bool ParseCoordinate(const string& strInput, int& iRow, int& iCol) {
    if (strInput.length() < 2) return false;

    char chLetter = toupper(strInput[0]);
    string strNumberPart = strInput.substr(1);

    if (chLetter < 'A' || chLetter > 'J') return false;
    try {
        iRow = stoi(strNumberPart) - 1;
        iCol = chLetter - 'A';
        return iRow >= 0 && iRow < 10 && iCol >= 0 && iCol < 10;
    }
    catch (...) {
        return false;
    }
}

bool IsCoordinatorInputValid(const string& strInput) {
    int iRow, iCol;
    return ParseCoordinate(strInput, iRow, iCol);
}

bool IsShipOutOfBound(int iRow, int iCol, int iShipSize, bool bHorizontal) {
    if (bHorizontal) return iCol >= 0 && iCol + iShipSize <= 10;
    else return iRow >= 0 && iRow + iShipSize <= 10;
}

bool IsOrientationInputValid(char chOrientation) {
    if (!isalpha(chOrientation)) return false;
    chOrientation = toupper(chOrientation);
    return (chOrientation == 'H' || chOrientation == 'V');
}

bool IsDirectionInputValid(char chDirection) {
    if (!isalpha(chDirection)) return false;
    chDirection = toupper(chDirection);
    return (chDirection == 'L' || chDirection == 'R' || chDirection == 'U' || chDirection == 'D');
}

void DrawBorder() {
    SetRgb(COLOUR_BLUE_ON_BLACK);
    GotoXY(0, 0);
    cout << char(201);
    for (int i = 0; i < CONSOLE_WIDTH - 2; ++i) cout << char(205);
    cout << char(187);

    for (int i = 1; i < CONSOLE_HEIGHT - 1; ++i) {
        GotoXY(0, i); cout << char(186);
        GotoXY(CONSOLE_WIDTH - 1, i); cout << char(186);
    }

    GotoXY(0, CONSOLE_HEIGHT - 1);
    cout << char(200);
    for (int i = 0; i < CONSOLE_WIDTH - 2; ++i) cout << char(205);
    cout << char(188);
    SetRgb(COLOUR_WHITE_ON_BLACK);
}

void ClearScreen() {
    system("cls");
}

void GotoXY(int iX, int iY) {
    COORD coord{};
    coord.X = iX;
    coord.Y = iY;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ClearInputLine(int iConsoleWidth, int iStartX, int iStartY) {
    GotoXY(iStartX, iStartY);
    cout << string(iConsoleWidth - 2, ' ');
    GotoXY(iStartX, iStartY);
}

void ClearInputArea(int iConsoleWidth, int iStartX, int iStartY, int iLines) {
    for (int i = 0; i < iLines; ++i) {
        GotoXY(iStartX, iStartY + i);
        cout << string(iConsoleWidth - iStartX, ' ');
    }
    GotoXY(iStartX, iStartY);
}

void CenterText(const string& strText, int iRow, int iTotalWidth) {
    int iStartX = (iTotalWidth - static_cast<int>(strText.size())) / 2;
    GotoXY(iStartX, iRow);
    cout << strText;
}

void CenterTextColored(const string& strText, int iRow, int iTotalWidth, EColour eColour) {
    SetRgb(eColour);
    int iStartX = (iTotalWidth - static_cast<int>(strText.size())) / 2;
    GotoXY(iStartX, iRow);
    cout << strText;
    SetRgb(COLOUR_WHITE_ON_BLACK);
}

static void ShowInputError(const string& strMessage, int iX, int iY) {
    ClearInputLine(CONSOLE_WIDTH - 50, iX, iY);
    SetRgbLine(COLOUR_RED_ON_BLACK, strMessage, iX, iY);
}

static void SetRgbLine(EColour eColour, const string& strText) {
    SetRgb(eColour);
    cout << strText;
    SetRgb(COLOUR_WHITE_ON_BLACK);
}

void SetRgbLine(EColour eColour, const string& strText, int iX, int iY) {
    GotoXY(iX, iY);
    SetRgb(eColour);
    cout << strText;
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