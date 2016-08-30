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
	:
	m_cMap(NULL)
	, EnemyInBattle(NULL)
	//, NPCInConvo(NULL)
{
}

Scene2::~Scene2()
{
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
}

void Scene2::Init()
{
	SceneBase::Init();
	Monster::InitDatabase();
	Equipment::InitDatabase();
	//Init GameState Here for testing purposes
	GS = TESTMAP;
	MS = PLAY;
	capturedMonster = false;
	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap->LoadMap("Data//KyMapData.csv");

	m_cMap2 = new CMap();
	m_cMap2->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap2->LoadMap("Data//KyMapData2.csv");

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

	GameObject* touch = new GameObject(Vector3(50.f, 50.f, 1));
	touch->position.Set(820, 150, 1);
	touch->type = GameObject::GO_DOWN;
	m_goList.push_back(touch);

	GameObject* down = new GameObject(Vector3(50.f, 50.f, 1));
	down->position.Set(612, 1200, 1);
	down->type = GameObject::GO_DOWN;
	m_goList.push_back(down);

	for (int i = 0; i < 4; ++i)
	{
		Enemy* theEnemy;
		theEnemy = new Enemy(Monster::getMonster(Monster::DRAGON), Vector3(32.f, 32.f, 1));
		theEnemy->type = GameObject::GO_ENEMY;
		theEnemy->position.Set(200, 700, 1);
		m_goList.push_back(theEnemy);
	}

	enemyMaxHealth = 100;
	currHealth = 100;
	enemyCatchPercentage = 0;
	npc.ReadFromFile("NPC//2.txt", m_goList);
	vector<NPC*>npcvec = npc.GetVec();

	for (int i = 0; i < npcvec.size(); i++)
	{
		if (npcvec[i]->GetID() == 1)
			npcvec[i]->position.Set(500, 100, 1);
		if (npcvec[i]->GetID() == 2)
			npcvec[i]->position.Set(700, 200, 1);
		if (npcvec[i]->GetID() == 3)
			npcvec[i]->position.Set(100, 400, 1);
		npcvec[i]->currDia = 1;
		m_goList.push_back(dynamic_cast<NPC*>(npcvec[i]));
	}

	// Initialise and load the REAR tile map
	//m_cRearMap = new CMap();
	//m_cRearMap->Init( 600, 800, 24, 32, 600, 1600 );
	//m_cRearMap->LoadMap( "Image//MapDesign_Rear.csv" );

	// Initialise the hero's position
	theHero = new CPlayerInfo();
	theHero->Init();
	theHero->SetPosition(Vector3(615, 2000, 0));
	theHero->SetPlayerMesh(meshList[GEO_HEROD]);
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	battleMonsterPos.Set(300, 240, 0);
	battleMonsterScale.Set(0.3, 0.3, 1);
	monsterScaleUp = true;


	SharedData::GetInstance()->soundManager.Init();


	//Battle HUD
	maxHpScale = 17.4f;
	hpPos.Set(36, 574, 0);
	mpPos.Set(36, 548.9, 0);
	maxMpScale = 10.9f;

	for (int j = 0; j < 10; ++j)
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
	flashEffect = false;
	flashTimer = 0.0f;
	renderedMp = 100;
	renderedHp = 100;
}

bool Scene2::GetMonsterScaleUp()
{
	return monsterScaleUp;
}

void Scene2::SetMonsterScaleUp(bool set)
{
	this->monsterScaleUp = set;
}

float Scene2::GetBattleMonsterScaleX()
{
	return battleMonsterScale.x;
}

void Scene2::SetBattleMonsterScaleX(float x)
{
	this->battleMonsterScale.x = x;
}

float Scene2::GetBattleMonsterScaleY()
{
	return battleMonsterScale.y;
}

void Scene2::SetBattleMonsterScaleY(float y)
{
	this->battleMonsterScale.y = y;
}

float Scene2::GetBattleMonsterPosX()
{
	return battleMonsterPos.x;
}

float Scene2::GetBattleMonsterPosY()
{
	return battleMonsterPos.y;
}

void Scene2::SetBattleMonsterPosX(float x)
{
	this->battleMonsterPos.x = x;
}

void Scene2::SetBattleMonsterPosY(float y)
{
	this->battleMonsterPos.y = y;
}

void Scene2::SetGS(string set)
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

void Scene2::CatchUpdate(double dt)
{
	enemyCatchPercentage = (EnemyInBattle->GetMaxHealth() - EnemyInBattle->GetHealth()) / EnemyInBattle->GetMaxHealth() * redbar->scale.x;

	float prevScale = greenbar->scale.x;
	greenbar->scale.x = enemyCatchPercentage;
	if (greenbar->scale.x > prevScale)
	{
		greenbar->position.x -= (greenbar->scale.x - prevScale) * 0.5;
	}

	chargebar->Update(dt, 200.f);
	greenbar->Update(dt, 800.f);
	if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
	{
		SharedData::GetInstance()->ENTERkeyPressed = true;
		if (chargebar->CheckCollision(greenbar, m_cMap))
		{
			cout << "CAPTURED" << endl;
			capturedMonster = true;
			currState = 3;
			SharedData::GetInstance()->inventory.addToInventory(EnemyInBattle);
			RemoveEnemy();
			GS = TESTMAP;
			return;
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
}

void Scene2::EnterBattleScene(Enemy* enemy)
{
	battleScene.SetBattleStart(true);
	SharedData::GetInstance()->soundManager.StopAllSound();
	SharedData::GetInstance()->soundManager.SoundPlay("Sound/battleStart.mp3", &SharedData::GetInstance()->battleStart, 0.3f, true);
	EnemyInBattle = enemy;
	GS = BATTLE;
}

void Scene2::PlayerUpdate(double dt)
{
	if (MS == PLAY)
	{
		if (MS == PLAY)
		{
			if (Application::IsKeyPressed('W'))
			{
				theHero->SetPlayerMesh(meshList[GEO_HEROUP]);

				SpriteAnimation *playerUP = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROUP]);
				if (playerUP)
				{
					playerUP->Update(dt);
					playerUP->m_anim->animActive = true;
				}
				this->theHero->MoveUpDown(false, m_cMap, dt);

			}
			if (Application::IsKeyPressed('S'))
			{
				theHero->SetPlayerMesh(meshList[GEO_HEROD]);

				SpriteAnimation *playerDOWN = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROD]);
				if (playerDOWN)
				{
					playerDOWN->Update(dt);
					playerDOWN->m_anim->animActive = true;
				}
				this->theHero->MoveUpDown(true, m_cMap, dt);
			}
			if (Application::IsKeyPressed('A'))
			{
				theHero->SetPlayerMesh(meshList[GEO_HEROLR]);
				theHero->SetFlipStatus(false);
				SpriteAnimation *playerLEFT = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROLR]);
				if (playerLEFT)
				{
					playerLEFT->Update(dt);
					playerLEFT->m_anim->animActive = true;
				}
				this->theHero->MoveLeftRight(true, m_cMap, dt);
			}
			if (Application::IsKeyPressed('D'))
			{
				theHero->SetPlayerMesh(meshList[GEO_HEROLR]);
				theHero->SetFlipStatus(true);
				SpriteAnimation *playerRIGHT = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROLR]);
				if (playerRIGHT)
				{
					playerRIGHT->Update(dt);

					playerRIGHT->m_anim->animActive = true;
				}
				this->theHero->MoveLeftRight(false, m_cMap, dt);
			}

		}
	}
	theHero->HeroUpdate(m_cMap, dt, meshList);

	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('S') || Application::IsKeyPressed('A') || Application::IsKeyPressed('D'))
	{
		//cout << SharedData::GetInstance()->soundFootstep << endl;
		SharedData::GetInstance()->soundManager.SoundPlay("Sound/footstepgrass.wav", &SharedData::GetInstance()->soundFootstep, 1.0f, true);
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
		SharedData::GetInstance()->inventory.ResetInventory();
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

		m_goList[i]->Update(dt, theHero->GetPosition(), theHero->GetMapOffset(), m_cMap);

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

			if (temp->collisionDetected && Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
			{
				npctalk.str("");
				MS = IN_DIALOUGE;
				SharedData::GetInstance()->ENTERkeyPressed = true;
				temp->ScrollDialogue(dialogueNum);
			}
			else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
				SharedData::GetInstance()->ENTERkeyPressed = false;

			if (temp->GetDialogueState() == temp->currState && temp->GetID() == temp->collideWhichNPC())
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
}
//For the mini game
void Scene2::TamagucciUpdate(double dt)
{
	SpriteAnimation *tamhappy = dynamic_cast<SpriteAnimation*>(meshList[GEO_TAMHAPPY]);
	if (tamhappy)
	{
		tamhappy->Update(dt);
		tamhappy->m_anim->animActive = true;
	}
	if (tamagucci.GetShowFood())
	{
		SpriteAnimation *tamfood = dynamic_cast<SpriteAnimation*>(meshList[GEO_TAMFOOD]);
		if (tamfood)
		{
			tamfood->Update(dt);
			tamfood->m_anim->animActive = true;
		}
	}
	SpriteAnimation *sleep = dynamic_cast<SpriteAnimation*>(meshList[GEO_TAMSLEEP]);
	if (sleep && tamagucci.GetSleep())
	{
		sleep->Update(dt);
		sleep->m_anim->animActive = true;
	}
	SpriteAnimation *salad = dynamic_cast<SpriteAnimation*>(meshList[GEO_SALAD]);

	foodAnimOver = false;
	if (salad && tamagucci.GetTouchedFood() && salad->m_anim->animActive)
	{
		salad->Update(dt);
	}
	else if (!salad->m_anim->animActive)
	{
		foodAnimOver = true;
		salad->m_anim->animActive = true;
	}
	tamagucci.SetAnimationOver(foodAnimOver);
}

void Scene2::UpdateInventory(double dt)
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

	SharedData::GetInstance()->inventory.Update(dt);
}

void Scene2::renderInventoryItems()
{
	static float xpos = 0.f;
	static float ypos = 0.f;

	if (Application::IsKeyPressed('U'))
	{
		ypos += 0.5f;
	}
	if (Application::IsKeyPressed('J'))
	{
		ypos -= 0.5f;
	}
	if (Application::IsKeyPressed('H'))
	{
		xpos -= 0.5f;
	}
	if (Application::IsKeyPressed('K'))
	{
		xpos += 0.5f;
	}

	for (int i = 0; i < SharedData::GetInstance()->inventory.EQinventory.size(); ++i)
	{
		if (SharedData::GetInstance()->inventory.EQinventory[i]->getName() == "UNDEFINED")
			continue;
		Vector3 pos;
		pos.y = i / 8;
		pos.x = i % 8;
		switch (SharedData::GetInstance()->inventory.EQinventory[i]->getType())
		{
		case Equipment::SWORD:
			Render2DMeshWScale(meshList[GEO_SWORD], false, 43, 44, 426.5 + pos.x * 46, 254 + -pos.y * 46, false);
			break;
		case Equipment::SHIELD:
			Render2DMeshWScale(meshList[GEO_SHIELD], false, 45, 45.5, 426.5 + pos.x * 46, 254 + -pos.y * 46, false);
			break;
		case Equipment::ARMOUR:
			Render2DMeshWScale(meshList[GEO_CHESTPLATE], false, 44.5, 44.5, 426.5 + pos.x * 46, 254 + -pos.y * 46, false);
			break;
		case Equipment::HELMET:
			Render2DMeshWScale(meshList[GEO_HELMET], false, 44.5, 44.5, 426.5 + pos.x * 46, 254 + -pos.y * 46, false);
			break;
		case Equipment::LEG:
			Render2DMeshWScale(meshList[GEO_LEG], false, 44.5, 44.5, 426.5 + pos.x * 46, 254 + -pos.y * 46, false);
			break;
		}
	}

	if (SharedData::GetInstance()->inventory.getArmour())
	{
		Render2DMeshWScale(meshList[GEO_CHESTPLATE], false, 44.5, 44.5, 426.5 + 47.5, 254 + 107.5, false);
	}

	if (SharedData::GetInstance()->inventory.getRightArm())
	{
		if (SharedData::GetInstance()->inventory.getRightArm()->getType() == Equipment::SWORD)
		{
			Render2DMeshWScale(meshList[GEO_SWORD], false, 44.5, 44.5, 426.5 + 93.5, 254 + 105.5, false);
		}
		else if (SharedData::GetInstance()->inventory.getRightArm()->getType() == Equipment::SHIELD)
		{
			Render2DMeshWScale(meshList[GEO_SHIELD], false, 44.5, 44.5, 426.5 + 93.5, 254 + 105.5, false);
		}
	}

	if (SharedData::GetInstance()->inventory.getLeftArm())
	{
		if (SharedData::GetInstance()->inventory.getLeftArm()->getType() == Equipment::SWORD)
		{
			Render2DMeshWScale(meshList[GEO_SWORD], false, 44.5, 44.5, 426.5, 254 + 105.5, false);
		}
		else if (SharedData::GetInstance()->inventory.getLeftArm()->getType() == Equipment::SHIELD)
		{
			Render2DMeshWScale(meshList[GEO_SHIELD], false, 44.5, 44.5, 426.5, 254 + 105.5, false);
		}
	}

	if (SharedData::GetInstance()->inventory.getHead())
	{
		Render2DMeshWScale(meshList[GEO_HELMET], false, 44.5, 44.5, 426.5 + 47, 254 + 151, false);
	}

	if (SharedData::GetInstance()->inventory.getLeg())
	{
		Render2DMeshWScale(meshList[GEO_LEG], false, 44.5, 44.5, 426.5 + 45.5, 254 + 59, false);
	}

	stringstream ss;
	ss.str("");
	ss << "ATTACK :         " << SharedData::GetInstance()->inventory.GetTotalATK();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 30, 43, 371);
	ss.str("");
	ss << "DEFENSE:         " << SharedData::GetInstance()->inventory.GetTotalDEF();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 30, 43, 371 - 33);
}

void Scene2::renderInventoryMenus()
{
	if (SharedData::GetInstance()->inventory.getEquipmentLookAt()->getName() != "UNDEFINED")
	{
		std::ostringstream ss;
		ss.str("");
		ss << "Name: " << SharedData::GetInstance()->inventory.getEquipmentLookAt()->getName() << endl;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 562.5, 392);
		ss.str("");
		ss << "Damage: " << SharedData::GetInstance()->inventory.getEquipmentLookAt()->getDamage() << endl;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 562.5, 392 - 20);
		ss.str("");
		ss << "Defense: " << SharedData::GetInstance()->inventory.getEquipmentLookAt()->getDefense() << endl;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 562.5, 392 - 40);
	}

	if (SharedData::GetInstance()->inventory.getInputState() == Inventory::INVENTORY)
		Render2DMesh(meshList[GEO_INVENTORYSEEKER], false, 1, 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46, 254 + SharedData::GetInstance()->inventory.getSeeker().y * 46);
	if (SharedData::GetInstance()->inventory.getInputState() == Inventory::EQUIP_OPTIONS)
	{
		Render2DMeshWScale(meshList[GEO_INVENTORYSECONDBACKGROUND], false, 122, -44 - (float)(SharedData::GetInstance()->inventory.getOptions().size() * 18), 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46 + 36, 290 + SharedData::GetInstance()->inventory.getSeeker().y * 46);
		for (int i = 0; i < SharedData::GetInstance()->inventory.getOptions().size(); ++i)
		{
			std::ostringstream ss;
			ss.str("");
			ss << SharedData::GetInstance()->inventory.getOptions()[i];
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46 + 46, 254 + SharedData::GetInstance()->inventory.getSeeker().y * 46 - (i * 20));
		}
		std::ostringstream ss;
		ss.str("");
		ss << SharedData::GetInstance()->inventory.getOptions()[SharedData::GetInstance()->inventory.getSecondSeeker()];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46 + 46, 254 + SharedData::GetInstance()->inventory.getSeeker().y * 46 - (SharedData::GetInstance()->inventory.getSecondSeeker() * 20));
	}
}

void Scene2::RenderInventory()
{
	RenderBackground(meshList[GEO_INVENTORYBACKGROUND]);
	renderInventoryItems();
	renderInventoryMenus();
}

void Scene2::RenderItemScreen()
{
	//std::ostringstream ss;
	//ss.str("");
	//ss.precision(5);
	//ss << "Potion: " << SharedData::GetInstance()->inventory.getPotionInventory() << endl;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 60, 300, 450);

	//std::ostringstream ss2;
	//ss2.str("");
	//ss2.precision(5);
	//ss2 << "Traps: " << SharedData::GetInstance()->inventory.getTrapInventory()  << endl;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(1, 0, 0), 60, 300, 390);
}

void Scene2::ItemScreenUpdate(double dt)
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

	SharedData::GetInstance()->inventory.Update(dt);
}

void Scene2::RenderEquipScreen()
{
	std::ostringstream ss;
	ss.str("");
	ss.precision(5);
	ss << "Equips: " << endl;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 60, 300, 450);

}

void Scene2::MapUpdate(double dt)
{
	if (MS == PLAY)
	{
		PlayerUpdate(dt);
		GOupdate(dt);
		SharedData::GetInstance()->soundManager.SoundPlay("Sound/route1.mp3", &SharedData::GetInstance()->worldBGM, 0.3f, true);
	}

	PlayerUpdate(dt);
	GOupdate(dt);
	if (Application::IsKeyPressed('R'))
	{

	}

}
void Scene2::NPCUpdate(double dt)
{
	GOupdate(dt);
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
			GS = TESTMAP;
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
	case TESTMAP:
		MapUpdate(dt);
		break;
	case BATTLE:

		//Updating of catch percentage
		enemyCatchPercentage = (EnemyInBattle->GetMaxHealth() - EnemyInBattle->GetHealth()) / EnemyInBattle->GetMaxHealth() * 100;

		//Flashing effect of dialogue
		if (flashEffect)
		{
			flashTimer += 1.5 * dt;
			if (flashTimer > 1.0f)
			{
				flashTimer = 0;
				flashEffect = false;
			}
		}
		else
		{
			flashTimer -= 1.5 * dt;
			if (flashTimer < -1.0f)
			{
				flashTimer = 0.0f;
				flashEffect = true;
			}
		}

		//Decrease Rendered HP on screen
		if (theHero->GetHP() < renderedHp)
		{
			renderedHp -= dt * 100;

			//if enemy not dead		
			if (EnemyInBattle->GetHealth() < 0)
			{
				Scene4* mainScene = (Scene4*)Application::GetInstance().GetScene();

				//Player win
				battleScene.Reset();
				mainScene->RemoveEnemy();
				//destory enemy here
			}
			else if (renderedHp < 0.0f)
			{
				if (SharedData::GetInstance()->playerLives > 0)
				{
					SharedData::GetInstance()->playerLives--;
					SharedData::GetInstance()->stateCheck = true;
					SharedData::GetInstance()->gameState = SharedData::GAME_S1;
				}
				else
					//Player Lose should do auto load to previous save file
					GS = LOSE;

				//Player Lose should do auto load to previous save file

				SharedData::GetInstance()->playerTurn = true;
				SharedData::GetInstance()->enemyTurn = false;
				battleScene.SetFirstChoice(true);
				battleScene.SetSecondChoice(false);
				battleScene.SetBattleSelection(BattleSystem::BS_ATTACK);
			}

		}

		//Decrease RenderedMP on screen
		if (theHero->GetMP() < renderedMp)
		{
			renderedMp -= dt * 100;
		}
		battleScene.UpdateBattleSystem(SharedData::GetInstance()->UPkeyPressed, SharedData::GetInstance()->DNkeyPressed, SharedData::GetInstance()->LEFTkeyPressed, SharedData::GetInstance()->RIGHTkeyPressed, SharedData::GetInstance()->ENTERkeyPressed, theHero, EnemyInBattle);
		//SharedData::GetInstance()->soundManager.SoundPlay("Sound/battleStart.mp3", &SharedData::GetInstance()->battleStart, 0.3f, true);

		if (SharedData::GetInstance()->BS_SlashRender)
		{
			SpriteAnimation *slashAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_SLASHANIMATION]);

			if (slashAnimation)
			{
				slashAnimation->m_anim->animActive = true;
				slashAnimation->Update(dt);

				if (slashAnimation->m_anim->animActive == false)
				{
					SharedData::GetInstance()->BS_SlashRender = false;
					slashAnimation->m_currentFrame = 0;
					SharedData::GetInstance()->playerBattleDialogue = true;
				}
			}
		}
		if (SharedData::GetInstance()->BS_StabRender)
		{
			SpriteAnimation *stabAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_STABANIMATION]);
			//cout << "Animation = " << slashAnimation->m_currentFrame << endl;
			if (stabAnimation)
			{
				stabAnimation->m_anim->animActive = true;
				stabAnimation->Update(dt);

				if (stabAnimation->m_anim->animActive == false)
				{
					SharedData::GetInstance()->BS_StabRender = false;
					stabAnimation->m_currentFrame = 0;
					SharedData::GetInstance()->playerBattleDialogue = true;
				}
			}
		}
		if (SharedData::GetInstance()->BS_ScreamRender)
		{
			SpriteAnimation *screamAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_SCREAMANIMATION]);

			if (screamAnimation)
			{
				screamAnimation->m_anim->animActive = true;
				screamAnimation->Update(dt);

				if (screamAnimation->m_anim->animActive == false)
				{
					SharedData::GetInstance()->BS_ScreamRender = false;
					screamAnimation->m_currentFrame = 0;
					SharedData::GetInstance()->playerBattleDialogue = true;
				}
			}
		}
		if (SharedData::GetInstance()->BS_BiteRender)
		{
			SpriteAnimation *biteAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_BITEANIMATION]);
			if (biteAnimation)
			{
				biteAnimation->m_anim->animActive = true;
				biteAnimation->Update(dt);

				if (biteAnimation->m_anim->animActive == false)
				{
					SharedData::GetInstance()->BS_BiteRender = false;
					biteAnimation->m_currentFrame = 0;
					SharedData::GetInstance()->playerBattleDialogue = true;
				}
			}
		}
		if (SharedData::GetInstance()->BS_RoarRender)
		{
			SpriteAnimation *roarAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_ROARANIMATION]);

			if (roarAnimation)
			{
				roarAnimation->m_anim->animActive = true;
				roarAnimation->Update(dt);

				if (roarAnimation->m_anim->animActive == false)
				{
					SharedData::GetInstance()->BS_RoarRender = false;
					roarAnimation->m_currentFrame = 0;
					SharedData::GetInstance()->playerBattleDialogue = true;
				}
			}
		}
		if (SharedData::GetInstance()->BS_SkinRender)
		{
			SpriteAnimation *skinAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_SKINANIMATION]);
			if (skinAnimation)
			{
				skinAnimation->m_anim->animActive = true;
				skinAnimation->Update(dt);

				if (skinAnimation->m_anim->animActive == false)
				{
					SharedData::GetInstance()->BS_SkinRender = false;
					skinAnimation->m_currentFrame = 0;
					SharedData::GetInstance()->playerBattleDialogue = true;
				}
			}
		}
		break;
	case CATCH:
		CatchUpdate(dt);
		break;
	case INVENTORY_SCREEN:
		UpdateInventory(dt);
		break;
	case TAMAGUCCI_SCREEN:
		if (SharedData::GetInstance()->inventory.getRightArm() || SharedData::GetInstance()->inventory.getLeftArm() || SharedData::GetInstance()->inventory.getArmour() || SharedData::GetInstance()->inventory.getHead())
			tamagucci.UpdateTamagucci(dt);
		TamagucciUpdate(dt);
		break;
	}
	fps = (float)(1.f / dt);
}

void Scene2::RenderPlayer()
{
	Render2DMesh(theHero->GetPlayerMesh(), false, 32.0f, theHero->GetPosition().x, theHero->GetPosition().y, theHero->GetFlipStatus());
}

static bool touched = true;
void Scene2::RenderTestMap()
{
	RenderBackground(meshList[GEO_BACKGROUND]);
	RenderTileMap(meshList[GEO_TILESET3], m_cMap);
	RenderTileMap(meshList[GEO_TILESET1], m_cMap2);

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
				if (temp->itemType == Items::TRAP)
					Render2DMeshWScale(meshList[GEO_TRAP], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false);
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
				if (temp->GetID() == 1 && temp->GetDialogueState() == temp->currState && temp->GetNum() == 1)
					Render2DMeshWScale(meshList[GEO_NPC1_LEFT], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, temp->GetMoveRight(), 32);
				if (temp->GetID() == 2 && temp->GetDialogueState() == temp->currState && temp->GetNum() == 1)
					Render2DMeshWScale(meshList[GEO_NPC3_LEFT], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, temp->GetMoveRight(), 32);
				if (temp->GetID() == 3 && temp->GetDialogueState() == temp->currState && temp->GetNum() == 1)
					Render2DMeshWScale(meshList[GEO_NPC2_LEFT], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, temp->GetMoveRight(), 32);
			}
		}
		if (m_goList[i]->type == GameObject::GO_ENEMY)
		{
			Enemy* temp = (Enemy*)m_goList[i];
			Render2DMeshWScale(meshList[GEO_MONSTERBANSHEE], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, temp->GetFlipStatus(), 50);
		}
		if (m_goList[i]->type == GameObject::GO_DOWN)
		{
			Render2DMeshWScale(meshList[GEO_POTION], false, m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->position.x - theHero->GetMapOffset().x, m_goList[i]->position.y - theHero->GetMapOffset().y, false);

			if (m_goList[i]->CheckCollision(theHero->GetPosition(), theHero->GetMapOffset(), m_cMap))
			{
				Render2DMeshWScale(meshList[GEO_POPUP], false, 1, 1, 150, 200, false);
				if (Application::IsKeyPressed(VK_RETURN))
				{
					SharedData::GetInstance()->stateCheck = true;
					SharedData::GetInstance()->gameState = SharedData::GAME_S1;
				}
			}
		}

	}
	RenderTextOnScreen(meshList[GEO_TEXT], npctalk.str(), Color(1, 1, 0), 30, 60, 100);
	RenderPlayer();
	Render2DMeshWScale(meshList[GEO_ICONTAM], false, 1, 1, 700, 10, false);
	Render2DMeshWScale(meshList[GEO_ICONINV], false, 1, 1, 630, 10, false);
	Render2DMeshWScale(meshList[GEO_LIVES], false, 120, 50, 0, 550, false);

	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 30, 0, 0);
}

void Scene2::RemoveEnemy()
{
	for (int i = 0; i < m_goList.size(); ++i)
	{
		if (m_goList[i] == EnemyInBattle)
		{
			delete m_goList[i];
			m_goList.erase(m_goList.begin() + i);
		}
	}
}

void Scene2::RenderMonster()
{
	if (battleScene.GetMonsterHitAnimation())
	{
		if (battleMonsterScale.x < 0.5 && battleMonsterScale.y < 0.5 && monsterScaleUp)
		{
			battleMonsterScale.x += 0.01;
			battleMonsterScale.y += 0.01;
			battleMonsterPos.x -= 4.0f;
			battleMonsterPos.y -= 3.0f;

			if (battleMonsterScale.x > 0.5 || battleMonsterScale.y > 0.5)
				monsterScaleUp = false;
		}
		if (battleMonsterScale.x > 0.3  && !monsterScaleUp || battleMonsterScale.y > 0.3 && !monsterScaleUp)
		{
			battleMonsterScale.x -= 0.01;
			battleMonsterScale.y -= 0.01;
			battleMonsterPos.x += 4.0f;
			battleMonsterPos.y += 3.0f;
			if (battleMonsterScale.x < 0.3 || battleMonsterScale.y < 0.3)
			{
				//Reset to player's turn after enemy end its turn
				//There must be something here i forgot to reset so 
				//The slash animation isn't working properly

				battleScene.SetMonsterHitAnimation(false);
				battleScene.SetFirstChoice(true);
				battleScene.SetSecondChoice(false);
				battleScene.SetBattleSelection(BattleSystem::BS_ATTACK);
				SharedData::GetInstance()->enemyTurn = false;
				SharedData::GetInstance()->playerTurn = true;
				battleMonsterScale.x = 0.3f;
				battleMonsterScale.y = 0.3f;
				monsterScaleUp = true;
			}
		}
		//cout << "x : " << battleMonsterScale.x << ", y : " << battleMonsterScale.y << endl;
	}

	Render2DMeshWScale(meshList[GEO_BATTLEMONSTER], false, battleMonsterScale.x, battleMonsterScale.y, battleMonsterPos.x, battleMonsterPos.y, false);



}

void Scene2::renderFirstTamagotchiFirstMenu(float yoffset)
{
	stringstream ss;
	ss << "Foods";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 80, 60 + yoffset);
	ss.str("");
	ss << "Sleep";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 230, 60 + yoffset);
	ss.str("");
	ss << "Entertain";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 360, 60 + yoffset);
	ss.str("");
	ss << "Clean";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 530, 60 + yoffset);
	ss.str("");
	ss << "Stats";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 660, 60 + yoffset);
}

void Scene2::renderTamagotchiMenu()
{
	static float xpos = 0.f;
	static float ypos = 0.f;

	if (Application::IsKeyPressed('I'))
	{
		ypos += 0.5f;
	}
	if (Application::IsKeyPressed('K'))
	{
		ypos -= 0.5f;
	}
	if (Application::IsKeyPressed('J'))
	{
		xpos -= 0.5f;
	}
	if (Application::IsKeyPressed('L'))
	{
		xpos += 0.5f;
	}

	Vector3 arrowPos;
	stringstream ss;
	switch (tamagucci.getMenuState())
	{
	case TAMAGUCCI::FIRSTMENU:
		//RENDER OPTIONS
		renderFirstTamagotchiFirstMenu();
		//RENDER OPTION SEEKER
		switch (tamagucci.getFirstMenuOption())
		{
		case TAMAGUCCI::FOOD:
			arrowPos.Set(30, 60);
			break;
		case TAMAGUCCI::SLEEP:
			arrowPos.Set(170, 60);
			break;
		case TAMAGUCCI::ENTERTAINMENT:
			arrowPos.Set(310, 60);
			break;
		case TAMAGUCCI::CLEAN:
			arrowPos.Set(480, 60);
			break;
		case TAMAGUCCI::STATS:
			arrowPos.Set(620, 60);
			break;
		}
		Render2DMeshWScale(meshList[GEO_BATTLEARROW], false, 0.05, 0.03, arrowPos.x, arrowPos.y, false);
		break;
	case TAMAGUCCI::SECONDMENU:
		switch (tamagucci.getFirstMenuOption())
		{
		case TAMAGUCCI::FOOD:
			renderFirstTamagotchiFirstMenu(27.5f);
			//RENDER MENU
			switch (tamagucci.getFoodChoice())
			{
			case TAMAGUCCI::FC_SALAD:
			case TAMAGUCCI::FC_HAMBURGER:
			case TAMAGUCCI::FC_PORK:

				ss << "SALAD";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 80, 59.5);
				ss.str("");
				ss << "HAMBURGER";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 80, 32.5);
				ss.str("");
				ss << "PORK";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 80, 7.5);
				break;
			case TAMAGUCCI::FC_BACK:
				ss << "HAMBURGER";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 80, 59.5);
				ss.str("");
				ss << "PORK";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 80, 32.5);
				ss.str("");
				ss << "BACK";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 80, 7.5);
				break;
			}
			//RENDER OPTION SEEKER
			switch (tamagucci.getFoodChoice())
			{
			case TAMAGUCCI::FC_SALAD:
				arrowPos.Set(36.5, 59.5);
				if (tamagucci.GetShowFood())
				{
					Render2DMeshWScale(meshList[GEO_SALAD], false, tamagucci.GetTamFood()->scale.x, tamagucci.GetTamFood()->scale.y, tamagucci.GetTamFood()->position.x, tamagucci.GetTamFood()->position.y, false);
					Render2DMeshWScale(meshList[GEO_TAMFOOD], false, tamagucci.GetTamTam()->scale.x, tamagucci.GetTamTam()->scale.y, tamagucci.GetTamTam()->position.x + (tamagucci.GetTamTam()->scale.x * 0.5), tamagucci.GetTamTam()->position.y + (tamagucci.GetTamTam()->scale.y * 0.5), tamagucci.GetMoveLeft());
				}
				break;
			case TAMAGUCCI::FC_HAMBURGER:
				arrowPos.Set(36.5, 32.5);
				if (tamagucci.GetShowFood())
				{
					Render2DMeshWScale(meshList[GEO_HAMGURGER], false, tamagucci.GetTamFood()->scale.x, tamagucci.GetTamFood()->scale.y, tamagucci.GetTamFood()->position.x, tamagucci.GetTamFood()->position.y, false);
					Render2DMeshWScale(meshList[GEO_TAMFOOD], false, tamagucci.GetTamTam()->scale.x, tamagucci.GetTamTam()->scale.y, tamagucci.GetTamTam()->position.x + (tamagucci.GetTamTam()->scale.x * 0.5), tamagucci.GetTamTam()->position.y + (tamagucci.GetTamTam()->scale.y * 0.5), tamagucci.GetMoveLeft());
				}
				break;
			case TAMAGUCCI::FC_PORK:
				if (tamagucci.GetShowFood())
				{
					Render2DMeshWScale(meshList[GEO_PORK], false, tamagucci.GetTamFood()->scale.x, tamagucci.GetTamFood()->scale.y, tamagucci.GetTamFood()->position.x, tamagucci.GetTamFood()->position.y, false);
					Render2DMeshWScale(meshList[GEO_TAMFOOD], false, tamagucci.GetTamTam()->scale.x, tamagucci.GetTamTam()->scale.y, tamagucci.GetTamTam()->position.x + (tamagucci.GetTamTam()->scale.x * 0.5), tamagucci.GetTamTam()->position.y + (tamagucci.GetTamTam()->scale.y * 0.5), tamagucci.GetMoveLeft());
				}
				break;
			case TAMAGUCCI::FC_BACK:
				arrowPos.Set(36.5, 7.5);
				break;
			}
			Render2DMeshWScale(meshList[GEO_BATTLEARROW], false, 0.05, 0.03, arrowPos.x, arrowPos.y, false);
			break;
		case TAMAGUCCI::SLEEP:
		{
			renderFirstTamagotchiFirstMenu(0.f);
			if (tamagucci.GetSleep())
			{
				RenderBackground(meshList[GEO_BLACK]);
				Render2DMeshWScale(meshList[GEO_TAMSLEEP], false, 100, 100, tamagucci.GetTamTam()->position.x + 60, tamagucci.GetTamTam()->position.y + 70, false);
			}
			else
				arrowPos.Set(170, 60);
			Render2DMeshWScale(meshList[GEO_BATTLEARROW], false, 0.05, 0.03, arrowPos.x, arrowPos.y, false);
			renderTamagotchiUI();
		}
		break;
		case TAMAGUCCI::ENTERTAINMENT:
			//RENDER MENU AND OPTIONS
			renderFirstTamagotchiFirstMenu(27.5f);
			ss << "REACH FOR THE STARS";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 360, 59.5);
			ss.str("");
			ss << "ROCK PAPER SCISSORS";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 360, 32.5);
			ss.str("");
			ss << "BACK";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 360, 7.5);

			//RENDER OPTION SEEKER
			switch (tamagucci.getGameChoice())
			{
			case TAMAGUCCI::CATCHING:
				arrowPos.Set(314.5, 57.5);
				break;
			case TAMAGUCCI::ROCKPAPERSCISSORS:
				arrowPos.Set(314.5, 30.5);
				break;
			case TAMAGUCCI::G_BACK:
				arrowPos.Set(314.5, 5.5);
				break;
			}
			Render2DMeshWScale(meshList[GEO_BATTLEARROW], false, 0.05, 0.03, arrowPos.x, arrowPos.y, false);
			break;
		}
		break;
	}
}

void Scene2::renderTamagotchiGame()
{
	ostringstream ss;
	switch (tamagucci.getGameChoice())
	{
	case TAMAGUCCI::CATCHING:
		RenderBackground(meshList[GEO_TAMBG1]);
		Render2DMeshWScale(meshList[GEO_STAR], false, tamagucci.GetTamDrop()->scale.x, tamagucci.GetTamDrop()->scale.y, tamagucci.GetTamDrop()->position.x, tamagucci.GetTamDrop()->position.y, false);
		Render2DMeshWScale(meshList[GEO_POOP], false, tamagucci.GetTamDrop2()->scale.x, tamagucci.GetTamDrop2()->scale.y, tamagucci.GetTamDrop2()->position.x, tamagucci.GetTamDrop2()->position.y, false);
		renderTamagotchiUI();
		ss << "Score: " << tamagucci.GetScore() << "/ 20";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 50, 520);
		break;
	}
}

void Scene2::renderTamagotchiUI()
{
	Render2DMeshWScale(meshList[GEO_TAMAGUCCIUIBACKGROUND], false, 1, 1, 400, -291, false);
	Render2DMeshWScale(meshList[GEO_TAMAGUCCIUIBACKGROUND], false, 1, 1, 400, 892, false);
	if (SharedData::GetInstance()->inventory.getRightArm())
	{
		Render2DMeshWScale(meshList[GEO_GREEN], false, 60, SharedData::GetInstance()->inventory.getRightArm()->GetTamHunger() * 12, 549, 516.5, false);
		Render2DMeshWScale(meshList[GEO_GREEN], false, 60, SharedData::GetInstance()->inventory.getRightArm()->GetTamEnergy() * 12, 630, 516.5, false);
		Render2DMeshWScale(meshList[GEO_GREEN], false, 60, SharedData::GetInstance()->inventory.getRightArm()->GetTamHappy() * 12, 717, 516.5, false);
	}
	Render2DMeshWScale(meshList[GEO_HUNGERFRAME], false, 60, 60, 549, 516.5, false);
	Render2DMeshWScale(meshList[GEO_ENERGYFRAME], false, 60, 60, 630, 516.5, false);
	Render2DMeshWScale(meshList[GEO_HAPPINESSFRAME], false, 60, 60, 717, 516.5, false);
}

void Scene2::RenderTamagucci()
{
	//RenderBackground(meshList[GEO_TAMAGUCCIBACKGROUND]);
	RenderBackground(meshList[GEO_TAMLIVINGROOM]);
	renderTamagotchiUI();
	ostringstream ss;

	switch (tamagucci.getTamagotchiState())
	{
	case TAMAGUCCI::MENU:
		renderTamagotchiMenu();
		break;
	case TAMAGUCCI::GAME:
		renderTamagotchiGame();
		break;
	}
	if (SharedData::GetInstance()->inventory.getRightArm())
	{
		RenderTextOnScreen(meshList[GEO_TEXT], SharedData::GetInstance()->inventory.getRightArm()->getName(), Color(1, 1, 1), 30, 100, 500);
		if (tamagucci.GetScore() < 20 && !tamagucci.GetShowFood() && !tamagucci.GetSleep())
			Render2DMeshWScale(meshList[GEO_TAMAGUCCI], false, tamagucci.GetTamTam()->scale.x, tamagucci.GetTamTam()->scale.y, tamagucci.GetTamTam()->position.x, tamagucci.GetTamTam()->position.y, false);
		if (tamagucci.GetScore() >= 20)
			Render2DMeshWScale(meshList[GEO_TAMHAPPY], false, tamagucci.GetTamTam()->scale.x, tamagucci.GetTamTam()->scale.y, tamagucci.GetTamTam()->position.x + (tamagucci.GetTamTam()->scale.x * 0.5), tamagucci.GetTamTam()->position.y + (tamagucci.GetTamTam()->scale.y * 0.5), false);
	}
}

void Scene2::RenderCatch()
{
	RenderBackground(meshList[GEO_BATTLESCENE]);
	RenderMonster();

	Render2DMeshWScale(meshList[GEO_RED], false, redbar->scale.x, redbar->scale.y, redbar->position.x, redbar->position.y, false);
	Render2DMeshWScale(meshList[GEO_GREEN], false, greenbar->scale.x, greenbar->scale.y, greenbar->position.x, greenbar->position.y, false);
	Render2DMeshWScale(meshList[GEO_BAR], false, chargebar->scale.x, chargebar->scale.y, chargebar->position.x, chargebar->position.y, false);
}

void Scene2::RenderBattleDialogue()
{
	//Player Attack Enemy Dialogue
	if (SharedData::GetInstance()->playerBattleDialogue && battleScene.GetBattleSelection() == BattleSystem::BS_SLASH ||
		SharedData::GetInstance()->playerBattleDialogue && battleScene.GetBattleSelection() == BattleSystem::BS_STAB ||
		SharedData::GetInstance()->playerBattleDialogue && battleScene.GetBattleSelection() == BattleSystem::BS_SKILL)
	{
		Render2DMeshWScale(meshList[GEO_BATTLEDIALOUGEBACKGROUND], false, 1, 0.3, 0, 0, false);

		if (battleScene.GetBattleSelection() == BattleSystem::BS_SLASH)
		{
			std::ostringstream ss;
			ss.str("");
			ss.precision(5);
			ss << "You Slash Enemy for " << theHero->GetDMG() << ", Enemy HP left " << EnemyInBattle->GetHealth();

			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 100);

			if (flashEffect)
			{
				ss.str("");
				ss.precision(5);
				ss << "Press enter to continue ";

				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 15, 300, 50);
			}
		}

		if (battleScene.GetBattleSelection() == BattleSystem::BS_STAB)
		{
			std::ostringstream ss;
			ss.str("");
			ss.precision(5);
			ss << "You stab Enemy for " << theHero->GetDMG() << ", Enemy HP left " << EnemyInBattle->GetHealth();

			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 100);

			if (flashEffect)
			{
				ss.str("");
				ss.precision(5);
				ss << "Press enter to continue ";

				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 15, 300, 50);
			}

		}
		if (battleScene.GetBattleSelection() == BattleSystem::BS_SKILL)
		{
			std::ostringstream ss;
			ss.str("");
			ss.precision(5);

			switch (SharedData::GetInstance()->inventory.getArmour()->getMonster().GetType())
			{
			case 0:
			case 1:
			case 2:
				ss << "You damage Enemy for " << theHero->GetDMG() << ", Enemy HP left " << EnemyInBattle->GetHealth();
				break;
			case 3:
				ss << "defend increased!!";
				break;
			}

			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 100);

			if (flashEffect)
			{
				ss.str("");
				ss.precision(5);
				ss << "Press enter to continue ";

				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 15, 300, 50);
			}

		}
	}
	else if (SharedData::GetInstance()->playerBattleDialogue && battleScene.GetBattleSelection() == BattleSystem::BS_CAPTURE ||
		SharedData::GetInstance()->playerBattleDialogue && battleScene.GetBattleSelection() == BattleSystem::BS_RUN)
	{
		Render2DMeshWScale(meshList[GEO_BATTLEDIALOUGEBACKGROUND], false, 1, 0.3, 0, 0, false);

		if (battleScene.GetBattleSelection() == BattleSystem::BS_CAPTURE)
		{
			std::ostringstream ss;
			ss.str("");
			ss.precision(5);
			ss << "You failed to capture the enemy!";

			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 100);

			if (flashEffect)
			{
				ss.str("");
				ss.precision(5);
				ss << "Press enter to continue ";

				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 15, 300, 50);
			}
		}

		if (battleScene.GetBattleSelection() == BattleSystem::BS_RUN)
		{
			std::ostringstream ss;
			ss.str("");
			ss.precision(5);
			ss << "You failed to escape!";

			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 200, 100);

			if (flashEffect)
			{
				ss.str("");
				ss.precision(5);
				ss << "Press enter to continue ";

				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 15, 300, 50);
			}

		}
	}
}

void Scene2::RenderBattleAnimation()
{
	if (SharedData::GetInstance()->BS_SlashRender)
		Render2DMeshWScale(meshList[GEO_SLASHANIMATION], false, 200.0f, 200.0f, battleMonsterPos.x + 120.f, battleMonsterPos.y + 80.0f, false);
	if (SharedData::GetInstance()->BS_StabRender)
		Render2DMeshWScale(meshList[GEO_STABANIMATION], false, 200.0f, 200.0f, battleMonsterPos.x + 120.0f, battleMonsterPos.y + 80.0f, false);
}

void Scene2::RenderBattleHUD()
{
	//Rendering HP & MP bar and Background
	Render2DMeshWScale(meshList[GEO_HPBARDESIGN], false, 0.5f, 0.1f, 0, 540, false);
	Render2DMeshWScale(meshList[GEO_HPBAR], false, maxHpScale * (renderedHp / 100), 1.0f, hpPos.x, hpPos.y, false);
	Render2DMeshWScale(meshList[GEO_MPBAR], false, maxMpScale * (renderedMp / 100), 1.0f, mpPos.x, mpPos.y, false);

	//Rendering Selection Arrow based on selection
	if (GS == BATTLE)
	{
		switch (battleScene.GetBattleSelection())
		{
		case BattleSystem::BATTLE_SELECTION::BS_SLASH:
		case BattleSystem::BATTLE_SELECTION::BS_ATTACK:
			battleScene.SetArrowPos(120, 91, 0);
			break;
		case BattleSystem::BATTLE_SELECTION::BS_SKILL:
		case BattleSystem::BATTLE_SELECTION::BS_CAPTURE:
			battleScene.SetArrowPos(120, 41, 0);
			break;
		case BattleSystem::BATTLE_SELECTION::BS_STAB:
		case BattleSystem::BATTLE_SELECTION::BS_ITEM:
			battleScene.SetArrowPos(420, 91, 0);
			break;
		case BattleSystem::BATTLE_SELECTION::BS_BACK:
		case BattleSystem::BATTLE_SELECTION::BS_RUN:
			battleScene.SetArrowPos(420, 41, 0);
			break;
		}
	}
	//When it is player's turn
	if (SharedData::GetInstance()->playerTurn &&
		!SharedData::GetInstance()->enemyTurn &&
		!SharedData::GetInstance()->BS_SlashRender &&
		!SharedData::GetInstance()->BS_StabRender &&
		!SharedData::GetInstance()->playerBattleDialogue)
	{
		Render2DMeshWScale(meshList[GEO_BATTLEDIALOUGEBACKGROUND], false, 1.5, 0.3, -50, 0, false);

		Render2DMeshWScale(meshList[GEO_BATTLEARROW], false, 0.1, 0.05, battleScene.GetArrowPosX(), battleScene.GetArrowPosY(), false, 2);

		std::ostringstream ss;
		ss.str("");
		ss.precision(5);
		if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice())
			ss << "Attack";
		else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice())
			ss << "Slash";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 200, 100);

		ss.str("");
		ss.precision(5);
		if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice())
			ss << "Item";
		else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice())
			ss << "Stab";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 500, 100);

		ss.str("");
		ss.precision(5);
		if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice())
			ss << "Capture";
		else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice())
		{
			if (SharedData::GetInstance()->inventory.getArmour() != NULL)
			{
				switch (SharedData::GetInstance()->inventory.getArmour()->getMonster().GetType())
				{
				case 0:
					ss << "Banshee Scream";
					break;
				case 1:
					ss << "Cerebus Bite";
					break;
				case 2:
					ss << "Dragon Roar";
					break;
				case 3:
					ss << "GOLEM SKIN";
					break;
				}
			}
			else
			{
				ss << "No skill Equided";
			}
		}
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 200, 50);

		ss.str("");
		ss.precision(5);
		if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice())
			ss << "Run";
		else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice())
			ss << "Back";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 500, 50);

		ss.str("");
		ss << "Capture Rate : " << enemyCatchPercentage << "%";
		ss.precision(5);
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 580, 180);
	}

}

void Scene2::RenderBattleScene()
{
	RenderBackground(meshList[GEO_BATTLESCENE]);
	RenderMonster();
	RenderBattleAnimation();
	RenderBattleHUD();
	RenderBattleDialogue();
}

void Scene2::Render()
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
		RenderTestMap();
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
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void Scene2::RenderTileMap(Mesh* mesh, CMap* map, Vector3 speed)
{
	for (int y = 0; y < map->theNumOfTiles_Height; ++y)
	{
		for (int x = 0; x < map->theNumOfTiles_Width; ++x)
		{
			//if (map->theMap[y][x].BlockID != 0)
			{
				RenderTile(mesh, map->theMap[y][x].BlockID, 32, x*map->GetTileSize() - (theHero->GetMapOffset().x * speed.x), y*map->GetTileSize() - (theHero->GetMapOffset().y* speed.y));
			}
		}
	}
}