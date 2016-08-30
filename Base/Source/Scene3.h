#ifndef SCENE_3_H
#define SCENE_3_H
#include "SceneGame.h"

#include "Camera3.h"
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

#include "SharedData.h"

class Scene3 : public SceneGame
{
public:
	Scene3();
	~Scene3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderMap();
	void RenderGO();
	//Update Functions
	void PlayerUpdate(double dt); // Update to the player 
	void GOupdate(double dt); // Main GO Collisions
	void MapUpdate(double dt);

private:
	vector<NPC*> npcvec;
	CMap* m_cMap;	// Handle to the tilemaps
	CMap* m_cMap2;	// Handle to the tilemaps
};

#endif