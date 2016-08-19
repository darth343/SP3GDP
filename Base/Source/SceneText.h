#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Gauge.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Map.h"
#include "Vector2.h"
#include "PlayerInfo.h"
#include "GameObject.h"
#include "NPC.h"
#include "Enemy.h"
#include "EquipmentManager.h"
#include "CharacterData.h"

class SceneText : public Scene
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

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_BLEND,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

public:
	static enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_BACKGROUND,
		GEO_GROUNDTOP,
		GEO_HEROWALK,
		GEO_MONSTER,
		GEO_RED,
		GEO_GREEN,
		GEO_NPC,
		GEO_NPCPIC,
		GEO_BATTLESCENE,
		GEO_BATTLEARROW,
		GEO_BATTLEMONSTER,
		GEO_BATTLEDIALOUGEBACKGROUND,
		GEO_INVENTORYBACKGROUND,
		GEO_BAR,
		GEO_POTION,
		GEO_GREENTILE,
		GEO_REDTILE,
		GEO_BLUETILE,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, const bool enableLight, const float size , const float x, const float y, const bool rotate = false);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderBackground();
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool rotate = false, const bool flip = false);
	void Render2DMeshWScale(Mesh *mesh, const bool enableLight, const float sizeX, const float sizeY, const float x = 0.0f, const float y = 0.0f, const bool rotate = false, const bool flip = false);
	void RenderTileMap(CMap* m_cMap, Vector3 speed = Vector3(1, 1, 1));
	void DialogueFile(string);

	//Render Functions
	void BasicRender(); // Basic Render Codes, do not touch unless needed 
	void RenderPlayer(); // RenderPlayer
	void RenderTestMap(); //RenderTestMap
	void RenderBattleScene(); //RenderBattleScene
	void RenderMonster(); // Render Monsters here
	void RenderInventory(); // Render Inventory Main Screen here

	//Update Functions
	void UselessUpdate(double dt); // Ask KY for details :>
	void PlayerUpdate(double dt); // Update to the player 
	void GOupdate(double dt); // Main GO Collisions
	void UpdateInventory(); // Updates for All Inventory 

	void EnterBattleScene(); //its like the update for BattleScene
private:
	CharacterData FontData;
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	vector<GameObject *> m_goList;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;
	int npcsize = 0;
	float rotateAngle;
	float enemyCatchPercentage;
	float enemyMaxHealth;
	float currHealth = 100;
	float xtranslate;
	float ytranslate;
	MS modelStack;
	MS viewStack;
	MS projectionStack;
	bool moveLeft=false;
	bool moveRight = true;
	Light lights[2];

	bool bLightEnabled;

	bool renderNPCstuff = false;
	float fps;
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

>>>>>>> origin/master
};

#endif