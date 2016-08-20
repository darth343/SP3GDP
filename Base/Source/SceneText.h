#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H
#include "SceneBase.h"

#include "Camera3.h"
#include "Gauge.h"
#include "Map.h"
#include "PlayerInfo.h"
#include "GameObject.h"
#include "NPC.h"
#include "Enemy.h"
#include "EquipmentManager.h"
#include "CharacterData.h"

class SceneText : public SceneBase
{
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

	enum GAMESTATE_TYPE
	{
		START_SCREEN,
		TESTMAP,
		INVENTORY_SCREEN,
		TAMAGOTCHI_SCREEN,
		BATTLE,
		CATCH,
		GS_TOTAL,
	};

public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void RenderTileMap(CMap* m_cMap, Vector3 speed = Vector3(1, 1, 1));
	void DialogueFile(string);

	//Render Functions
	void BasicRender(); // Basic Render Codes, do not touch unless needed 
	void RenderPlayer(); // RenderPlayer
	void RenderTestMap(); //RenderTestMap
	void RenderCatch();
	void RenderBattleScene(); //RenderBattleScene
	void RenderMonster(); // Render Monsters here
	void RenderInventory(); // Render Inventory Main Screen here

	//Update Functions
	void UselessUpdate(double dt); // Ask KY for details :>
	void PlayerUpdate(double dt); // Update to the player 
	void GOupdate(double dt); // Main GO Collisions
	void UpdateInventory(); // Updates for All Inventory 
	void EnterBattleScene(Enemy* enemy); //its like the update for BattleScene
	void BattleSceneUpdate();
	void CatchUpdate(double dt);
	void SetBattleStatus(bool status);
	bool GetBattleStatus();
private:
	Gauge* greenbar;
	Gauge* redbar;
	Gauge* chargebar;
	Enemy* EnemyInBattle;
	vector<GameObject *> m_goList;
	int npcsize = 0;
	float enemyCatchPercentage;
	float enemyMaxHealth;
	float currHealth = 100;
	bool moveLeft=false;
	bool moveRight = true;
	bool renderNPCstuff = false;
	NPC npc;

	Enemy* theEnemy;
	Pathfinder testpathfinder;
	int dialogueNum = 0;
	int npcNum = 0;
	int npcID = 0;

	// Handle to the tilemaps
	CMap* m_cMap;
	// Hero's information
	CPlayerInfo* theHero;
	// GameState
	GAMESTATE_TYPE GS; // Change GameState in SceneText.cpp line 144 for testing purposes
	// Monster Calls
	Monster MonType;

	// Equipment Functions
	EquipmentManager equipManager;

	//BattleScene Variables

	//Selection chosen in battlescene
	BATTLE_SELECTION battleSelection;

	//Battle scene Selection
	bool firstChoice, secondChoice;

	//Escape chances
	float escapePercentage;

	//BattleScene Arow position
	float arrowPosX;
	float arrowPosY;

	//Current Turn for battle scene
	bool enemyTurn, playerTurn;

	//Stating whether has the battle started or not 
	bool battleStart;

	int currState = 1;
	bool npc1;
	bool npc2;
	bool npc3;

	bool UPkeyPressed;
	bool LEFTkeyPressed;
	bool RIGHTkeyPressed;
	bool ENTERkeyPressed;
	bool IkeyPressed;
};

#endif