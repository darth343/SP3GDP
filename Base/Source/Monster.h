#pragma once
#include <string>

using std::string;

enum MONSTER_TYPE
{
	BANSHEE,
	CEREBUS,
	DRAGON,
	GOLEM,
	HYDRA,
	MANTICORE,
	OGRE,
	PEGASUS,
	WRAITH,
	SPHINX,
	SCYLLA,
	MINOTAUR,

	MONSTER_TOTAL,
};

enum MONSTER_SKILLS
{
	//numSKills = 48
};

class Monster
{
public:
	Monster();
	~Monster();

	void setMonsterType(MONSTER_TYPE monType);
	void setMonsterName(string  name);
	void setMonsterHP(int hp);
	void setMonsterAtk(int atk);
	void setMonsterDef(int def);

	void setSkillSlotOneName(string skillName1);
	void setSkillSlotTwoName(string skillName2);
	void setSkillSlotThreeName(string skillName3);
	void setSkillSlotFourName(string skillName4);

	int getMonsterType();
	string getMonsterName();
	int getMonsterHP();
	int getMonsterAtk();
	int getMonsterDef();

	string getSkillSlotOneName();
	string getSkillSlotTwoName();
	string getSkillSlotThreeName();
	string getSkillSlotFourName();


private:
	MONSTER_TYPE monsterType;
	string monsterName;
	int monsterHP;
	int monsterAtk;
	int monsterDef;

	string skillSlotOneName;
	string skillSlotTwoName;
	string skillSlotThreeName;
	string skillSlotFourName;


};

