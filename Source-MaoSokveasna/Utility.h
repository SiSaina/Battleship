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

int GetValidMainMenuChoice(int startX, int startY);
int GetValidPlacementMenuChoice(int startX, int startY);
string GetValidCoordinateInput(int startX, int startY);
char GetValidOrientationInput(int startX, int startY);
char GetValidDirectionInput(char orientation, int startX, int startY);

bool ParseCoordinate(const string& input, int& row, int& col); // TF: Function
bool IsCoordinatorInputValid(const string& input); // TF: Function
bool IsShipOutOfBound(int row, int col, int shipSize, bool horizontal); // TF: Function
bool IsOrientationInputValid(char orientation); // TF: Function
bool IsDirectionInputValid(char direction); // TF: Function
void DrawBorder(int CONSOLE_WIDTH, int CONSOLE_HEIGHT);
void ClearScreen();
void GotoXY(int x, int y);
// Clear a single line of input
void ClearInputLine(int CONSOLE_WIDTH, int START_X, int START_Y);
// Clear multiple lines of input area
void ClearInputArea(int CONSOLE_WIDTH, int startX, int startY, int lines);
// Center text horizontally in the console
void CenterText(const string& text, int row, int totalWidth);
// Center colored text horizontally in the console
void CenterTextColored(const string& text, int row, int totalWidth, EColour color);
// Show input error message in red at specified position
static void ShowInputError(const string& message, int x, int y);
// Print colored text (no positioning)
static void SetRgbLine(EColour colour, const string& text);
// Print colored text at a specific console position
void SetRgbLine(EColour colour, const string& text, int x, int y);
// Set console text color based on EColour enum
void SetRgb(EColour colour);