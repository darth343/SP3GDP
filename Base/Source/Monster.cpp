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

void Monster::setSkillSlotOne(MONSTER_DAMAGE_SKILLS dmgSlot1)
{
	if (skillSlotOneName == "Slash")
		dmgSlot1 = SLASH;
	else if (skillSlotOneName == "Bite")
		dmgSlot1 = BITE;
	else if (skillSlotOneName == "Charge")
		dmgSlot1 = CHARGE;
	else if (skillSlotOneName == "Tail-Whip")
		dmgSlot1 = TAILWHIP;
	else if (skillSlotOneName == "Smash")
		dmgSlot1 = SMASH;
	else if (skillSlotOneName == "Flare-Breath")
		dmgSlot1 = FLAREBREATH;
	else if (skillSlotOneName == "Lightning-Blast")
		dmgSlot1 = LIGHTNINGBLAST;
	else if (skillSlotOneName == "Void-Blast")
		dmgSlot1 = VOIDBLAST;
	else if (skillSlotOneName == "Icy-Mist")
		dmgSlot1 = ICYMIST;

	skillSlotOne = dmgSlot1;
}

void Monster::setSkillSlotTwo(MONSTER_DAMAGE_SKILLS dmgslot2)
{
	if (skillSlotTwoName == "Slash")
		dmgslot2 = SLASH;
	else if (skillSlotTwoName == "Bite")
		dmgslot2 = BITE;
	else if (skillSlotTwoName == "Charge")
		dmgslot2 = CHARGE;
	else if (skillSlotTwoName == "Tail-Whip")
		dmgslot2 = TAILWHIP;
	else if (skillSlotTwoName == "Smash")
		dmgslot2 = SMASH;
	else if (skillSlotTwoName == "Flare-Breath")
		dmgslot2 = FLAREBREATH;
	else if (skillSlotTwoName == "Lightning-Blast")
		dmgslot2 = LIGHTNINGBLAST;
	else if (skillSlotTwoName == "Void-Blast")
		dmgslot2 = VOIDBLAST;
	else if (skillSlotTwoName == "Icy-Mist")
		dmgslot2 = ICYMIST;

	skillSlotTwo = dmgslot2;
}

void Monster::setSkillSlotThree(MONSTER_BUFFDEBUFF_SKILLS bdfSlot3)
{
	if (skillSlotThreeName == "Roar")
		bdfSlot3 = ROAR;
	else if (skillSlotThreeName == "Howl")
		bdfSlot3 = HOWL;
	else if (skillSlotThreeName == "Frenzy")
		bdfSlot3 = FRENZY;
	else if (skillSlotThreeName == "Bolster")
		bdfSlot3 = BOLSTER;

	skillSlotThree = bdfSlot3;
}

void Monster::setSkillSlotFour(MONSTER_SPECIFIC_SKILLS specificSlot4)
{
	if (skillSlotFourName == "DeathScream")
		specificSlot4 = DEATHSCREAM;
	else if (skillSlotFourName == "ThreeProngAttack")
		specificSlot4 = THREEPRONGATTACK;
	else if (skillSlotFourName == "DragonForce")
		specificSlot4 = DRAGONFORCE;
	else if (skillSlotFourName == "RollOut")
		specificSlot4 = ROLLOUT;
	else if (skillSlotFourName == "NineHeadedDuel")
		specificSlot4 = NINEHEADEDDUEL;
	else if (skillSlotFourName == "WingedCharge")
		specificSlot4 = WINGEDCHARGE;
	else if (skillSlotFourName == "OgreSmash")
		specificSlot4 = OGRESMASH;
	else if (skillSlotFourName == "WingedAscent")
		specificSlot4 = WINGEDASCENT;
	else if (skillSlotFourName == "DeathEmbrace")
		specificSlot4 = DEATHEMBRACE;
	else if (skillSlotFourName == "Calm")
		specificSlot4 = CALM;
	else if (skillSlotFourName == "ArrowRain")
		specificSlot4 = ARROWRAIN;
	else if (skillSlotFourName == "Pulverize")
		specificSlot4 = PULVERIZE;

	skillSlotFour = specificSlot4;
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

int Monster::getSkillSlotOne()
{
	return skillSlotOne;
}

int Monster::getSkillSlotTwo()
{
	return skillSlotTwo;
}

int Monster::getSkillSlotThree()
{
	return skillSlotThree;
}

int Monster::getSkillSlotFour()
{
	return skillSlotFour;
}

int Monster::Slash()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Bite()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Charge()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::TailWhip()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Smash()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Roar()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Howl()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Frenzy()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Bolster()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::FlareBreath()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::LightningBlast()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::VoidBlast()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::IcyMist()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::DeathScream()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::ThreeProngAttack()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::DragonForce()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::RollOut()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::NineHeadedDuel()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::WingedCharge()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::OgreSmash()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::WingedAscent()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::DeathEmbrace()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Calm()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::ArrowRain()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}

int Monster::Pulverize()
{
	skillDmg = monsterAtk * 1.5f;

	return skillDmg;
}