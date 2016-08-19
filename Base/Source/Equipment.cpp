#include "Equipment.h"

Equipment::Equipment() 
	: EQAtk(10)
	, EQDef(10)
	, EQHP(10)
	, EQType(SWORD)
{
}

Equipment::~Equipment()
{
}

void Equipment::setEQType(EQUIPMENT_TYPE equipType)
{
	this->EQType = equipType;
}

void Equipment::setEQName(string name)
{
	this->EQName = name;
}

void Equipment::setEQAtk(int atk)
{
	atk = MonMon.getMonsterAtk();
	EQAtk = atk;
}

void Equipment::setEQDef(int def)
{
	def = MonMon.getMonsterDef;
	EQDef = def;
}

void Equipment::setEQHP(int hP)
{
	hP = MonMon.getMonsterHP;
	EQHP = hP;
}

int Equipment::getEQType()
{
	return EQType;
}

string Equipment::getEQName()
{
	return EQName;
}

int Equipment::getEQAtk()
{
	return EQAtk;
}

int Equipment::getEQDef()
{
	return EQDef;
}

int Equipment::getEQHP()
{
	return EQHP;
}


