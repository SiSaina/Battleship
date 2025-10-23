#pragma once
#include "Player.h"
#include "Utility.h"

class Game {
private:
    Player* human;
    Player* computer;
    bool debugMode;

    void ShowMainMenu() const;
    void ShowShipPlacementMenu() const;
    void StartNewGame();
    void ToggleDebugMode();
    void ShowGameOver();
    void DrawBorder(int width, int height) const;
public:
    Game(); // TF: Constructor
    ~Game(); // TF: Destructor

    void Run();
};