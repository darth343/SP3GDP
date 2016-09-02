#include "Scene2.h"
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
#include "SharedData.h"

Scene2::Scene2()
: m_cMap(NULL)
, m_cMap2(NULL)
{
}

Scene2::~Scene2()
{
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
	if (m_cMap2)
	{
		delete m_cMap2;
		m_cMap2 = NULL;
	}
}

void Scene2::Reset()
{
	for (int i = 0; i < m_goList.size(); ++i)
	{
		if (m_goList[i]->type == GameObject::GO_ENEMY)
		{
			Enemy* temp = (Enemy*)m_goList[i];
			temp->Reset();
		}
		m_goList[i]->active = true;
	}
}

void Scene2::Init()
{
	SceneGame::Init();
	//Init GameState Here for testing purposes
	GS = MAP;
	MS = PLAY;
	capturedMonster = false;
	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap->LoadMap("Data//ArunMapData.csv");

	m_cMap2 = new CMap();
	m_cMap2->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap2->LoadMap("Data//ArunMapData2.csv");

	// Init for loading GameObjects
	Items* thePotion = new Items(Vector3(32.f, 32.f, 1));
	thePotion->type = GameObject::GO_ITEM;
	thePotion->position.Set(150, 150, 1);
	m_goList.push_back(thePotion);

	redbar = new Gauge(Vector3(500.f, 32.f, 1));
	redbar->type = GameObject::GO_REDBAR;
	redbar->gauge = Gauge::GREENBAR;
	redbar->position.Set(150, 150, 1);

	greenbar = new Gauge(Vector3(0.f, 32.f, 1));
	greenbar->gauge = Gauge::GREENBAR;
	greenbar->type = GameObject::GO_GREENBAR;
	greenbar->position.Set(400, 150, 1);

	chargebar = new Gauge(Vector3(1.f, 32.f, 1));
	chargebar->gauge = Gauge::MOVE;
	chargebar->type = GameObject::GO_MOVE;
	chargebar->position.Set(500, 150, 1);

	for (int i = 0; i < 4; ++i)
	{
		Enemy* theEnemy;
		theEnemy = new Enemy(Monster::getMonster(Monster::GOLEM), Vector3(50.f, 50.f, 1));
		SharedData::GetInstance()->monType.SetType(Monster::GOLEM);
		theEnemy->type = GameObject::GO_ENEMY;
		Vector3 RandPos;
		while (RandPos.IsZero())
		{
			RandPos.Set(Math::RandIntMinMax(0, m_cMap->GetNumOfTiles_Width() - 1), Math::RandIntMinMax(0, m_cMap->GetNumOfTiles_Height() - 1), 0);
			if (m_cMap->theMap[RandPos.y][RandPos.x].shouldCollide)
				RandPos.SetZero();
		}
		theEnemy->position.Set(RandPos.x * 32, RandPos.y * 32, 1);
		m_goList.push_back(theEnemy);
	}

	enemyMaxHealth = 100;
	currHealth = 100;
	enemyCatchPercentage = 0;
	NPC::ReadFromFile("NPC//2.txt", npcvec);

	for (int i = 0; i < npcvec.size(); i++)
	{
		if (npcvec[i]->GetID() == 1)
			npcvec[i]->position.Set(500, 100, 1);
		if (npcvec[i]->GetID() == 2)
			npcvec[i]->position.Set(700, 300, 1);
		if (npcvec[i]->GetID() == 3)
			npcvec[i]->position.Set(100, 360, 1);
		npcvec[i]->currDia = 1;

		m_goList.push_back(dynamic_cast<NPC*>(npcvec[i]));
	}

	GameObject* touch = new GameObject(Vector3(50.f, 50.f, 1));
	touch->position.Set(1500, 80, 1);
	touch->type = GameObject::GO_NEXT;
	m_goList.push_back(touch);

	// Initialise the hero's position
	SharedData::GetInstance()->player->SetPosition(Vector3(1300, 60, 0));
	SharedData::GetInstance()->player->SetPlayerMesh(meshList[GEO_HEROLR]);
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	//Mtx44 perspective;
	//perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	////perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	//projectionStack.LoadMatrix(perspective);

	battleMonsterPos.Set(280, 240, 0);
	battleMonsterScale.Set(300, 300, 1);
	monsterScaleUp = true;

	SharedData::GetInstance()->soundManager.Init();

	//Battle HUD
	maxHpScale = 17.4f;
	hpPos.Set(36, 574, 0);
	mpPos.Set(36, 548.9, 0);
	maxMpScale = 10.9f;

	//new
	flashEffect = false;
	flashTimer = 0.0f;
	renderedMp = 100;
	renderedHp = 100;

	for (int j = 0; j < 0; ++j)
	{
		Equipment* temp = new Equipment();
		//Equipment::EQUIPMENT_TYPE randType = (Equipment::EQUIPMENT_TYPE)Math::RandIntMinMax(Equipment::SWORD, Equipment::TOTAL_ETYPE -1);
		Monster::MONSTER_TYPE randmonstertype = (Monster::MONSTER_TYPE)Math::RandIntMinMax(Monster::BANSHEE, Monster::GOLEM);
		Equipment::EQUIPMENT_TYPE randType = Equipment::ARMOUR;
		stringstream ss;
		ss << Monster::getMonster(randmonstertype).getName() << " " << randType;
		temp->SetName(ss.str());
		temp->SetMonster(Monster::getMonster(randmonstertype));
		temp->setType(randType);
		temp->setDamage(Equipment::getDatabase()[randType][0]->getDamage() + Math::RandIntMinMax(-10, 10));
		temp->setDefense(Equipment::getDatabase()[randType][0]->getDefense() + Math::RandIntMinMax(-10, 10));
		if (temp->getDamage() < 0)
			temp->setDamage(0);
		if (temp->getDefense() < 0)
			temp->setDefense(0);
		for (int i = 0; i < SharedData::GetInstance()->inventory.EQinventory.size(); ++i)
		{
			if (SharedData::GetInstance()->inventory.EQinventory[i]->getName() == "UNDEFINED")
			{
				SharedData::GetInstance()->inventory.EQinventory[i] = temp;
				break;
			}
		}
	}
}

void Scene2::PlayerUpdate(double dt)
{
	if (MS == PLAY)
	{
		if (Application::IsKeyPressed('W'))
		{
			SharedData::GetInstance()->player->SetPlayerMesh(meshList[GEO_HEROUP]);

			SpriteAnimation *playerUP = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROUP]);
			if (playerUP)
			{
				playerUP->Update(dt);
				playerUP->m_anim->animActive = true;
			}
			SharedData::GetInstance()->player->MoveUpDown(false, m_cMap, dt);

		}
		if (Application::IsKeyPressed('S'))
		{
			SharedData::GetInstance()->player->SetPlayerMesh(meshList[GEO_HEROD]);

			SpriteAnimation *playerDOWN = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROD]);
			if (playerDOWN)
			{
				playerDOWN->Update(dt);
				playerDOWN->m_anim->animActive = true;
			}
			SharedData::GetInstance()->player->MoveUpDown(true, m_cMap, dt);
		}
		if (Application::IsKeyPressed('A'))
		{
			SharedData::GetInstance()->player->SetPlayerMesh(meshList[GEO_HEROLR]);
			SharedData::GetInstance()->player->SetFlipStatus(false);
			SpriteAnimation *playerLEFT = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROLR]);
			if (playerLEFT)
			{
				playerLEFT->Update(dt);
				playerLEFT->m_anim->animActive = true;
			}
			SharedData::GetInstance()->player->MoveLeftRight(true, m_cMap, dt);
		}
		if (Application::IsKeyPressed('D'))
		{
			SharedData::GetInstance()->player->SetPlayerMesh(meshList[GEO_HEROLR]);
			SharedData::GetInstance()->player->SetFlipStatus(true);
			SpriteAnimation *playerRIGHT = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROLR]);
			if (playerRIGHT)
			{
				playerRIGHT->Update(dt);

				playerRIGHT->m_anim->animActive = true;
			}
			SharedData::GetInstance()->player->MoveLeftRight(false, m_cMap, dt);
		}

	}
	SharedData::GetInstance()->player->HeroUpdate(m_cMap, dt, meshList);

	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('S') || Application::IsKeyPressed('A') || Application::IsKeyPressed('D'))
	{
		//cout << SharedData::GetInstance()->soundFootstep << endl;
		SharedData::GetInstance()->soundManager.SoundPlay("Sound/footstepgrass.wav", &SharedData::GetInstance()->soundFootstep, 1.0f, false);
	}
	else
	{
		SharedData::GetInstance()->soundManager.SoundPause(&SharedData::GetInstance()->soundFootstep);
	}

	/*SpriteAnimation *arrow = dynamic_cast<SpriteAnimation*>(meshList[GEO_BATTLEARROW]);
	if (arrow)
	{
	arrow->Update(dt);
	arrow->m_anim->animActive = true;
	}*/

	if (Application::IsKeyPressed('I') && !SharedData::GetInstance()->IkeyPressed && GS)
	{
		GS = INVENTORY_SCREEN;
		SharedData::GetInstance()->inventory.ResetInventoryVariables();
		SharedData::GetInstance()->IkeyPressed = true;
	}
	else if (!Application::IsKeyPressed('I') && SharedData::GetInstance()->IkeyPressed)
	{
		SharedData::GetInstance()->IkeyPressed = false;
	}
}

void Scene2::GOupdate(double dt)
{
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPCPIC]);
	if (sa)
	{
		sa->Update(dt);
		sa->m_anim->animActive = true;
	}

	SpriteAnimation *golem = dynamic_cast<SpriteAnimation*>(meshList[GEO_MONSTER]);
	if (golem)
	{
		golem->Update(dt);
		golem->m_anim->animActive = true;
	}

	SpriteAnimation *lives = dynamic_cast<SpriteAnimation*>(meshList[GEO_LIVES]);
	if (lives)
	{
		//lives->Update(dt);
		lives->m_currentFrame = SharedData::GetInstance()->playerLives;
		lives->m_anim->animActive = true;
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
	for (int i = 0; i < m_goList.size(); ++i)
	{
		if (m_goList[i]->type == GameObject::GO_ENEMY && MS == IN_DIALOUGE)
			continue;

		m_goList[i]->Update(dt, SharedData::GetInstance()->player->GetPosition(), SharedData::GetInstance()->player->GetMapOffset(), m_cMap);

		if (m_goList[i]->type == GameObject::GO_NPC)
		{
			NPC* temp = (NPC*)m_goList[i];
			if (temp->GetAnimationState() == NPC::NPC_AWANDERING && MS == PLAY)
			{
				if (temp->GetID() == 1)
				{
					SpriteAnimation *npc1 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPC1_LEFT]);
					if (npc1)
					{
						npc1->Update(dt*0.2);
						npc1->m_anim->animActive = true;
					}
				}
				if (temp->GetID() == 3)
				{
					SpriteAnimation *npc2 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPC2_LEFT]);
					if (npc2)
					{
						npc2->Update(dt*0.2);
						npc2->m_anim->animActive = true;
					}
				}
				if (temp->GetID() == 2)
				{
					SpriteAnimation *npc3 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPC3_LEFT]);
					if (npc3)
					{
						npc3->Update(dt*0.2);
						npc3->m_anim->animActive = true;
					}
				}
			}
			if (temp->collideWhichNPC() == npcID)
				temp->SetState(currState);

			if (temp->GetColDetected() && Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
			{
				npctalk.str("");
				MS = IN_DIALOUGE;
				SharedData::GetInstance()->ENTERkeyPressed = true;
				temp->ScrollDialogue(dialogueNum);
			}
			else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
				SharedData::GetInstance()->ENTERkeyPressed = false;

			if (temp->GetDialogueState() == temp->GetCurrState() && temp->GetID() == temp->collideWhichNPC())
			{
				if (dialogueNum == temp->maxDia)
				{
					npctalk.str("");
					npctalk << "Enter to Exit";
				}
				npcPic = temp->collideWhichNPC();

				if (temp->GetNum() == dialogueNum)
				{
					npctalk.str("");
					npctalk << temp->GetDialogue();
				}

				if (dialogueNum >= 1 && dialogueNum <= temp->maxDia)
				{
					renderNPCstuff = true;
				}
				else if (dialogueNum >= temp->maxDia)
				{
					npctalk.str("");
					if (temp->GetID() == temp->collideWhichNPC())
					{
						npcID = temp->collideWhichNPC();

						currState = 2;

						dialogueNum = 0;
						renderNPCstuff = false;
						MS = PLAY;
					}
				}
			}
		}
	}
	if (Application::IsKeyPressed('Q') && !SharedData::GetInstance()->QKeyPressed)
	{
		SharedData::GetInstance()->QKeyPressed = true;

		if (!renderQuest && MS == PLAY)
		{
			renderQuest = true;
			MS = IN_DIALOUGE;
		}
		else if (renderQuest)
		{
			renderQuest = false;
			MS = PLAY;
		}
	}
	if (!Application::IsKeyPressed('Q') && SharedData::GetInstance()->QKeyPressed)
	{
		SharedData::GetInstance()->QKeyPressed = false;
	}
}

void Scene2::MapUpdate(double dt)
{
	if (MS == PLAY)
	{
		PlayerUpdate(dt);
		SharedData::GetInstance()->soundManager.SoundPlay("Sound/Map.mp3", &SharedData::GetInstance()->worldBGM, 0.3f, false);
	}
	GOupdate(dt);
	if (Application::IsKeyPressed('R'))
	{

	}

}

void Scene2::Update(double dt)
{
	SceneBase::Update(dt);

	static bool f6press = false;
	if (Application::IsKeyPressed(VK_F6) && !f6press)
	{
		f6press = true;
		SharedData::GetInstance()->inventory.SortInventory();
	}
	else if (!Application::IsKeyPressed(VK_F6) && f6press)
	{
		f6press = false;
	}
	if (Application::IsKeyPressed('Z') && !SharedData::GetInstance()->ZKeyPressed)
	{
		SharedData::GetInstance()->ZKeyPressed = true;
		if (GS != TAMAGUCCI_SCREEN)
			GS = TAMAGUCCI_SCREEN;
		else
			GS = MAP;
	}
	else if (!Application::IsKeyPressed('Z') && SharedData::GetInstance()->ZKeyPressed)
	{
		SharedData::GetInstance()->ZKeyPressed = false;
	}

	//For battle scene Dialogue
	if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed && SharedData::GetInstance()->playerBattleDialogue)
	{
		SharedData::GetInstance()->ENTERkeyPressed = true;
		battleScene.EndPlayerTurn();
		battleScene.SetBattleSelection(BattleSystem::BS_ATTACK);
		SharedData::GetInstance()->playerBattleDialogue = false;
	}
	else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
	{
		SharedData::GetInstance()->ENTERkeyPressed = false;
	}

	switch (GS)
	{
	case MAP:
		MapUpdate(dt);
		SharedData::GetInstance()->tamagucci.TamagucciBackgroundUpdate(dt);
	case BATTLE:
	case CATCH:
	case INVENTORY_SCREEN:
	case TAMAGUCCI_SCREEN:
		SceneGame::Update(dt);
		break;
	}
	fps = (float)(1.f / dt);
}

void Scene2::RenderMap()
{
	RenderBackground(meshList[GEO_BACKGROUND]);
	RenderTileMap(meshList[GEO_TILESET1], m_cMap);
	RenderTileMap(meshList[GEO_TILESET1], m_cMap2);
	std::ostringstream ss;
	RenderPlayer();
	Render2DMeshWScale(meshList[GEO_ICONTAM], false, 1, 1, 700, 10, false);
	if (SharedData::GetInstance()->tamagucci.getAlertStatus())
		Render2DMeshWScale(meshList[GEO_EXCLAIM], false, 1, 1, 700, 51 + alertPosOffset, false);
	Render2DMeshWScale(meshList[GEO_ICONINV], false, 1, 1, 630, 10, false);
	Render2DMeshWScale(meshList[GEO_ICONQUEST], false, 1, 1, 560, 10, false);
	Render2DMeshWScale(meshList[GEO_LIVES], false, 120, 50, 0, 550, false);

	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 30, 0, 0);
}

void Scene2::RenderGO()
{
	for (int i = 0; i < m_goList.size(); i++)
	{
		if (m_goList[i]->active == true)
		{
			if (m_goList[i]->type == GameObject::GO_ITEM)
			{
				Items* temp = (Items*)m_goList[i];
				if (temp->itemType == Items::POTION)
					Render2DMeshWScale(meshList[GEO_POTION], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - SharedData::GetInstance()->player->GetMapOffset().x, m_goList[i]->position.y - SharedData::GetInstance()->player->GetMapOffset().y, false);
				if (temp->itemType == Items::TRAP)
					Render2DMeshWScale(meshList[GEO_TRAP], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - SharedData::GetInstance()->player->GetMapOffset().x, m_goList[i]->position.y - SharedData::GetInstance()->player->GetMapOffset().y, false);
			}
			if (m_goList[i]->type == GameObject::GO_NPC)
			{
				NPC* temp = (NPC*)m_goList[i];
				if (MS == IN_DIALOUGE)
					temp->SetAnimationState(NPC::NPC_AIDLE);

				if (renderNPCstuff)
				{
					if (npcPic == 1)
						Render2DMeshWScale(meshList[GEO_NPCPIC], false, 350, 350, 450, 60, false);
					if (npcPic == 2)
						Render2DMeshWScale(meshList[GEO_NPCPIC2], false, 350, 350, 450, 60, false);
					if (npcPic == 3)
						Render2DMeshWScale(meshList[GEO_NPCPIC3], false, 350, 350, 450, 60, false);
					Render2DMeshWScale(meshList[GEO_BATTLEDIALOUGEBACKGROUND], false, 1, 0.25, 10, 20, false);
				}
				if (temp->GetID() == 1 && temp->GetDialogueState() == temp->GetCurrState() && temp->GetNum() == 1)
					Render2DMeshWScale(meshList[GEO_NPC1_LEFT], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - SharedData::GetInstance()->player->GetMapOffset().x, m_goList[i]->position.y - SharedData::GetInstance()->player->GetMapOffset().y, temp->GetMoveRight(), 32);
				if (temp->GetID() == 2 && temp->GetDialogueState() == temp->GetCurrState() && temp->GetNum() == 1)
					Render2DMeshWScale(meshList[GEO_NPC3_LEFT], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - SharedData::GetInstance()->player->GetMapOffset().x, m_goList[i]->position.y - SharedData::GetInstance()->player->GetMapOffset().y, temp->GetMoveRight(), 32);
				if (temp->GetID() == 3 && temp->GetDialogueState() == temp->GetCurrState() && temp->GetNum() == 1)
					Render2DMeshWScale(meshList[GEO_NPC2_LEFT], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - SharedData::GetInstance()->player->GetMapOffset().x, m_goList[i]->position.y - SharedData::GetInstance()->player->GetMapOffset().y, temp->GetMoveRight(), 32);
				RenderTextOnScreen(meshList[GEO_TEXT], npctalk.str(), Color(1, 1, 0), 30, 60, 100);
			}
			if (m_goList[i]->type == GameObject::GO_ENEMY)
			{
				Enemy* temp = (Enemy*)m_goList[i];
				Render2DMeshWScale(meshList[GEO_MONSTER], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - SharedData::GetInstance()->player->GetMapOffset().x, m_goList[i]->position.y - SharedData::GetInstance()->player->GetMapOffset().y, temp->GetFlipStatus(), 50);
			}
		}
		if (m_goList[i]->type == GameObject::GO_NEXT)
		{
			if (m_goList[i]->CheckCollision(SharedData::GetInstance()->player->GetPosition(), SharedData::GetInstance()->player->GetMapOffset(), m_cMap))
			{
				Render2DMeshWScale(meshList[GEO_POPUP], false, 1, 1, 150, 200, false);
				if (Application::IsKeyPressed(VK_RETURN))
				{
					SharedData::GetInstance()->player->SetMapOffset(Vector3(0, 0, 0));
					SharedData::GetInstance()->player->SetPosition(Vector3(80, 830, 0));
					Reset();
					SharedData::GetInstance()->gameState = SharedData::GAME_S1;
				}
			}
		}
	}
	

	if (renderQuest)
	{
		Render2DMeshWScale(meshList[GEO_QUEST], false, 1.5, 1.5, 100, 80);

		MS = IN_DIALOUGE;

		if (SharedData::GetInstance()->capturedBanshee)
			Render2DMeshWScale(meshList[GEO_TICKCROSS], false, 1, 1, 245, 250);

		if (SharedData::GetInstance()->capturedCerebus)
			Render2DMeshWScale(meshList[GEO_TICKCROSS], false, 1, 1, 245, 100);

		if (SharedData::GetInstance()->capturedDragon)
			Render2DMeshWScale(meshList[GEO_TICKCROSS], false, 1, 1, 500, 100);

		if (SharedData::GetInstance()->capturedGolem)
			Render2DMeshWScale(meshList[GEO_TICKCROSS], false, 1, 1, 500, 250);
	}
}

void Scene2::Render()
{
	SceneGame::Render();
	switch (GS)
	{
	case MAP:
		RenderMap();
		RenderGO();
		break;
	case BATTLE:
		RenderBattleScene();
		break;
	case CATCH:
		RenderCatch();
		break;
	case INVENTORY_SCREEN:
		RenderMap();
		RenderInventory();
		break;

	case ITEM_SCREEN:
		RenderBackground(meshList[GEO_INVENTORYBACKGROUND]);
		RenderItemScreen();
		break;
	case EQUIP_SCREEN:
		RenderBackground(meshList[GEO_INVENTORYBACKGROUND]);
		RenderEquipScreen();

	case TAMAGUCCI_SCREEN:
		RenderTamagucci();
		break;
	case LOSE:
		RenderBackground(meshList[GEO_LOSE]);
		if (Application::IsKeyPressed(VK_SPACE) && !SharedData::GetInstance()->SpaceKeyPressed)
		{
			SharedData::GetInstance()->SpaceKeyPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_SPACE) && SharedData::GetInstance()->SpaceKeyPressed)
		{
			SharedData::GetInstance()->SpaceKeyPressed = false;
			SharedData::GetInstance()->gameState = SharedData::MENU;
			GS = MAP;
		}
		break;
	}
}

void Scene2::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	for (int i = 0; i < m_goList.size(); ++i)
	{
		delete m_goList[i];
		m_goList.erase(m_goList.begin() + i);
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}