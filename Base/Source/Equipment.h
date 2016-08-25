#pragma once
#include "Monster.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Equipment
{
public:
	enum EQUIPMENT_TYPE
	{
		SWORD,
		SHIELD,
		HELMET,
		ARMOUR,
		LEG,
		TOTAL_ETYPE,
	};
	Equipment();
	~Equipment();
	static void InitDatabase();
	static vector<Equipment*>* getDatabase();
	void SetName(string name);
	void SetMonster(Monster monster);
	void setType(EQUIPMENT_TYPE type);
	void setDamage(float damage);
	void setDefense(float defense);
	void reset();
	string getName();
	Monster getMonster();
	EQUIPMENT_TYPE getType();
	float getDamage();
	float getDefense();
	friend std::ostream& operator<<(std::ostream& cout, EQUIPMENT_TYPE type);
private:
	string name;
	Monster monster;
	EQUIPMENT_TYPE type;
	float damage;
	float defense;
	static vector<Equipment*> equipmentDatabase[TOTAL_ETYPE];
};

