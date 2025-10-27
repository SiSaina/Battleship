#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <algorithm>

using namespace std;

class CShip {
private:
    string strName;
    int iSize;
    int iHits;
public:
    CShip(string _strName, int _iSize); // TF: Constructor
    CShip(const CShip& shipOther); // TF: Copy Constructor

    string GetName() const;
    int GetSize() const;
    bool IsSunk() const;
    void RegisterHit();
};

