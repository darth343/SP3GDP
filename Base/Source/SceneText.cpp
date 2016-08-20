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
, EnemyInBattle(NULL)
, IkeyPressed(false)
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
	SceneBase::Init();
	//cout << npcvec[1].GetDialogue() << endl;
	//cout << npcvec[0].GetDialogue() << endl;
	//Init GameState Here for testing purposes
	GS = TESTMAP;

	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap->LoadMap("Image//MapDesign.csv");

	// Init for loading GameObjects
	Items* thePotion = new Items(Vector3(32.f, 32.f, 1));
	thePotion->type = GameObject::GO_ITEM;
	thePotion->position.Set(150, 150, 1);
	m_goList.push_back(thePotion);

	redbar = new Gauge(Vector3(500.f, 32.f, 1));
	redbar->type = GameObject::GO_REDBAR;
	redbar->gauge = Gauge::GREENBAR;
	redbar->position.Set(150, 150, 1);

	greenbar= new Gauge(Vector3(50.f, 32.f, 1));
	greenbar->gauge = Gauge::GREENBAR;
	greenbar->type = GameObject::GO_GREENBAR;
	greenbar->position.Set(400, 150, 1);

	chargebar = new Gauge(Vector3(1.f, 32.f, 1));
	chargebar->gauge = Gauge::MOVE;
	chargebar->type = GameObject::GO_MOVE;
	chargebar->position.Set(500, 150, 1);

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

void SceneText::SetBattleStatus(bool status)
{
	battleStart = status;
}

bool SceneText::GetBattleStatus()
{
	return battleStart;
}

void SceneText::CatchUpdate(double dt)
{
	enemyCatchPercentage = (enemyMaxHealth - currHealth) / 100 * 20;

	if (Application::IsKeyPressed('V'))
	{
		currHealth -= 10;
	}

	float prevScale = greenbar->scale.x;
	greenbar->scale.x = enemyCatchPercentage * 0.1;
	if (greenbar->scale.x > prevScale)
	{
		greenbar->position.x -= (greenbar->scale.x - prevScale) * 0.5;
	}

	chargebar->Update(dt);

	if (Application::IsKeyPressed(VK_RETURN) && !ENTERkeyPressed)
	{
		ENTERkeyPressed = true;
		if (chargebar->CheckCollision(greenbar, m_cMap))
		{
			cout << "CAPTURED" << endl;
			GS = TESTMAP;

			// Despawn monster once captured
			for (int i = 0; i < m_goList.size(); ++i)
			{
				if (m_goList[i] == EnemyInBattle)
				{
					delete EnemyInBattle;
					m_goList.erase(m_goList.begin() + i);
				}
			}
		}
		else if (!chargebar->CheckCollision(greenbar, m_cMap))
		{
			//cout << "NOT CAPTURED" << endl;
			GS = BATTLE;
		}
	}
	else if (!Application::IsKeyPressed(VK_RETURN) && ENTERkeyPressed)
	{
		ENTERkeyPressed = false;
	}
	//else
	//{
	//	cout << "ENTER KEY FKED UP AND DOWN" << endl;
	//}

	//if (chargebar->CheckCollision(greenbar, m_cMap))
	//{
	//	cout << "COLLIDED" << endl;
	//	// DO COLLISION RESPONSE BETWEEN TWO GAMEOBJECTS
	//	if (Application::IsKeyPressed(VK_RETURN) && !ENTERkeyPressed)
	//	{
	//		ENTERkeyPressed = true;
	//		cout << "CORRECT" << endl;
	//		GS = TESTMAP;

	//		// Despawn monster once captured
	//		for (int i = 0; i < m_goList.size(); ++i)
	//		{
	//			if (m_goList[i] == EnemyInBattle)
	//			{
	//				delete EnemyInBattle;
	//				m_goList.erase(m_goList.begin() + i);
	//			}
	//		}
	//	}
	//	else if (!Application::IsKeyPressed(VK_RETURN) && ENTERkeyPressed)
	//	{
	//		ENTERkeyPressed = false;
	//	}
	//}
	//else
	//{
	//	cout << ENTERkeyPressed << endl;
	//	if (Application::IsKeyPressed(VK_RETURN) && !ENTERkeyPressed)
	//	{
	//		cout << "NOT COLLIDED" << endl;
	//		GS = BATTLE;
	//	}
	//	else if (!Application::IsKeyPressed(VK_RETURN) && ENTERkeyPressed)
	//	{
	//		ENTERkeyPressed = false;
	//	}
	//}

	//if (m_goList[i]->type == GameObject::GO_GREENBAR && m_goList[j]->type == GameObject::GO_MOVE && m_goList[i]->active)
	//if (m_goList[i]->CheckCollision(m_goList[j], m_cMap))
	//{
	//	cout << "COLLIDED" << endl;
	//	// DO COLLISION RESPONSE BETWEEN TWO GAMEOBJECTS
	//	if (Application::IsKeyPressed(VK_SPACE))
	//	{
	//		cout << "CORRECT!";
	//		//get monster into the inventory of monsters
	//		GS = TESTMAP;
	//	}
	//	m_goList[i]->active = false;
	//}
	//else
	//{
	//	//	cout << "NOT COLLIDED" << endl;
	//	if (Application::IsKeyPressed(VK_SPACE))
	//	{
	//		GS = BATTLE;
	//	}
	//	m_goList[i]->active = false;
	//}
}

void SceneText::BattleSceneUpdate()
{
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

		if (Application::IsKeyPressed(VK_RETURN) && !ENTERkeyPressed)
		{
			ENTERkeyPressed = true;
			if (firstChoice)
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
						// Despawn monster once run away
						for (int i = 0; i < m_goList.size(); ++i)
						{
							if (m_goList[i] == EnemyInBattle)
							{
								delete EnemyInBattle;
								m_goList.erase(m_goList.begin() + i);
							}
						}
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
			}
			else if (secondChoice)
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
				secondChoice = false;
				firstChoice = true;
			}

		}
		else if (!Application::IsKeyPressed(VK_RETURN) && ENTERkeyPressed)
		{
			ENTERkeyPressed = false;
		}

	}

	//Enemy's turn to hit back
	if (enemyTurn && !playerTurn)
	{

	}

}

void SceneText::EnterBattleScene(Enemy* enemy)
{
	EnemyInBattle = enemy;
	GS = BATTLE;
}

void SceneText::DialogueFile(string filename)
{
	
}

void SceneText::PlayerUpdate(double dt)
{
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

void SceneText::GOupdate(double dt)
{
	for (int i = 0; i < m_goList.size(); ++i)
	{
		m_goList[i]->Update(dt, theHero->GetPosition(), theHero->GetMapOffset(), m_cMap);
		if (m_goList[i]->type == GameObject::GO_NPC)
		{
			NPC* temp = (NPC*)m_goList[i];
			
			if (temp->collideWhichNPC() == npcID)
				temp->SetState(currState);

			if (temp->collideWhichNPC() != 0 && Application::IsKeyPressed(VK_RETURN) && !ENTERkeyPressed)
			{
				ENTERkeyPressed = true;
				temp->ScrollDialogue(dialogueNum);
			}
			else if (!Application::IsKeyPressed(VK_RETURN) && ENTERkeyPressed)
				ENTERkeyPressed = false;
		}
	}
}

void SceneText::UpdateInventory()
{
	if (!Application::IsKeyPressed('I') && IkeyPressed)
	{
		IkeyPressed = false;
	}

	if (Application::IsKeyPressed('I') && !IkeyPressed && GS == INVENTORY_SCREEN)
	{
		GS = TESTMAP;
		IkeyPressed = true;
	}
	else if (!Application::IsKeyPressed('I') && IkeyPressed)
	{
		IkeyPressed = false;
	}
}

void SceneText::RenderInventory()
{
	std::ostringstream ss;
	ss.str("");
	ss.precision(5);
	ss << "Items: " << endl;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 60, 300, 450);

	std::ostringstream ss2;
	ss2.str("");
	ss2.precision(5);
	ss2 << "Equips: " << endl;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(1, 0, 0), 60, 300, 390);

	std::ostringstream ss3;
	ss3.str("");
	ss3.precision(5);
	ss3 << "Tamagotchi: " << endl;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 0, 0), 60, 300, 330);
}

void SceneText::Update(double dt)
{
	switch (GS)
	{
	case TESTMAP:
		PlayerUpdate(dt);
		GOupdate(dt);
		break;
	case BATTLE:
		BattleSceneUpdate();
		break;
	case CATCH:
		CatchUpdate(dt);
		break;
	case INVENTORY_SCREEN:
		UpdateInventory();
		break;
	}

	fps = (float)(1.f / dt);
}

void SceneText::RenderPlayer()
{
	Render2DMesh(theHero->GetPlayerMesh(), false, 32.0f, theHero->GetPosition().x, theHero->GetPosition().y, theHero->GetFlipStatus());
}

static bool touched = true;
void SceneText::RenderTestMap()
{
	RenderBackground(meshList[GEO_BACKGROUND]);
	RenderTileMap(m_cMap);
	if (renderNPCstuff)
	{
		Render2DMeshWScale(meshList[GEO_NPCPIC], false, 300, 300, 100, 100, false);
		Render2DMeshWScale(meshList[GEO_BATTLEDIALOUGEBACKGROUND], false, 1, 0.3, 0, 0, false);
	}

	std::ostringstream ss;

	for (int i = 0; i < m_goList.size(); i++)
	{
		if (m_goList[i]->active == true)
		{
			if (m_goList[i]->type == GameObject::GO_ITEM)
			{
				Items* temp = (Items*)m_goList[i];
				if (temp->itemType == Items::POTION)
					Render2DMeshWScale(meshList[GEO_POTION], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false);
			}

			if (m_goList[i]->type == GameObject::GO_NPC)
			{

				NPC* temp = (NPC*)m_goList[i];
				
				ss.str("");
				ss.precision(5);
				Render2DMeshWScale(meshList[GEO_NPC], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false);

				if (temp->GetDialogueState() == temp->currState && temp->GetID() == temp->collideWhichNPC())
				{
					if (dialogueNum == temp->maxDia)
						ss << "Enter to Exit";
					if (temp->GetNum() == dialogueNum)
						ss << "Dialogue: " << temp->GetDialogue();

					if (dialogueNum >= 1 && dialogueNum <= temp->maxDia)
					{
						renderNPCstuff = true;
						RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 60, 100);
					}

					else if (dialogueNum >= temp->maxDia)
					{
						if (temp->GetID() == temp->collideWhichNPC())
						{
							npcID = temp->collideWhichNPC();
							currState = 2;
							dialogueNum = 0;
							renderNPCstuff = false;
						}
					}
					

				}
			}
			if (m_goList[i]->type == GameObject::GO_ENEMY)
			{
				Enemy* temp = (Enemy*)m_goList[i];
				Render2DMeshWScale(meshList[GEO_MONSTER], false, m_goList[i]->scale.x, m_goList[0]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, temp->GetFlipStatus());
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

void SceneText::RenderMonster()
{/*
	if (MonType.getMonsterType() == BANSHEE)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == CEREBUS)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == DRAGON)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == GOLEM)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == HYDRA)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == MANTICORE)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == OGRE)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == PEGASUS)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == WRAITH)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == SPHINX)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == SCYLLA)
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false, false);
	else if (MonType.getMonsterType() == MINOTAUR)*/
		Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, 0.3, 0.3, 300, 240, false);
	
}

void SceneText::RenderCatch()
{
	RenderBackground(meshList[GEO_BATTLESCENE]);
	RenderMonster();

	Render2DMeshWScale(meshList[GEO_RED], false, redbar->scale.x, redbar->scale.y, redbar->position.x, redbar->position.y, false);
	Render2DMeshWScale(meshList[GEO_GREEN], false, greenbar->scale.x, greenbar->scale.y, greenbar->position.x, greenbar->position.y, false);
	Render2DMeshWScale(meshList[GEO_BAR], false, chargebar->scale.x, chargebar->scale.y, chargebar->position.x, chargebar->position.y, false);
}

void SceneText::RenderBattleScene()
{	
	RenderBackground(meshList[GEO_BATTLESCENE]);

	RenderMonster();

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

			Render2DMeshWScale(meshList[GEO_BATTLEDIALOUGEBACKGROUND], false, 1, 0.3, 0, 0, false);

			Render2DMeshWScale(meshList[GEO_BATTLEARROW], false, 0.1, 0.05, arrowPosX, arrowPosY, false);

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
}

void SceneText::Render()
{
	SceneBase::Render();
	// Check for which GameState we are in
	switch (GS)
	{
	case TESTMAP:
		RenderTestMap();
		break;
	case BATTLE:
		RenderBattleScene();
		break;
	case CATCH:
		RenderCatch();
		break;
	case INVENTORY_SCREEN:
		RenderBackground(meshList[GEO_INVENTORYBACKGROUND]);
		RenderInventory();
		break;
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