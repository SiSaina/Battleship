#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib> // TF: Pseudo Random Number
#include <ctime>
#include <utility>
#include <algorithm>

using namespace std;

class CShip
{
private:
    string strName;
    int iSize;
    int iHits;
public:
    // TF: Class
    CShip(string _strName, int _iSize); // TF: Constructor
    CShip(const CShip& shipOther); // TF: Copy Constructor

    string GetName() const; // TF: Access Specifier
    int GetSize() const;
    bool IsSunk() const;
    void RegisterHit();
};

