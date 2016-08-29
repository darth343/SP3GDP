#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "Inventory.h"
#include "Monster.h"
#include "Enemy.h"
#include <vector>
#include "SoundManager.h"
#include "Tamagucci.h"

class SharedData
{
public:
	enum GAMESTATE
	{
		MENU,
		STORY,
		GAME_S1,
		GAME_S2,
		GAME_S3,
		GAME_S4,
		GAME_SECRET,
		GAME_BOSS,
		MAX,
	};
	static SharedData* GetInstance()
	{
		static SharedData data;
		return &data;
	}
	GAMESTATE gameState;
	bool stateCheck = false;
	
	Monster monType; // Monster Calls
	Inventory inventory;
	SoundManager soundManager;
	ISound* soundFootstep;
	ISound* worldBGM;
	ISound* battleStart;
	ISound* title;
	TAMAGUCCI tamagucci;
	std::vector<Enemy*> enemyInventory;
	bool ZKeyPressed;
	bool UPkeyPressed;
	bool DNkeyPressed;
	bool LEFTkeyPressed;
	bool RIGHTkeyPressed;
	bool ENTERkeyPressed;
	bool BACKkeyPressed;
	bool IkeyPressed;

	bool BS_SlashRender;
	bool BS_StabRender;
	bool enemyHitPlayer;
	bool enemyTurn;
	bool playerTurn;

	int hungerLevel;
	int energyLevel;
	int happinessLevel;

private:
	SharedData()
	{
		//Instantiate
		gameState = MENU;
		ZKeyPressed = false;
		UPkeyPressed = false;
		DNkeyPressed = false;
		LEFTkeyPressed = false;
		RIGHTkeyPressed = false;
		ENTERkeyPressed = false;
		BACKkeyPressed = false;
		IkeyPressed = false;


		soundFootstep = NULL;
		worldBGM = NULL;
		battleStart = NULL;
		title = NULL;

		BS_StabRender = false;
		BS_SlashRender = false;
		enemyTurn = false;
		playerTurn = true;
		enemyHitPlayer = false;

	}
	~SharedData()
	{

	}
};

#endif // !SHARED_DATA_H
