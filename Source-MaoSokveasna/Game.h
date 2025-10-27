#pragma once
#include "Player.h"
#include "Utility.h"

class CGame { // TF: Class
private: // TF: Access Specifier
	CPlayer* pHuman; // TF: Pointer Initialised
	CPlayer* pComputer;
	bool bDebugMode; // TF: Variable Type

    void ShowMainMenu() const; // TF: Constant Member Function
    void ShowShipPlacementMenu() const;
    void StartNewGame();
    void ToggleDebugMode();
    void ShowGameOver();
public:
    CGame(); // TF: Constructor
    ~CGame(); // TF: Destructor
    void Run();
};