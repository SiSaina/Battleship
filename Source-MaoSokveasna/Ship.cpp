#include "Ship.h"

Ship::Ship(string name, int size) : name(name), size(size), hits(0) {} // TF: Constructor
Ship::Ship(const Ship& other) : name(other.name), size(other.size), hits(other.hits) {} // TF: Copy Constructor 
Ship::~Ship() {} // TF: Destructor

string Ship::GetName() const { return name; } // TF: Access Specifier
int Ship::GetSize() const { return size; }
bool Ship::IsSunk() const { return hits >= size; } // TF: Relational Operator
void Ship::RegisterHit() { hits++; } // TF: Arithmetic Operator