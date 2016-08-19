#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SpriteAnimation.h"
#include "Enemy.h"
#include "Items.h"
SceneText::SceneText()
	:
	m_cMap(NULL)
{
}

SceneText::~SceneText()
{
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
}

void SceneText::Init()
{
	//Hell ARUN, KY and WM
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
//	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
//	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//cout << npcvec[1].GetDialogue() << endl;
	//cout << npcvec[0].GetDialogue() << endl;
	//Init GameState Here for testing purposes
	GS = TESTMAP;

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	FontData.Init("Image//FontData.csv");
	// Load the ground mesh and texture
	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background.tga");
	meshList[GEO_GROUNDTOP] = MeshBuilder::Generate2DMesh("GEO_GROUNDTOP", Color(1, 1, 1), 0.0f, 0.0f, 32.0f, 32.0f);
	meshList[GEO_GROUNDTOP]->textureID = LoadTGA("Image//groundTop.tga");

	meshList[GEO_GREENTILE] = MeshBuilder::Generate2DMesh("GEO_GREENTILE", Color(1, 1, 1), 0.0f, 0.0f, 32.0f, 32.0f);
	meshList[GEO_GREENTILE]->textureID = LoadTGA("Image//greenTile.tga");

	meshList[GEO_NPC] = MeshBuilder::Generate2DMesh("GEO_GREENTILE", Color(1, 1, 1), 0.0f, 0.0f, 1.f, 1.0f);
	meshList[GEO_NPC]->textureID = LoadTGA("Image//NPC1.tga");

	meshList[GEO_REDTILE] = MeshBuilder::Generate2DMesh("GEO_REDTILE", Color(1, 1, 1), 0.0f, 0.0f, 32.0f, 32.0f);
	meshList[GEO_REDTILE]->textureID = LoadTGA("Image//redTile.tga");

	meshList[GEO_BLUETILE] = MeshBuilder::Generate2DMesh("GEO_BLUETILE", Color(1, 1, 1), 0.0f, 0.0f, 32.0f, 32.0f);
	meshList[GEO_BLUETILE]->textureID = LoadTGA("Image//blueTile.tga");

	Math::InitRNG();
	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap->LoadMap("Image//MapDesign.csv");

	// Init for loading GameObjects
	Items* thePotion = new Items(Vector3(32.f, 32.f, 1));
	thePotion->type = GameObject::GO_ITEM;
	thePotion->position.Set(150, 150, 1);
	m_goList.push_back(thePotion);

	Gauge* theChargeRed = new Gauge(Vector3(500.f, 32.f, 1));
	theChargeRed->type = GameObject::GO_REDBAR;
	theChargeRed->gauge = Gauge::GREENBAR;
	theChargeRed->position.Set(150, 150, 1);
	m_goList.push_back(theChargeRed);

	Gauge* theChargeGreen = new Gauge(Vector3(50.f, 32.f, 1));
	theChargeGreen->gauge = Gauge::GREENBAR;
	theChargeGreen->type = GameObject::GO_GREENBAR;
	theChargeGreen->position.Set(400, 150, 1);
	m_goList.push_back(theChargeGreen);

	Gauge* theChargeBar = new Gauge(Vector3(1.f, 32.f, 1));
	theChargeBar->gauge = Gauge::MOVE;
	theChargeBar->type = GameObject::GO_MOVE;
	theChargeBar->position.Set(500, 150, 1);
	m_goList.push_back(theChargeBar);

	theEnemy = new Enemy(Vector3(32.f, 32.f, 1));
	theEnemy->type = GameObject::GO_ENEMY;
	theEnemy->position.Set(64, 224, 1);
	m_goList.push_back(theEnemy);
	enemyMaxHealth= currHealth = 100;
	enemyCatchPercentage = 0;
	npc.ReadFromFile("Image//Text.txt",m_goList);
	vector<NPC*>npcvec = npc.GetVec();

	for (int i = 0; i < npc.GetVec().size(); i++)
	{
		if (npcvec[i]->GetID() == 1)
			npcvec[i]->position.Set(500, 400, 1);
		if (npcvec[i]->GetID() == 2)
			npcvec[i]->position.Set(700, 400, 1);
		if (npcvec[i]->GetID() == 3)
			npcvec[i]->position.Set(600, 400, 1);
		npcvec[i]->currDia = 1;

		m_goList.push_back(dynamic_cast<NPC*>(npcvec[i]));
	}

	// Initialise and load the REAR tile map
	//m_cRearMap = new CMap();
	//m_cRearMap->Init( 600, 800, 24, 32, 600, 1600 );
	//m_cRearMap->LoadMap( "Image//MapDesign_Rear.csv" );

	// Initialise the hero's position
	theHero = new CPlayerInfo();
	theHero->SetPosition(Vector3(530, 64, 0));

	meshList[GEO_HEROWALK] = MeshBuilder::Generate2DMesh("Player", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_HEROWALK]->textureID = LoadTGA("Image//Hero.tga");

	meshList[GEO_MONSTER] = MeshBuilder::Generate2DMesh("Monster", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_MONSTER]->textureID = LoadTGA("Image//Monster.tga");

	meshList[GEO_POTION] = MeshBuilder::Generate2DMesh("Potion", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_POTION]->textureID = LoadTGA("Image//Potion.tga");

	meshList[GEO_GREEN] = MeshBuilder::Generate2DMesh("Potion", Color(0, 1, 0), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_BAR] = MeshBuilder::Generate2DMesh("Potion", Color(1, 1, 0), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_RED] = MeshBuilder::Generate2DMesh("Potion", Color(1, 0, 0), 0.0f, 0.0f, 1.0f, 1.0f);

	meshList[GEO_BATTLESCENE] = MeshBuilder::Generate2DMesh("GEO_BATTLESCENE", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BATTLESCENE]->textureID = LoadTGA("Image//battleScene.tga");
	meshList[GEO_BATTLEMONSTER] = MeshBuilder::Generate2DMesh("GEO_BATTLESCENE", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BATTLEMONSTER]->textureID = LoadTGA("Image//battleMonster.tga");

	meshList[GEO_BATTLEDIALOUGEBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BATTLEDIALOUGEBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BATTLEDIALOUGEBACKGROUND]->textureID = LoadTGA("Image//dialougeBG.tga");

	meshList[GEO_BATTLEARROW] = MeshBuilder::Generate2DMesh("GEO_BATTLEARROW", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BATTLEARROW]->textureID = LoadTGA("Image//arrow.tga");

	theHero->SetPlayerMesh(meshList[GEO_HEROWALK]);
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;
	bLightEnabled = true;

	//BattleScene Variables
	enemyTurn = false;
	playerTurn = true;

	firstChoice = true;
	secondChoice = false;
	battleStart = false;

	//Aarow position for Battle Scene
	arrowPosX = 125;
	arrowPosY = 92.5;

	battleSelection = BS_ATTACK;
}

//BattleScene Key press 
static bool DNkeyPressed = false;
static bool UPkeyPressed = false;
static bool LEFTkeyPressed = false;
static bool RIGHTkeyPressed = false;
static bool ENTERkeyPressed = false;

void SceneText::EnterBattleScene()
{
	GS = BATTLE;
	/*cout << "Start battleScene" << endl;*/

	if (playerTurn && !enemyTurn)
	{
		if (Application::IsKeyPressed(VK_UP))
		{
			if (UPkeyPressed)
			{
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 2);

				if (firstChoice && battleSelection < BS_ATTACK)
					battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 4);
				else if (secondChoice && battleSelection < BS_SLASH)
					battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 4);

				cout << "BS = " << battleSelection << endl;
				UPkeyPressed = false;
			}
		}
		else
			UPkeyPressed = true;

		if (Application::IsKeyPressed(VK_DOWN))
		{
			if (DNkeyPressed)
			{
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 2);

				if (firstChoice && battleSelection > BS_RUN)
					battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 4);
				else if (secondChoice && battleSelection > BS_BACK)
					battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 4);

				cout << "BS = " << battleSelection << endl;
				DNkeyPressed = false;
			}
		}
		else
			DNkeyPressed = true;

		if (Application::IsKeyPressed(VK_LEFT))
		{
			if (LEFTkeyPressed)
			{
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 1);

				if (firstChoice && battleSelection < BS_ATTACK)
					battleSelection = BS_RUN;
				else if (secondChoice && battleSelection < BS_SLASH)
					battleSelection = BS_BACK;

				cout << "BS = " << battleSelection << endl;
				LEFTkeyPressed = false;
			}
		}
		else
			LEFTkeyPressed = true;

		if (Application::IsKeyPressed(VK_RIGHT))
		{
			if (RIGHTkeyPressed)
			{
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 1);

				if (firstChoice && battleSelection > BS_RUN)
					battleSelection = BS_ATTACK;
				else if (secondChoice && battleSelection > BS_BACK)
					battleSelection = BS_SLASH;

				cout << "BS = " << battleSelection << endl;
				RIGHTkeyPressed = false;
			}
		}
		else
			RIGHTkeyPressed = true;

		if (Application::IsKeyPressed(VK_RETURN))
		{
			if (firstChoice && ENTERkeyPressed)
			{
				switch (battleSelection)
				{
				case BS_ATTACK:
					//Render Attack skills choices (Slash, Stab, (Monster's Skills), Back)
					battleSelection = BS_SLASH;
					firstChoice = false;
					secondChoice = true;
					cout << "ATTACK! " << endl;
					secondChoice = true;
					firstChoice = false;
					break;

				case BS_ITEM:
					//Render Item bag design
					//Battle selection set to Item's
					firstChoice = false;
					secondChoice = true;
					cout << "Item Bag" << battleSelection << endl;
					secondChoice = true;
					firstChoice = false;
					break;

				case BS_CAPTURE:
					//Start Capture function && render capture function
					GS = CATCH;
					battleStart = false;
					break;

				case BS_RUN:
					cout << "RUN AWAY" << battleSelection << endl;
					escapePercentage += Math::RandFloatMinMax(0.0f, 50.0f);
					cout << "Escape % = " << escapePercentage << endl;
					if (escapePercentage > 50.0f)
					{
						playerTurn = true;
						enemyTurn = false;
						escapePercentage = 25.0f;
						cout << "ESCAPE LOHHHHHHHHHH!" << endl;
						battleSelection = BS_ATTACK;
						GS = TESTMAP;
						//go back to exploring
					}
					else
					{
						/*enemyTurn = true;
						playerTurn = false;*/
						escapePercentage = 25.0f;
						cout << "WHY YOU NO ESCAPE!!!!" << endl;
						battleSelection = BS_ATTACK;
					}
					break;
				}

				ENTERkeyPressed = false;
			}
			else if (secondChoice && ENTERkeyPressed)
			{
				//Second Choice only applys to Attack and Item as it need to display a new numbers of choices
				cout << "Second Choice Selection" << endl;
				switch (battleSelection)
				{
				case BS_SLASH:
					//minus enemy hp, then enemy turn = true, player turn = false
					cout << "Slash enemy " << endl;

					//if enemy not dead
					enemyTurn = true;
					playerTurn = false;
					break;
				case BS_STAB:
					//minus enemy hp, then enemy turn = true, player turn = false
					cout << "Stab enemy " << battleSelection << endl;

					//if enemy not dead
					enemyTurn = true;
					playerTurn = false;
					break;
				case BS_SKILL:
					//minus enemy hp, then enemy turn = true, player turn = false
					cout << " Monster's skills " << battleSelection << endl;

					//if enemy not dead
					enemyTurn = true;
					playerTurn = false;
					break;
				case BS_BACK:
					cout << " Back " << battleSelection << endl;

					firstChoice = true;
					secondChoice = false;
					battleSelection = BS_ATTACK;
					//Render back 1st page choices (Attack, Item bag, Capture, Run)
					break;

				}

				ENTERkeyPressed = false;
				secondChoice = false;
				firstChoice = true;
			}

		}
		else
			ENTERkeyPressed = true;

	}

	//Enemy's turn to hit back
	if (enemyTurn && !playerTurn)
	{

	}

}

void SceneText::DialogueFile(string filename)
{
	
}
void SceneText::UselessUpdate(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	rotateAngle -= Application::camera_yaw;// += (float)(10 * dt);

	camera.Update(dt);
}

void SceneText::PlayerUpdate(double dt)
{
	enemyCatchPercentage = (enemyMaxHealth - currHealth) / 100 * 20;

	if (Application::IsKeyPressed('V'))
	{
		currHealth -= 10;
	}
	
	//cout << enemyCatchPercentage << endl;

	// Update the hero
	if (Application::IsKeyPressed('W'))
		this->theHero->MoveUpDown(false, m_cMap, dt);
	if (Application::IsKeyPressed('S'))
		this->theHero->MoveUpDown(true, m_cMap, dt);
	if (Application::IsKeyPressed('A'))
		this->theHero->MoveLeftRight(true, m_cMap, dt);
	if (Application::IsKeyPressed('D'))
		this->theHero->MoveLeftRight(false, m_cMap, dt);
	theHero->HeroUpdate(m_cMap, dt, meshList);

	//For Testing Purpose
	if (Application::IsKeyPressed('G'))
		battleStart = true;
}
static bool enterpressed = false;
void SceneText::GOupdate(double dt)
{
	for (int i = 0; i < m_goList.size(); ++i)
	{
		m_goList[i]->Update(dt, theHero->GetPosition(), theHero->GetMapOffset(), m_cMap);
		if (m_goList[i]->type == GameObject::GO_GREENBAR)
		{
			float prevScale = m_goList[i]->scale.x;
			m_goList[i]->scale.x = enemyCatchPercentage * 0.1;
			if (m_goList[i]->scale.x > prevScale)
			{
				m_goList[i]->position.x -= (m_goList[i]->scale.x - prevScale) * 0.5;
			}
		}
		if (m_goList[i]->type == GameObject::GO_NPC)
		{
			NPC* temp = (NPC*)m_goList[i];
			
			if (temp->collideWhichNPC() == npcID)
				temp->SetState(currState);

			if (temp->collideWhichNPC() != 0 && Application::IsKeyPressed(VK_RETURN) && !enterpressed)
			{
				enterpressed = true;
				temp->ScrollDialogue(dialogueNum);
			}
			else if (!Application::IsKeyPressed(VK_RETURN) && enterpressed)
				enterpressed = false;
		}
		for (int j = i + 1; j < m_goList.size(); ++j)
		{
			if (m_goList[i]->type == GameObject::GO_GREENBAR && m_goList[j]->type == GameObject::GO_MOVE && m_goList[i]->active)
			if (m_goList[i]->CheckCollision(m_goList[j], m_cMap))
			{
				cout << "COLLIDED" << endl;
				// DO COLLISION RESPONSE BETWEEN TWO GAMEOBJECTS
				if (Application::IsKeyPressed(VK_SPACE))
				{
					cout << "CORRECT!";
					//get monster into the inventory of monsters
					GS = TESTMAP;
				}
				m_goList[i]->active = false;
			}
			else
			{
			//	cout << "NOT COLLIDED" << endl;
				if (Application::IsKeyPressed(VK_SPACE))
				{
					GS = BATTLE;
				}
				m_goList[i]->active = false;
			}
		}
	}
}

void SceneText::Update(double dt)
{
	// Uncomment this if you want to access lights and stuff
	// UselessUpdate(double dt);
	PlayerUpdate(dt);
	GOupdate(dt);
	fps = (float)(1.f / dt);

	if (battleStart)
		EnterBattleScene();
	
}

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				float pointer = 0.2f;
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					pointer += FontData.charOffsets[text[Math::Min(i, i - 1)]] + 0.18f;
					characterSpacing.SetToTranslation(pointer, 0.3f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

					mesh->Render((unsigned)text[i] * 6, 6);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneText::RenderMeshIn2D(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				if (rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void SceneText::Render2DMeshWScale(Mesh *mesh, const bool enableLight, const float sizeX, const float sizeY, const float x, const float y, const bool rotate, const bool flip)
{
	glDisable(GL_CULL_FACE);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	if (flip)
	{
		modelStack.Translate(x+32, y, 0);
	}
	else
	{
		modelStack.Translate(x, y, 0);
	}
	modelStack.Scale(sizeX, sizeY, 1);
	if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);

	if (flip)
		modelStack.Rotate(180, 0, 1, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_CULL_FACE);
}

void SceneText::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				if (rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);

				if (flip)
					modelStack.Rotate(180, 0, 1, 0);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneText::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneText::RenderPlayer()
{
	Render2DMesh(theHero->GetPlayerMesh(), false, 32.0f, theHero->GetPosition().x, theHero->GetPosition().y, false, theHero->GetFlipStatus());
}

void SceneText::RenderBackground()
{
	// Render the crosshair
	Render2DMesh(meshList[GEO_BACKGROUND], false, 1.0f);
}

void SceneText::BasicRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}
static bool touched = true;
void SceneText::RenderTestMap()
{
	RenderBackground();
	RenderTileMap(m_cMap);
	std::ostringstream ss;

	for (int i = 0; i < m_goList.size(); i++)
	{
		if (m_goList[i]->active == true)
		{
			if (m_goList[i]->type == GameObject::GO_ITEM)
			{
				Items* temp = (Items*)m_goList[i];
				if (temp->itemType == Items::POTION)
					Render2DMeshWScale(meshList[GEO_POTION], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false, false);
			}
			if (m_goList[i]->type == GameObject::GO_NPC)
			{
				NPC* temp = (NPC*)m_goList[i];
				ss.str("");
				ss.precision(5);
				if (temp->GetDialogueState() == temp->currState && temp->GetID() == temp->collideWhichNPC())
				{
					if (temp->GetNum() == dialogueNum)
						ss << "Dialogue: " << temp->GetDialogue();

					if (dialogueNum == temp->maxDia)
						ss << "Enter to Exit";

					else if (dialogueNum >= temp->maxDia)
					{
						if (temp->GetID() == temp->collideWhichNPC())
						{
							npcID = temp->collideWhichNPC();
							currState = 2;
							dialogueNum = 0;
						}
					}
				}
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 0, 100);

				//if (temp->GetNum() == 0)
				Render2DMeshWScale(meshList[GEO_NPC], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false, false);
			}
			if (m_goList[i]->type == GameObject::GO_ENEMY)
			{
				Enemy* temp = (Enemy*)m_goList[i];
				Render2DMeshWScale(meshList[GEO_MONSTER], false, m_goList[i]->scale.x, m_goList[0]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false, temp->GetFlipStatus());
			}
		}
	}

	RenderPlayer();

	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 30, 0, 0);
}

void SceneText::RenderBattleScene()
{
	//RenderBackground of battle scene
	Render2DMesh(meshList[GEO_BATTLESCENE], false, 1.0f);

	Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false,0.3,0.3,300,240, false, false);

	if (GS == BATTLE)
	{

		switch (battleSelection)
		{
			case BS_SLASH:
			case BS_ATTACK:
				arrowPosX = 125;
				arrowPosY = 92.5;
				break;
			case BS_SKILL:
			case BS_CAPTURE:
				arrowPosX = 125;
				arrowPosY = 42.5;
				break;
			case BS_STAB:
			case BS_ITEM:
				arrowPosX = 420;
				arrowPosY = 92.5;
				break;
			case BS_BACK:
			case BS_RUN:
				arrowPosX = 420;
				arrowPosY = 42.5;
				break;
		}

		//When it is player's turn
		if (playerTurn && !enemyTurn)
		{

			Render2DMeshWScale(meshList[GEO_BATTLEDIALOUGEBACKGROUND], false, 1, 0.3, 0, 0, false, false);

			Render2DMeshWScale(meshList[GEO_BATTLEARROW], false, 0.1, 0.05, arrowPosX, arrowPosY, false, false);

			std::ostringstream ss;
			ss.str("");
			ss.precision(5);
			if (firstChoice && !secondChoice)
				ss << "Attack";
			else if (secondChoice && !firstChoice)
				ss << "Slash";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 100);

			ss.str("");
			ss.precision(5);
			if (firstChoice && !secondChoice)
				ss << "Item";
			else if (secondChoice && !firstChoice)
				ss << "Stab";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 500, 100);

			ss.str("");
			ss.precision(5);
			if (firstChoice && !secondChoice)
				ss << "Capture";
			else if (secondChoice && !firstChoice)
				ss << "Monster's Skill";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 50);

			ss.str("");
			ss.precision(5);
			if (firstChoice && !secondChoice)
				ss << "Run";
			else if (secondChoice && !firstChoice)
				ss << "Back";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 500, 50);
		}
		else if (!playerTurn && enemyTurn)
		{
		}

	}
	for (int i = 0; i < m_goList.size(); i++)
	{
		if (GS == CATCH)
		{
				if (m_goList[i]->type == GameObject::GO_REDBAR)
				{
					m_goList[i]->active = true;
					Render2DMeshWScale(meshList[GEO_RED], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false, false);
				}
				if (m_goList[i]->type == GameObject::GO_GREENBAR)
				{
					m_goList[i]->active = true;

					Render2DMeshWScale(meshList[GEO_GREEN], false,m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false, false);
				}
				if (m_goList[i]->type == GameObject::GO_MOVE)
				{
					m_goList[i]->active = true;
					Render2DMeshWScale(meshList[GEO_BAR], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false, false);
				}
			}
	}
}

void SceneText::Render()
{
	BasicRender(); // Basic Render stuff, please don't comment this out, like seriously

	// Check for which GameState we are in
	if (GS == TESTMAP)
	{
		RenderTestMap();
	}

	if (GS == BATTLE)
	{
		//RenderBattleScene....
		RenderBattleScene();
	}
	if (GS == CATCH)
	{
		RenderBattleScene();
	}

}

void SceneText::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void SceneText::RenderTileMap(CMap* map, Vector3 speed)
{
	for (int y = 0; y < map->theNumOfTiles_Height; ++y)
	{
		for (int x = 0; x < map->theNumOfTiles_Width; ++x)
		{
			if (map->theMap[y][x].BlockID == 1)
			{
				Render2DMesh(meshList[GEO_GROUNDTOP], false, 1.0f, x*map->GetTileSize() - (theHero->GetMapOffset().x * speed.x), y*map->GetTileSize() - (theHero->GetMapOffset().y* speed.y));
			}
			else if (map->theMap[y][x].BlockID == 2)
				Render2DMesh(meshList[GEO_HEROWALK], false, 32.0f, x*map->GetTileSize() - (theHero->GetMapOffset().x * speed.x), y*map->GetTileSize() - (theHero->GetMapOffset().y* speed.y));


		}
	}
}