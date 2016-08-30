#pragma once
#include <string>

using std::string;

class Monster
{
public:
	enum MONSTER_TYPE
	{
		BANSHEE,
		CEREBUS,
		DRAGON,
		GOLEM,
		BOSS,
		MONSTER_TOTAL,
	};
	Monster();
	~Monster();
	static void Exit();
	static void InitDatabase();
	static Monster getMonster(MONSTER_TYPE type);
	string getName();
	float getDefense();
	float getHealth();
	float getMaxHealth();
	float getDamage();
	void takeDamage(int damage);
	int GetType();

private:
	static Monster* database[MONSTER_TOTAL];
	MONSTER_TYPE type;
	string name;
	float defense;
	float health;
	float maxHealth;
	float damage;
};