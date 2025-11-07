#pragma once
#include "Player.h"
#include "Utility.h"

class CGame { // TF: Class
private: // TF: Access Specifier
	CPlayer* m_pHuman; // TF: Pointer Initialised
	CPlayer* m_pComputer;
	bool m_bDebugMode; // TF: Variable Type

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