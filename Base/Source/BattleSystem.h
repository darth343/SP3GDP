#pragma once
#include "Vector3.h"

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
	void SetEnemyTurn(bool set);
	void SetPlayerTurn(bool set);
	void SetArrowPos(float x, float y, float z);
	void SetFirstChoice(bool set);
	void SetSecondChoice(bool set);
	void SetEscapeChance(float percetage);
	void SetBattleSelection(BATTLE_SELECTION selection);

	bool GetBattleStart();
	bool GetEnemyTurn();
	bool GetPlayerTurn();
	float GetArrowPosX();
	float GetArrowPosY();
	bool GetFirstChoice();
	bool GetSecondChoice();
	BATTLE_SELECTION GetBattleSelection();

	void UpdateBattleSystem(static bool& UPkeyPressed, static bool& DNkeyPressed, static bool& LEFTkeyPressed, static bool& RIGHTkeyPressed, static bool& ENTERkeyPressed);

private:

	bool battleStart;				  //Detect if the battle has started

	bool enemyTurn, playerTurn;		  //Current Turn for battle scene.

	Vector3 arrowPos;				  //Arrow's placing for Battle Scene 
	
	bool firstChoice, secondChoice;   //Battle scene Selection

	float escapePercentage;			  //Escape chances

	BATTLE_SELECTION battleSelection; //Selection chosen in battlescene




	
};

