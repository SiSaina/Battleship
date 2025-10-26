#include "Game.h"

Game::Game() : debugMode(false) {
    human = new Player("Player", false);
    computer = new Player("Computer", true);
}
Game::~Game() {
    delete human;
    delete computer;
}
void Game::Run() {
    bool running = true;
    while (running) {
        ClearScreen();
        ShowMainMenu();
        int choice = GetValidMainMenuChoice((CONSOLE_WIDTH - 72), 11);

        switch (choice) {
        case 1:
            StartNewGame();
            break;
        case 2:
            ToggleDebugMode();
            break;
        case 3:
            running = false;
            break;
        default:
            SetRgbLine(COLOUR_RED_ON_BLACK, "Invalid option. Try again.", (CONSOLE_WIDTH - 72), 13);
            break;
        }
    }

    ClearScreen();
	DrawBorder(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    CenterTextColored("Thank you for playing Battleships! See you next time. :((", 13, CONSOLE_WIDTH, COLOUR_CYAN_ON_BLACK);
    system("pause > nul");
}

void Game::ShowMainMenu() const {

    ClearScreen();
    DrawBorder(CONSOLE_WIDTH, CONSOLE_HEIGHT);

    CenterTextColored("======= BATTLESHIPS =======", 2, CONSOLE_WIDTH, COLOUR_CYAN_ON_BLACK);

    // Menu options
    CenterText("1. Start New Game", 5, CONSOLE_WIDTH);
    CenterText("2. Toggle Debug Mode (" + string(debugMode ? "ON" : "OFF") + ")", 6, CONSOLE_WIDTH);
    CenterText("3. Exit", 7, CONSOLE_WIDTH);

    // Divider
    SetRgb(COLOUR_BLUE_ON_BLACK);
    CenterText("---------------------------------", 9, CONSOLE_WIDTH);
    SetRgb(COLOUR_WHITE_ON_BLACK);

}
void Game::ShowShipPlacementMenu() const {
    ClearScreen();
    DrawBorder(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    CenterTextColored("Choose Ship Placement Method", 2, CONSOLE_WIDTH, COLOUR_YELLOW_ON_BLACK);
    CenterText("1. Manual Placement", 4, CONSOLE_WIDTH);
    CenterText("2. Random Placement", 5, CONSOLE_WIDTH);
}

// === TOGGLE DEBUG ===
void Game::ToggleDebugMode() {
    debugMode = !debugMode;
    SetRgb(debugMode ? COLOUR_GREEN_ON_BLACK : COLOUR_RED_ON_BLACK);
    cout << "Debug mode is now " << (debugMode ? "ON" : "OFF") << ".\n";
    SetRgb(COLOUR_WHITE_ON_BLACK);
}

// === GAME START ===
void Game::StartNewGame() {
    ClearScreen();

    delete human;
    delete computer;
    human = new Player("Player", false);
    computer = new Player("Computer", true);

    ShowShipPlacementMenu();
    int setupChoice = GetValidPlacementMenuChoice((CONSOLE_WIDTH - 72), 9);
    bool manual = (setupChoice == 1);

    ClearInputArea(CONSOLE_WIDTH - 2, 10, 1, 15);

    human->SetupShips(manual);
    computer->SetupShips(false);
    ClearScreen();

    bool gameOver = false;
    while (!gameOver) {
        DrawBorder(CONSOLE_WIDTH, CONSOLE_HEIGHT);
        human->ShowGrids(debugMode, 30, 5);

        cout << "\n";
        computer->ShowGrids(debugMode, 50, 5);
        // Player's turn

        gameOver = human->TakeTurn(*computer, 32, 18);
        if (gameOver) break;

        // Computer's turn
        computer->TakeTurn(*human, 32, 20);
        gameOver = human->HasLost();
    }

    ClearScreen();
    ShowGameOver();
}

void Game::ShowGameOver() {
    ClearScreen();
    DrawBorder(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    CenterTextColored("=== GAME OVER ===", 2, CONSOLE_WIDTH, COLOUR_YELLOW_ON_BLACK);
    if (human->HasLost()) CenterTextColored("Defeat! The computer sunk your fleet.", 5, CONSOLE_WIDTH, COLOUR_RED_ON_BLACK);
    else CenterTextColored("Victory! You destroyed all enemy ships!", 5, CONSOLE_WIDTH, COLOUR_GREEN_ON_BLACK);
	CenterTextColored("Press any key to return to menu...", 9, CONSOLE_WIDTH, COLOUR_CYAN_ON_BLACK);
    system("pause > nul");
}