#include "Ship.h"

CShip::CShip(string _m_strName, int _m_iSize) : m_strName(_m_strName), m_iSize(_m_iSize), m_iHits(0) {}
CShip::CShip(const CShip& CShipOther) : m_strName(CShipOther.m_strName), m_iSize(CShipOther.m_iSize), m_iHits(CShipOther.m_iHits) {}

string CShip::GetName() const { return m_strName; }
int CShip::GetSize() const { return m_iSize; }
bool CShip::IsSunk() const { return m_iHits >= m_iSize; } // TF: Relational Operator
void CShip::RegisterHit() { m_iHits++; } // TF: Arithmetic Operator