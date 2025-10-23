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

class Ship
{
private:
    string name;
    int size;
    int hits;
public:
    // TF: Class
    Ship(string name, int size); // TF: Constructor
    Ship(const Ship& other); // TF: Copy Constructor
    ~Ship(); // TF: Destructor

    string GetName() const; // TF: Access Specifier
    int GetSize() const;
    bool IsSunk() const;
    void RegisterHit();
};

