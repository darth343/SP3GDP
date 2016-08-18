#pragma once
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

	//Getters
	int getEQType();
	string getEQName();
	int getEQAtk();
	int getEQDef();
	int getEQHP();

private:
	EQUIPMENT_TYPE EQType; // Type of Equipment
	string EQName; // Name of Equipment
	int EQAtk; // Atk Val of Equipment
	int EQDef; // Def value of Equipment
	int EQHP; // HP value of Equipment
	//int crit // Crit Chance of Equipment



};

