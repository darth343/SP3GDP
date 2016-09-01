#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "SceneBase.h"

#include "Gauge.h"
#include "Map.h"
#include "PlayerInfo.h"
#include "GameObject.h"
#include "NPC.h"
#include "Enemy.h"
#include "CharacterData.h"
#include "BattleSystem.h"
#include "Inventory.h"
#include "Tamagucci.h"


class SceneGame : public SceneBase
{
protected:
	enum GAMESTATE_TYPE
	{
		START_SCREEN,
		MAP,
		INVENTORY_SCREEN,

		ITEM_SCREEN,
		EQUIP_SCREEN,
		TAMAGUCCI_SCREEN,

		BATTLE,
		CATCH,
		LOSE,
		WIN,
		GS_TOTAL,
	};

	enum MAP_STATE
	{
		PLAY,
		IN_DIALOUGE,
		MS_TOTAL,
	};
public:
	SceneGame();
	~SceneGame();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset() = 0;
	void RenderTileMap(Mesh* mesh, CMap* m_cMap, Vector3 speed = Vector3(1, 1, 1));

	//Render Functions
	void RenderPlayer(); // RenderPlayer
	void RenderCatch();
	void RenderBattleScene(); //RenderBattleScene
	void RenderMonster(); // Render Monsters here
	void RenderInventory(); // Render Inventory Main Screen here
	void RenderItemScreen();
	void RenderEquipScreen();
	void RenderBattleAnimation();
	void RenderBattleHUD();
	void RenderBattleDialogue();

	//Update Functions
	void UpdateInventory(double dt); // Updates for All Inventory 
	void EnterBattleScene(Enemy* enemy); //its like the update for BattleScene
	void CatchUpdate(double dt);
	void SetGS(string gs);
	void RemoveEnemy();
	void RenderNPCDialogue(double dt);

	void ItemScreenUpdate(double dt);
	void EquipScreenUpdate(double dt);
	
	int currState = 1;
	void RenderTamagucci();
	void TamagucciUpdate(double dt);

	float GetBattleMonsterPosX();
	float GetBattleMonsterPosY();
	void SetBattleMonsterPosX(float x);
	void SetBattleMonsterPosY(float y);

	float GetBattleMonsterScaleX();
	void SetBattleMonsterScaleX(float x);
	float GetBattleMonsterScaleY();
	void SetBattleMonsterScaleY(float y);

	bool GetMonsterScaleUp();
	void SetMonsterScaleUp(bool set);
	void NPCUpdate(double dt);
	void renderTamagotchiMenu();
	void renderFirstTamagotchiFirstMenu(float yOffset = 0);
	void renderTamagotchiGame();
	void renderTamagotchiUI();
	void renderInventoryItems();
	void renderInventoryMenus();

	void ResetGame();
protected:
	Gauge* greenbar;
	Gauge* redbar;
	Gauge* chargebar;
	Enemy* EnemyInBattle;
	vector<GameObject *> m_goList;
	GameObject* touch;
	int npcPic = 0; //Which npc picture to show on the dialogue
	float enemyCatchPercentage;
	float enemyMaxHealth;
	float currHealth = 100;
	bool renderNPCstuff = false;
	int dialogueNum = 0; //Checking which dialogue to show from npc
	int npcID = 0; //Changing state of npc dialogue
	bool tamagucciFood = false;
	bool foodAnimOver = false;
	GAMESTATE_TYPE GS; // Change GameState in SceneText.cpp line 144 for testing purposes
	MAP_STATE MS;
	
	// Battle system
	BattleSystem battleScene;
	Vector3 battleMonsterPos;
	Vector3 battleMonsterScale;
	bool monsterScaleUp;

	//Battle System HP bar 
	float maxHpScale;
	Vector3 hpPos;
	float renderedHp;
	bool capturedMonster;
	bool flashEffect;
	float flashTimer;
	float maxMpScale;
	Vector3 mpPos;
	float renderedMp;
	ostringstream npctalk;
	bool captured;
};

#endif