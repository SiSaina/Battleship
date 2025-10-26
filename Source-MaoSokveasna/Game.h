#pragma once
#include "Player.h"
#include "Utility.h"

class CGame {
private:
	CPlayer* pHuman; // Pointer to the human player
	CPlayer* pComputer; // Pointer to the computer player
	bool bDebugMode; // Debug mode flag

    void ShowMainMenu() const;
    void ShowShipPlacementMenu() const;
    void StartNewGame();
    void ToggleDebugMode();
    void ShowGameOver();
public:
    CGame(); // TF: Constructor
    ~CGame(); // TF: Destructor
    void Run();
};