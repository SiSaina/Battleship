#pragma once
#include <iostream>
#include <string>
#define NOMINMAX
#include <windows.h>
#include <limits>
using namespace std;

// Constants
const int CONSOLE_WIDTH = 120;
const int CONSOLE_HEIGHT = 30;
enum EColour
{
	COLOUR_WHITE_ON_BLACK = 0, // White on Black.
	COLOUR_RED_ON_BLACK = 1, // Red on Black.
	COLOUR_GREEN_ON_BLACK = 2, // Green on Black.
	COLOUR_YELLOW_ON_BLACK = 3, // Yellow on Black.
	COLOUR_BLUE_ON_BLACK = 4, // Blue on Black.
	COLOUR_MAGENTA_ON_BLACK = 5, // Magenta on Black.
	COLOUR_CYAN_ON_BLACK = 6, // Cyan on Black.
	COLOUR_BLACK_ON_GRAY = 7, // Black on Gray.
	COLOUR_BLACK_ON_WHITE = 8, // Black on White.
	COLOUR_RED_ON_WHITE = 9, // Red on White.
	COLOUR_GREEN_ON_WHITE = 10, // Green on White.
	COLOUR_YELLOW_ON_WHITE = 11, // Yellow on White.
	COLOUR_BLUE_ON_WHITE = 12, // Blue on White.
	COLOUR_MAGENTA_ON_WHITE = 13,// Magenta on White.
	COLOUR_CYAN_ON_WHITE = 14, // Cyan on White.
	COLOUR_WHITE_ON_WHITE = 15 // White on White.
};

// Input functions
// Get validated menu choices and inputs
int GetValidMainMenuChoice(int iStartX, int iStartY);
// Get validated placement menu choice
int GetValidPlacementMenuChoice(int iStartX, int iStartY);
// Get validated coordinate input
string GetValidCoordinateInput(int iStartX, int iStartY);
// Get validated orientation input
char GetValidOrientationInput(int iStartX, int iStartY);
// Get validated direction input
char GetValidDirectionInput(char chOrientation, int iStartX, int iStartY);

// Validation functions
// Parse and validate coordinator inputs
bool ParseCoordinate(const string& strInput, int& iRow, int& iCol);
// Check validity of coordinate inputs
bool IsCoordinatorInputValid(const string& strInput);
// Check if ship placement is out of bounds
bool IsShipOutOfBound(int iRow, int iCol, int iShipSize, bool bHorizontal);
// Check validity of orientation input
bool IsOrientationInputValid(char chOrientation);
// Check validity of direction input
bool IsDirectionInputValid(char chDirection);


// Display / console functions
void DrawBorder();
void ClearScreen();
void GotoXY(int iX, int iY);
// Clear specific input line
void ClearInputLine(int iConsoleWidth, int iStartX, int iStartY);
// Clear specific input area (multiple lines)
void ClearInputArea(int iConsoleWidth, int iStartX, int iStartY, int iLines);
// Center text helpers
void CenterText(const string& strText, int iRow, int iTotalWidth);
// Center colored text helpers
void CenterTextColored(const string& strText, int iRow, int iTotalWidth, EColour eColour);

// Colored text helpers
// Show input error message at specific position
static void ShowInputError(const string& strMessage, int iX, int iY);
// Set colored text in current position
static void SetRgbLine(EColour eColour, const string& strText);
// Set colored text at specific position
void SetRgbLine(EColour eColour, const string& strText, int iX, int iY);
void SetRgb(EColour eColour);