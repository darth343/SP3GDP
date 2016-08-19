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

enum MONSTER_DAMAGE_SKILLS
{
	SLASH,
	BITE,
	CHARGE,
	TAILWHIP,
	SMASH,
	FLAREBREATH,
	LIGHTNINGBLAST,
	VOIDBLAST,
	ICYMIST,

	DMG_SKILL_TOTAL,
};

enum MONSTER_BUFFDEBUFF_SKILLS
{
	ROAR,
	HOWL,
	FRENZY,
	BOLSTER,

	BDB_SKILL_TOTAL,
};

enum MONSTER_SPECIFIC_SKILLS
{
	DEATHSCREAM,
	THREEPRONGATTACK,
	DRAGONFORCE,
	ROLLOUT,
	NINEHEADEDDUEL,
	WINGEDCHARGE,
	OGRESMASH,
	WINGEDASCENT,
	DEATHEMBRACE,
	CALM,
	ARROWRAIN,
	PULVERIZE,

	SPECIFIC_SKILL_TOTAL,
};

class Monster
{
public:
	Monster();
	~Monster();

	// Setters for monster data
	void setMonsterType(MONSTER_TYPE monType);
	void setMonsterName(string name);
	void setMonsterHP(int hp);
	void setMonsterAtk(int atk);
	void setMonsterDef(int def);

	// Setters for monster skill names
	void setSkillSlotOneName(string skillName1);
	void setSkillSlotTwoName(string skillName2);
	void setSkillSlotThreeName(string skillName3);
	void setSkillSlotFourName(string skillName4);

	// Setters for monster skills
	void setSkillSlotOne(MONSTER_DAMAGE_SKILLS dmgSlot1);
	void setSkillSlotTwo(MONSTER_DAMAGE_SKILLS dmgSlot2);
	void setSkillSlotThree(MONSTER_BUFFDEBUFF_SKILLS bdfSlot3);
	void setSkillSlotFour(MONSTER_SPECIFIC_SKILLS specificSlot4);

	// Getters for monster data
	int getMonsterType();
	string getMonsterName();
	int getMonsterHP();
	int getMonsterAtk();
	int getMonsterDef();

	// Setters for monster skill names
	string getSkillSlotOneName();
	string getSkillSlotTwoName();
	string getSkillSlotThreeName();
	string getSkillSlotFourName();

	// Getters for monster skills
	int getSkillSlotOne();
	int getSkillSlotTwo();
	int getSkillSlotThree();
	int getSkillSlotFour();

	// Monster Skills
	int Slash();
	int Bite();
	int Charge();
	int TailWhip();
	int Smash();
	int FlareBreath();
	int LightningBlast();
	int VoidBlast();
	int IcyMist();
	int Roar();
	int Howl();
	int Frenzy();
	int Bolster();
	int DeathScream();
	int ThreeProngAttack();
	int DragonForce();
	int RollOut();
	int NineHeadedDuel();
	int WingedCharge();
	int OgreSmash();
	int WingedAscent();
	int DeathEmbrace();
	int Calm();
	int ArrowRain();
	int Pulverize();

private:
	MONSTER_TYPE monsterType;
	string monsterName;
	int monsterHP;
	int monsterAtk;
	int monsterDef;
	int skillDmg;

	string skillSlotOneName;
	string skillSlotTwoName;
	string skillSlotThreeName;
	string skillSlotFourName;

	MONSTER_DAMAGE_SKILLS skillSlotOne;
	MONSTER_DAMAGE_SKILLS skillSlotTwo;
	MONSTER_BUFFDEBUFF_SKILLS skillSlotThree;
	MONSTER_SPECIFIC_SKILLS skillSlotFour;


};

