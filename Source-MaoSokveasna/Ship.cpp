#include "Ship.h"

CShip::CShip(string _strName, int _iSize) : strName(_strName), iSize(_iSize), iHits(0) {}
CShip::CShip(const CShip& CShipOther) : strName(CShipOther.strName), iSize(CShipOther.iSize), iHits(CShipOther.iHits) {}

string CShip::GetName() const { return strName; }
int CShip::GetSize() const { return iSize; }
bool CShip::IsSunk() const { return iHits >= iSize; } // TF: Relational Operator
void CShip::RegisterHit() { iHits++; } // TF: Arithmetic Operator