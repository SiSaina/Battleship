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
int GetValidMainMenuChoice(int iStartX, int iStartY);
int GetValidPlacementMenuChoice(int iStartX, int iStartY);
string GetValidCoordinateInput(int iStartX, int iStartY);
char GetValidOrientationInput(int iStartX, int iStartY);
char GetValidDirectionInput(char chOrientation, int iStartX, int iStartY);

// Validation functions
bool ParseCoordinate(const string& strInput, int& iRow, int& iCol);
bool IsCoordinatorInputValid(const string& strInput);
bool IsShipOutOfBound(int iRow, int iCol, int iShipSize, bool bHorizontal);
bool IsOrientationInputValid(char chOrientation);
bool IsDirectionInputValid(char chDirection);


// Display / console functions
void DrawBorder();
void ClearScreen();
void GotoXY(int iX, int iY);
void ClearInputLine(int iConsoleWidth, int iStartX, int iStartY);
void ClearInputArea(int iConsoleWidth, int iStartX, int iStartY, int iLines);
void CenterText(const string& strText, int iRow, int iTotalWidth);
void CenterTextColored(const string& strText, int iRow, int iTotalWidth, EColour eColour);

// Colored text helpers
static void ShowInputError(const string& strMessage, int iX, int iY);
static void SetRgbLine(EColour eColour, const string& strText);
void SetRgbLine(EColour eColour, const string& strText, int iX, int iY);
void SetRgb(EColour eColour);