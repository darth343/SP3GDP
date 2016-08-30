#pragma once
#include "Vector3.h"
#include "PlayerInfo.h"
#include "Enemy.h"

using std::string;

class BattleSystem
{
public:
	enum BATTLE_SELECTION
	{
		BS_ATTACK = 1,
		BS_ITEM,
		BS_CAPTURE,
		BS_RUN,

		//Attack
		BS_SLASH,
		BS_STAB,
		BS_SKILL,
		BS_BACK,

		BS_TOTAL,
	};

	BattleSystem();
	~BattleSystem();

	void SetBattleStart(bool set);
	void SetArrowPos(float x, float y, float z);
	void SetFirstChoice(bool set);
	void SetSecondChoice(bool set);
	void SetEscapeChance(float percetage);
	void SetBattleSelection(BATTLE_SELECTION selection);
	void SetMonsterHitAnimation(bool set);

	//new
	void EndPlayerTurn();
	void EndEnemyTurn();

	void Reset();

	bool GetBattleStart();
	float GetArrowPosX();
	float GetArrowPosY();
	bool GetFirstChoice();
	bool GetSecondChoice();
	bool GetMonsterHitAnimation();
	BATTLE_SELECTION GetBattleSelection();

	void UpdateBattleSystem(static bool& UPkeyPressed, static bool& DNkeyPressed, static bool& LEFTkeyPressed, static bool& RIGHTkeyPressed, static bool& ENTERkeyPressed, CPlayerInfo* theHero, Enemy* enemy);
	void RunBattleChoice(CPlayerInfo* theHero, Enemy* enemy);
	void GetBattleChoiceInput(static bool& UPkeyPressed, static bool& DNkeyPressed, static bool& LEFTkeyPressed, static bool& RIGHTkeyPressed, static bool& ENTERkeyPressed);
private:

	bool battleStart;				  //Detect if the battle has started

	Vector3 arrowPos;				  //Arrow's placing for Battle Scene 

	bool firstChoice, secondChoice;   //Battle scene Selection

	float escapePercentage;			  //Escape chances

	BATTLE_SELECTION battleSelection; //Selection chosen in battlescene

	bool monsterHitAnimation;
};

