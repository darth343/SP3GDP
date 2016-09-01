#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "Inventory.h"
#include "Monster.h"
#include "Enemy.h"
#include "PlayerInfo.h"
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
		GAME_BOSS,
		MAX,
	};
	static SharedData* GetInstance()
	{
		static SharedData data;
		return &data;
	}
	GAMESTATE gameState;
	
	Monster monType; // Monster Calls
	Inventory inventory;
	SoundManager soundManager;
	ISound* soundFootstep;
	ISound* worldBGM;
	ISound* battleStart;
	ISound* title;
	TAMAGUCCI tamagucci;
	ISound* slash;
	ISound* stab;
	ISound* skill;
	ISound* click;
	std::vector<Enemy*> enemyInventory;
	bool ZKeyPressed;
	bool UPkeyPressed;
	bool DNkeyPressed;
	bool LEFTkeyPressed;
	bool RIGHTkeyPressed;
	bool ENTERkeyPressed;
	bool BACKkeyPressed;
	bool IkeyPressed;
	bool SpaceKeyPressed;
	bool QKeyPressed;

	bool BS_SlashRender;
	bool BS_StabRender;
	bool BS_ScreamRender;
	bool BS_RoarRender;
	bool BS_BiteRender;
	bool BS_SkinRender;
	bool enemyHitPlayer;
	bool playerHitenemy;
	bool enemyTurn;
	bool playerTurn;
	bool playerBattleDialogue;

	float menuCoolDown;

	int hungerLevel;
	int energyLevel;
	int happinessLevel;

	int playerLives;
	CPlayerInfo* player;

	float trapPercentageIncrease;
	float enemyCatchPercentage;


	bool capturedBanshee;
	bool capturedGolem;
	bool capturedDragon;
	bool capturedCerebus;

	bool soundPlay;

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
		SpaceKeyPressed = false;
		QKeyPressed = false;
		soundPlay = false;

		soundFootstep = NULL;
		worldBGM = NULL;
		battleStart = NULL;
		title = NULL;
		slash = NULL;
		stab = NULL;
		skill = NULL;
		click = NULL;

		BS_StabRender = false;
		BS_SlashRender = false;
		enemyTurn = false;
		playerTurn = true;
		enemyHitPlayer = false;
		playerHitenemy = false;
		BS_ScreamRender = false;
		BS_RoarRender = false;
		BS_BiteRender = false;
		BS_SkinRender = false;
		player = new CPlayerInfo();
		player->Init();
		player->SetPosition(Vector3(530, 64, 0));
		playerBattleDialogue = false;
		playerLives = 3;
		trapPercentageIncrease = 0.0f;
		enemyCatchPercentage = 0.0f;
		menuCoolDown = 0.f;


		capturedBanshee = false;
		capturedGolem = false;
		capturedDragon = false;
		capturedCerebus = false;

	}
	~SharedData()
	{
		if (soundFootstep)
		soundFootstep->drop();
		if(worldBGM)
		worldBGM->drop();
		if (battleStart)
		battleStart->drop();
		if (title)
		title->drop();
		if (slash)
		slash->drop();
		if (stab)
		stab->drop();
		if (skill)
		skill->drop();
		if (click)
		click->drop();
		if (player)
			delete player;
		Monster::Exit();
	}
};

#endif // !SHARED_DATA_H
