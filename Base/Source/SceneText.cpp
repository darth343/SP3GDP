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
//, NPCInConvo(NULL)
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
	MS = PLAY;
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

	tamtam = new TAMAGUCCI;
	tamtam->position.Set(350, 250,1);
	tamtam->scale.Set(64, 64, 1);
	tamdrop = new TAMAGUCCI;
	tamdrop->position.Set(Math::RandFloatMinMax(0, 730), 600, 1);
	tamdrop->type = GameObject::GO_TAMDROP1;
	tamdrop->scale.Set(64, 64, 1);
	m_goList.push_back(tamtam);
	m_goList.push_back(tamdrop);

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
	theHero->SetPlayerMesh(meshList[GEO_HEROWALK]);
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

}

void SceneText::SetGS(string set)
{
	if (set == "START_SCREEN")
		GS = START_SCREEN;
	else if (set == "TESTMAP")
		GS = TESTMAP;
	else if (set == "INVENTORY_SCREEN")
		GS = INVENTORY_SCREEN;
	else if (set == "TAMAGUCCI_SCREEN")
		GS = TAMAGUCCI_SCREEN;
	else if (set == "BATTLE")
		GS = BATTLE;
	else if (set == "CATCH")
		GS = CATCH;
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

	chargebar->Update(dt, 200.f);
	greenbar->Update(dt, 400.f);

	if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
	{
		SharedData::GetInstance()->ENTERkeyPressed = true;
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
			GS = BATTLE;
		}
	}
	else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
	{
		SharedData::GetInstance()->ENTERkeyPressed = false;
	}

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

void SceneText::EnterBattleScene(Enemy* enemy)
{
	battleScene.SetBattleStart(true);
	EnemyInBattle = enemy;
	GS = BATTLE;
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
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPCPIC]);
	if (sa)
	{
		sa->Update(dt);
		sa->m_anim->animActive = true;
	}
	SpriteAnimation *pic2 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPCPIC2]);
	if (pic2)
	{
		pic2->Update(dt);
		pic2->m_anim->animActive = true;
	}
	SpriteAnimation *pic3 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPCPIC3]);
	if (pic3)
	{
		pic3->Update(dt);
		pic3->m_anim->animActive = true;
	}

	if (Application::IsKeyPressed('I') && !SharedData::GetInstance()->IkeyPressed && GS)
	{
		GS = INVENTORY_SCREEN;
		SharedData::GetInstance()->IkeyPressed = true;
	}
	else if (!Application::IsKeyPressed('I') && SharedData::GetInstance()->IkeyPressed)
	{
		SharedData::GetInstance()->IkeyPressed = false;
	}
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

			if (temp->collideWhichNPC() != 0 && Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
			{
				SharedData::GetInstance()->ENTERkeyPressed = true;
				temp->ScrollDialogue(dialogueNum);
			}
			else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
				SharedData::GetInstance()->ENTERkeyPressed = false;
		}
	}
}
//For the mini game
void SceneText::TamagucciUpdate(double dt)
{
	tamagucci.MiniGame(tamtam->position, dt);

	tamdrop->position.y -= 100 * dt;
	if (tamdrop->position.y <= 0)
		tamdrop->position.Set(Math::RandFloatMinMax(0, 730), 600, 0);

	if (tamtam->CheckCollision(tamdrop, m_cMap))
	{
		tamdrop->position.Set(Math::RandFloatMinMax(0, 730), 600, 0);
	}
	if (!tamtam->CheckCollision(tamdrop, m_cMap))
	{
		cout << "AHAJHJAHSJ";
	}
}
void SceneText::UpdateInventory(double dt)
{
	if (Application::IsKeyPressed('I') && !SharedData::GetInstance()->IkeyPressed)
	{
		GS = TESTMAP;
		SharedData::GetInstance()->IkeyPressed = true;
	}
	else if (!Application::IsKeyPressed('I') && SharedData::GetInstance()->IkeyPressed)
	{
		SharedData::GetInstance()->IkeyPressed = false;
	}

	cursorDebounce += (float)dt;
	if (cursorDebounce > 0.5f)
	{
		if (Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
		{
			cursorDebounce = 0;
			SharedData::GetInstance()->UPkeyPressed = false;

			if (itemCursorPos != 0)
				itemCursorPos -= 1;
			else
				itemCursorPos = 2;

			cout << "UP" << endl;
			cout << itemCursorPos << endl;
			SharedData::GetInstance()->UPkeyPressed = false;
		}
		else
			SharedData::GetInstance()->UPkeyPressed = true;

		if (Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
		{
			cursorDebounce = 0;
			SharedData::GetInstance()->DNkeyPressed = false;

			if (itemCursorPos != 2)
				itemCursorPos += 1;
			else
				itemCursorPos = 0;

			cout << "DN" << endl;
			cout << itemCursorPos << endl;
			SharedData::GetInstance()->DNkeyPressed = false;
		}
		else
			SharedData::GetInstance()->DNkeyPressed = true;
	}
	if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
		SharedData::GetInstance()->ENTERkeyPressed = true;
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

void SceneText::MapUpdate(double dt)
{
	PlayerUpdate(dt);
	GOupdate(dt);
}

void SceneText::Update(double dt)
{

	if (Application::IsKeyPressed('Z'))
	{
		GS = TAMAGUCCI_SCREEN;

	}

	switch (GS)
	{
	case TESTMAP:
		MapUpdate(dt);
		break;
	case BATTLE:
		battleScene.UpdateBattleSystem(SharedData::GetInstance()->UPkeyPressed, SharedData::GetInstance()->DNkeyPressed, SharedData::GetInstance()->LEFTkeyPressed, SharedData::GetInstance()->RIGHTkeyPressed, SharedData::GetInstance()->ENTERkeyPressed);
		break;
	case CATCH:
		CatchUpdate(dt);
		break;
	case INVENTORY_SCREEN:
		UpdateInventory(dt);
		break;
	case TAMAGUCCI_SCREEN:
		TamagucciUpdate(dt);
		tamagucci.UpdateTamagucci();
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
		if (npcPic == 1)
			Render2DMeshWScale(meshList[GEO_NPCPIC], false, 350, 350, 650, 220, false);
		if (npcPic == 2)
			Render2DMeshWScale(meshList[GEO_NPCPIC2], false, 350, 350, 650, 220, false);
		if (npcPic == 3)
			Render2DMeshWScale(meshList[GEO_NPCPIC3], false, 350, 350, 650, 220, false);
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
					npcPic = temp->collideWhichNPC();

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
							npcID = temp->collideWhichNPC();
							currState = 2;
							dialogueNum = 0;
							renderNPCstuff = false;
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
void SceneText::RenderTamagucci()
{
	RenderBackground(meshList[GEO_TAMAGUCCIBACKGROUND]);
	Render2DMeshWScale(meshList[GEO_TAMAGUCCI], false, tamtam->scale.x, tamtam->scale.y, tamtam->position.x, tamtam->position.y, false);
	for (int i = 0; i < m_goList.size(); i++)
	{
		if (m_goList[i]->active == true && m_goList[i]->type == GameObject::GO_TAMDROP1)
		{
			Render2DMeshWScale(meshList[GEO_STAR], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x, m_goList[i]->position.y, false);
		}
		if (m_goList[i]->active == true && m_goList[i]->type == GameObject::GO_TAMDROP2)
		{
			Render2DMeshWScale(meshList[GEO_TAMAGUCCI], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x, m_goList[i]->position.y, false);
		}
	}
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

		switch (battleScene.GetBattleSelection())
		{
			case BattleSystem::BATTLE_SELECTION::BS_SLASH:
			case BattleSystem::BATTLE_SELECTION::BS_ATTACK:
				battleScene.SetArrowPos(110, 98, 0);
				break;
			case BattleSystem::BATTLE_SELECTION::BS_SKILL:
			case BattleSystem::BATTLE_SELECTION::BS_CAPTURE:
				battleScene.SetArrowPos(113, 48, 0);
				break;
			case BattleSystem::BATTLE_SELECTION::BS_STAB:
			case BattleSystem::BATTLE_SELECTION::BS_ITEM:
				battleScene.SetArrowPos(405, 98, 0);
				break;
			case BattleSystem::BATTLE_SELECTION::BS_BACK:
			case BattleSystem::BATTLE_SELECTION::BS_RUN:
				battleScene.SetArrowPos(405, 48, 0);
				break;
		}

		//When it is player's turn
		if (battleScene.GetPlayerTurn() && !battleScene.GetEnemyTurn())
		{
			Render2DMeshWScale(meshList[GEO_BATTLEDIALOUGEBACKGROUND], false, 1, 0.3, 0, 0, false);

			Render2DMeshWScale(meshList[GEO_BATTLEARROW], false, 0.1, 0.05, battleScene.GetArrowPosX(), battleScene.GetArrowPosY(), false);

			std::ostringstream ss;
			ss.str("");
			ss.precision(5);
			if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice())
				ss << "Attack";
			else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice())
				ss << "Slash";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 100);

			ss.str("");
			ss.precision(5);
			if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice())
				ss << "Item";
			else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice())
				ss << "Stab";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 500, 100);

			ss.str("");
			ss.precision(5);
			if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice())
				ss << "Capture";
			else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice())
				ss << "Monster's Skill";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 50);

			ss.str("");
			ss.precision(5);
			if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice())
				ss << "Run";
			else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice())
				ss << "Back";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 500, 50);
		}
		else if (!battleScene.GetPlayerTurn() && battleScene.GetEnemyTurn())
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
	case TAMAGUCCI_SCREEN:
		tamagucci.UpdateTamagucci();
		RenderTamagucci();
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