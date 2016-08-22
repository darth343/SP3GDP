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
#include "BattleSystem.h"
#include "Tamagucci.h"
#include "SharedData.h"

class SceneText : public SceneBase
{
	enum GAMESTATE_TYPE
	{
		START_SCREEN,
		TESTMAP,
		INVENTORY_SCREEN,
		TAMAGUCCI_SCREEN,
		BATTLE,
		CATCH,
		GS_TOTAL,
	};

	enum MAP_STATE
	{
		PLAY,
		IN_DIALOUGE,
		MS_TOTAL,
	};

public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void RenderTileMap(CMap* m_cMap, Vector3 speed = Vector3(1, 1, 1));

	//Render Functions
	void BasicRender(); // Basic Render Codes, do not touch unless needed 
	void RenderPlayer(); // RenderPlayer
	void RenderTestMap(); //RenderTestMap
	void RenderCatch();
	void RenderBattleScene(); //RenderBattleScene
	void RenderMonster(); // Render Monsters here
	void RenderInventory(); // Render Inventory Main Screen here

	//Update Functions
	void MapUpdate(double dt);
	void PlayerUpdate(double dt); // Update to the player 
	void GOupdate(double dt); // Main GO Collisions
	void UpdateInventory(double dt); // Updates for All Inventory 
	void EnterBattleScene(Enemy* enemy); //its like the update for BattleScene
	void CatchUpdate(double dt);
	void SetGS(string gs);
	void RemoveEnemy();
	void RenderNPCDialogue(double dt);
	void RenderTamagucci();
	void TamagucciUpdate(double dt);
private:
	NPC npc;
	Gauge* greenbar;
	Gauge* redbar;
	Gauge* chargebar;
	Enemy* theEnemy;
	Enemy* EnemyInBattle;
	TAMAGUCCI* tamtam;
	TAMAGUCCI* tamdrop;
	Pathfinder testpathfinder;
	TAMAGUCCI tamagucci;
	vector<GameObject *> m_goList;
	int npcPic = 0; //Which npc picture to show on the dialogue
	float enemyCatchPercentage;
	float enemyMaxHealth;
	float currHealth = 100;
	bool renderNPCstuff = false;
	int dialogueNum = 0; //Checking which dialogue to show from npc
	int npcID = 0; //Changing state of npc dialogue

	CMap* m_cMap;	// Handle to the tilemaps
	CPlayerInfo* theHero; // Hero's information
	GAMESTATE_TYPE GS; // Change GameState in SceneText.cpp line 144 for testing purposes
	MAP_STATE MS;
	Monster MonType; // Monster Calls

	// Equipment Functions
	EquipmentManager equipManager;
	
	// Battle system
	BattleSystem battleScene;

	int currState = 1; //The current state of npc dialogue

	int itemCursorPos = 0;
	float cursorDebounce = 0;
};

#endif