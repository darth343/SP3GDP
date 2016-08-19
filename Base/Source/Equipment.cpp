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
	def = MonMon.getMonsterDef();
	EQDef = def;
}

void Equipment::setEQHP(int hP)
{
	hP = MonMon.getMonsterHP();
	EQHP = hP;
}

void Equipment::setEQSkillSlotOne(int eqslot1)
{
	eqslot1 = MonMon.getSkillSlotOne();
	EQSkillSlot1 = eqslot1;
}

void Equipment::setEQSkillSlotTwo(int eqslot2)
{
	eqslot2 = MonMon.getSkillSlotTwo();
	EQSkillSlot2 = eqslot2;
}

void Equipment::setEQSkillSlotThree(int eqslot3)
{
	eqslot3 = MonMon.getSkillSlotThree();
	EQSkillSlot3 = eqslot3;
}

void Equipment::setEQSkillSlotFour(int eqslot4)
{
	eqslot4 = MonMon.getSkillSlotFour();
	EQSkillSlot4 = eqslot4;
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

int Equipment::getEQSkillSlotOne()
{
	return EQSkillSlot1;
}

int Equipment::getEQSkillSlotTwo()
{
	return EQSkillSlot2;
}

int Equipment::getEQSkillSlotThree()
{
	return EQSkillSlot3;
}

int Equipment::getEQSkillSlotFour()
{
	return EQSkillSlot4;
}


