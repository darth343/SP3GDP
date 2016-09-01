#pragma once
#include "Monster.h"
#include "Vector3.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Equipment
{
public:
	enum EQUIPMENT_TYPE
	{
		NO_TYPE = -1,
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
	int getLevels();
	void setLevels(int level);
	int getDamage();
	int getDefense();
	friend std::ostream& operator<<(std::ostream& 
		, EQUIPMENT_TYPE type);
	int GetTamHappy();
	int GetTamEnergy();
	int GetTamHunger();
	void SetTamHappy(int&);
	void SetTamHunger(int);
	void SetTamEnergy(int&);
	void IncrementTamHappy();
	void IncrementTamHunger(int amount = 1);
	void IncrementTamEnergy();
	void DecrementTamHappy();
	void DecrementTamHunger();
	void DecrementTamEnergy();
	Vector3 position;
	bool direction;
	float energyLosstimer;
	float hungerLosstimer;
	float happinessLosstimer;
	vector<Vector3> pooPositions;
private:
	int TamHappy;
	int TamHunger;
	int TamEnergy;
	int Levels;
	string name;
	Monster monster;
	EQUIPMENT_TYPE type;
	float damage;
	float defense;
	static vector<Equipment*> equipmentDatabase[TOTAL_ETYPE];
};

