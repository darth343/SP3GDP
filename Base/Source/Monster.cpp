#include "Monster.h"

Monster::Monster()
{
}

Monster::~Monster()
{
}

void Monster::setMonsterType(MONSTER_TYPE monType)
{
	this->monsterType = monType;
}

void Monster::setMonsterName(string name)
{
	this->monsterName = name;
}

void Monster::setMonsterHP(int hp)
{
	this->monsterHP = hp;
}

void Monster::setMonsterAtk(int atk)
{
	this->monsterAtk = atk;
}

void Monster::setMonsterDef(int def)
{
	this->monsterDef = def;
}

void Monster::setSkillSlotOneName(string skillName1)
{
	this->skillSlotOneName = skillName1;
}

void Monster::setSkillSlotTwoName(string skillName2)
{
	this->skillSlotTwoName = skillName2;
}

void Monster::setSkillSlotThreeName(string skillName3)
{
	this->skillSlotThreeName = skillName3;
}

void Monster::setSkillSlotFourName(string skillName4)
{
	this->skillSlotFourName = skillName4;
}

int Monster::getMonsterType()
{
	return monsterType;
}

string Monster::getMonsterName()
{
	return monsterName;
}

int Monster::getMonsterHP()
{
	return monsterHP;
}

int Monster::getMonsterAtk()
{
	return monsterAtk;
}

int Monster::getMonsterDef()
{
	return monsterDef;
}

string Monster::getSkillSlotOneName()
{
	return skillSlotOneName;
}

string Monster::getSkillSlotTwoName()
{
	return skillSlotTwoName;
}

string Monster::getSkillSlotThreeName()
{
	return skillSlotThreeName;
}

string Monster::getSkillSlotFourName()
{
	return skillSlotFourName;
}
