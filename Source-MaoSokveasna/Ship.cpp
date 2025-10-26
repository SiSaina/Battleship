#include "Ship.h"

CShip::CShip(string _strName, int _iSize) : strName(_strName), iSize(_iSize), iHits(0) {} // TF: Constructor
CShip::CShip(const CShip& CShipOther) : strName(CShipOther.strName), iSize(CShipOther.iSize), iHits(CShipOther.iHits) {} // TF: Copy Constructor 

string CShip::GetName() const { return strName; } // TF: Access Specifier
int CShip::GetSize() const { return iSize; }
bool CShip::IsSunk() const { return iHits >= iSize; } // TF: Relational Operator
void CShip::RegisterHit() { iHits++; } // TF: Arithmetic Operator