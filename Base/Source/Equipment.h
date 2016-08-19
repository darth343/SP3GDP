#pragma once
#include "MonsterManager.h"
#include <string>

using std::string;

enum EQUIPMENT_TYPE
{
	SWORD,
	SHIELD,
	HELM,
	ARMOUR,
	MEDALLION,

	EQUIPMENT_TOTAL,
};

class Equipment
{
public:
	Equipment();
	~Equipment();

	//Setters
	void setEQType(EQUIPMENT_TYPE equipType);
	void setEQName(string name);
	void setEQAtk(int atk);
	void setEQDef(int def);
	void setEQHP(int hP);
	void setEQSkillSlotOne(int eqslot1);
	void setEQSkillSlotTwo(int eqslot2);
	void setEQSkillSlotThree(int eqslot3);
	void setEQSkillSlotFour(int eqslot4);

	//Getters
	int getEQType();
	string getEQName();
	int getEQAtk();
	int getEQDef();
	int getEQHP();
	int getEQSkillSlotOne();
	int getEQSkillSlotTwo();
	int getEQSkillSlotThree();
	int getEQSkillSlotFour();
	

private:
	EQUIPMENT_TYPE EQType; // Type of Equipment
	string EQName; // Name of Equipment
	int EQAtk; // Atk Val of Equipment
	int EQDef; // Def value of Equipment
	int EQHP; // HP value of Equipment
	int EQSkillSlot1; // Skills transfered from Monsters
	int EQSkillSlot2; // Skills transfered from Monsters
	int EQSkillSlot3; // Skills transfered from Monsters
	int EQSkillSlot4; // Skills transfered from Monsters
	//int crit // Crit Chance of Equipment

	Monster MonMon;



};

