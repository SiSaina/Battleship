#include "Game.h"

CGame::CGame() : m_bDebugMode(false) {
	m_pHuman = new CPlayer("Player", false); // TF: Pointer Initialised, TF: Class Instance, // TF: Dynamic Memory
	m_pComputer = new CPlayer("Computer", true); 
}
CGame::~CGame() {
	delete m_pHuman; // TF: Dynamic Memory
    delete m_pComputer;
}
void CGame::Run() {
    bool bRunning = true;

	// TF: Iteration Statement
    while (bRunning) {
        ClearScreen();
        ShowMainMenu();
		// Get main menu choice at specified position
        int iChoice = GetValidMainMenuChoice((CONSOLE_WIDTH - 72), 11);

		// TF: Conditional Statement
        switch (iChoice) {
        case 1: StartNewGame(); break;
        case 2: ToggleDebugMode(); break;
		case 3: bRunning = false; break;
        default:
            SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid option. Try again.", (CONSOLE_WIDTH - 72), 13);
            break;
        }
    }
    ClearScreen();
	DrawBorder();
    CenterTextColored("Thank you for playing Battleships! See you next time. :((", 13, CONSOLE_WIDTH, COLOUR_CYAN_ON_BLACK);
    system("pause > nul");
}
void CGame::ShowMainMenu() const {
    ClearScreen();
    DrawBorder();
    CenterTextColored("======= BATTLESHIPS =======", 2, CONSOLE_WIDTH, COLOUR_CYAN_ON_BLACK);
    CenterText("1. Start New Game", 5, CONSOLE_WIDTH);
    CenterText("2. Toggle Debug Mode (" + string(m_bDebugMode ? "ON" : "OFF") + ")", 6, CONSOLE_WIDTH);
    CenterText("3. Exit", 7, CONSOLE_WIDTH);
	CenterTextColored("---------------------------------", 9, CONSOLE_WIDTH, COLOUR_BLUE_ON_BLACK);
}
void CGame::ShowShipPlacementMenu() const {
    ClearScreen();
    DrawBorder();

    CenterTextColored("Choose Ship Placement Method", 2, CONSOLE_WIDTH, COLOUR_YELLOW_ON_BLACK);
    CenterText("1. Manual Placement", 4, CONSOLE_WIDTH);
    CenterText("2. Random Placement", 5, CONSOLE_WIDTH);
}
// Toggle debug mode on/off
void CGame::ToggleDebugMode() {
    m_bDebugMode = !m_bDebugMode;
}
// Start a new game
void CGame::StartNewGame() {
    ClearScreen();
	// Delete previous objects and create a new one
    delete m_pHuman;
    delete m_pComputer;
    m_pHuman = new CPlayer("Player", false);
    m_pComputer = new CPlayer("Computer", true);

    ShowShipPlacementMenu();
    int iSetupChoice = GetValidPlacementMenuChoice((CONSOLE_WIDTH - 72), 9);
	bool bManual = (iSetupChoice == 1); // TF: Relational Operator

    ClearInputArea(CONSOLE_WIDTH - 2, 10, 1, 15);

    m_pHuman->SetupShips(bManual);
    m_pComputer->SetupShips(false);
    
    ClearScreen();
    bool isGameOver = false;

    while (!isGameOver) {
        DrawBorder();

        m_pHuman->ShowGrids(m_bDebugMode, 30, 5);
		m_pComputer->ShowGrids(m_bDebugMode, 50, 5);

		// TF: Pointer Dereference
		// TF: Reference
        isGameOver = m_pHuman->TakeTurn(*m_pComputer, 32, 18);
        if (isGameOver) break;

        m_pComputer->TakeTurn(*m_pHuman, 32, 20);
        isGameOver = m_pHuman->HasLost();
    }

    ClearScreen();
    ShowGameOver();
}

void CGame::ShowGameOver() {
    ClearScreen();
    DrawBorder();
    
    CenterTextColored("========= GAME OVER =========", 2, CONSOLE_WIDTH, COLOUR_YELLOW_ON_BLACK);
	// Display victory or defeat message
    if (m_pHuman->HasLost()) {
        CenterTextColored("Defeat! The Computer sunk your fleet.", 5, CONSOLE_WIDTH, COLOUR_RED_ON_BLACK);
    }
    else {
        CenterTextColored("Victory! You destroyed all enemy ships!", 5, CONSOLE_WIDTH, COLOUR_GREEN_ON_BLACK);
    }
    CenterTextColored("Press any key to return to menu...", 9, CONSOLE_WIDTH, COLOUR_CYAN_ON_BLACK);
    system("pause > nul");
}