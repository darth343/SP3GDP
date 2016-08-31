#include "SceneGame.h"
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

SceneGame::SceneGame()
: EnemyInBattle(NULL)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	SceneBase::Init();
	Monster::InitDatabase();
	Equipment::InitDatabase();
	//Init GameState Here for testing purposes
	GS = MAP;
	MS = PLAY;
	capturedMonster = false;
	redbar = new Gauge(Vector3(500.f, 32.f, 1));
	redbar->type = GameObject::GO_REDBAR;
	redbar->gauge = Gauge::GREENBAR;
	redbar->position.Set(150, 150, 1);

	greenbar= new Gauge(Vector3(0.f, 32.f, 1));
	greenbar->gauge = Gauge::GREENBAR;
	greenbar->type = GameObject::GO_GREENBAR;
	greenbar->position.Set(400, 150, 1);

	chargebar = new Gauge(Vector3(1.f, 32.f, 1));
	chargebar->gauge = Gauge::MOVE;
	chargebar->type = GameObject::GO_MOVE;
	chargebar->position.Set(500, 150, 1);

	/*teleporter1 = new GameObject(Vector3(20.f, 20.f, 1));
	teleporter1->position.Set(182, 154, 1);
	teleporter1->type = GameObject::GO_TELEPORT;
	m_goList.push_back(teleporter1);

	teleporter2 = new GameObject(Vector3(20.f, 20.f, 1));
	teleporter2->position.Set(182, 510, 1);
	teleporter2->type = GameObject::GO_TELEPORT2;
	m_goList.push_back(teleporter2);*/
	// Initialise the hero's position
	SharedData::GetInstance()->player->SetHP(100);
	SharedData::GetInstance()->player->SetPosition(Vector3(530, 64, 0));
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
	renderedMp = 0;
	renderedHp = 0;
}

bool SceneGame::GetMonsterScaleUp()
{
	return monsterScaleUp;
}

void SceneGame::SetMonsterScaleUp(bool set)
{
	this->monsterScaleUp = set;
}

float SceneGame::GetBattleMonsterScaleX()
{
	return battleMonsterScale.x;
}

void SceneGame::SetBattleMonsterScaleX(float x)
{
	this->battleMonsterScale.x = x;
}

float SceneGame::GetBattleMonsterScaleY()
{
	return battleMonsterScale.y;
}

void SceneGame::SetBattleMonsterScaleY(float y)
{
	this->battleMonsterScale.y = y;
}

float SceneGame::GetBattleMonsterPosX()
{
	return battleMonsterPos.x;
}

float SceneGame::GetBattleMonsterPosY()
{
	return battleMonsterPos.y;
}

void SceneGame::SetBattleMonsterPosX(float x)
{
	this->battleMonsterPos.x = x;
}

void SceneGame::SetBattleMonsterPosY(float y)
{
	this->battleMonsterPos.y = y;
}

void SceneGame::SetGS(string set)
{
	if (set == "START_SCREEN")
		GS = START_SCREEN;
	else if (set == "MAP")
		GS = MAP;
	else if (set == "INVENTORY_SCREEN")
		GS = INVENTORY_SCREEN;
	else if (set == "TAMAGUCCI_SCREEN")
		GS = TAMAGUCCI_SCREEN;
	else if (set == "BATTLE")
		GS = BATTLE;
	else if (set == "CATCH")
		GS = CATCH;
}

void SceneGame::CatchUpdate(double dt)
{
	SharedData::GetInstance()->enemyCatchPercentage = (EnemyInBattle->GetMaxHealth() - EnemyInBattle->GetHealth()) / EnemyInBattle->GetMaxHealth() * 100 + SharedData::GetInstance()->trapPercentageIncrease;

	float prevScale = greenbar->scale.x;
	greenbar->scale.x = SharedData::GetInstance()->enemyCatchPercentage * 0.01 * redbar->scale.x; // * 0.01 is the same as divide by 100
	if (greenbar->scale.x > prevScale)
	{
		greenbar->position.x -= (greenbar->scale.x - prevScale) * 0.5;
	}

	chargebar->Update(dt, 200.f);
	greenbar->Update(dt, 800.f);
	if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
	{
		SharedData::GetInstance()->ENTERkeyPressed = true;
		if (chargebar->CheckCollision(greenbar))
		{
			cout << "CAPTURED" << endl;

			if (SharedData::GetInstance()->gameState == SharedData::GAME_S1)
				SharedData::GetInstance()->capturedBanshee = true;

			else if (SharedData::GetInstance()->gameState == SharedData::GAME_S2)
				SharedData::GetInstance()->capturedGolem = true;

			else if (SharedData::GetInstance()->gameState == SharedData::GAME_S3)
				SharedData::GetInstance()->capturedCerebus = true;

			else if (SharedData::GetInstance()->gameState == SharedData::GAME_S4)
				SharedData::GetInstance()->capturedDragon = true;

			capturedMonster = true;
			currState = 3;
			SharedData::GetInstance()->inventory.addToInventory(EnemyInBattle);
			battleScene.SetBattleSelection(BattleSystem::BS_ATTACK);
			battleScene.SetFirstChoice(true);
			battleScene.SetSecondChoice(false);
			SharedData::GetInstance()->playerTurn = true;
			SharedData::GetInstance()->enemyTurn = false;
			RemoveEnemy();
			SharedData::GetInstance()->soundManager.StopAllSound();
			GS = MAP;
			return;
		}
		else if (!chargebar->CheckCollision(greenbar))
		{
			SharedData::GetInstance()->playerBattleDialogue = true;
			GS = BATTLE;
		}
	}
	else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
	{
		SharedData::GetInstance()->ENTERkeyPressed = false;
	}
}

void SceneGame::EnterBattleScene(Enemy* enemy)
{
	battleScene.SetBattleStart(true);
	SharedData::GetInstance()->soundManager.StopAllSound();
	SharedData::GetInstance()->soundManager.SoundPlay("Sound/battleStart.mp3", &SharedData::GetInstance()->battleStart, 0.3f, true);
	renderedHp = 0;
	renderedMp = 0;
	cout << "Battle" << endl;
	EnemyInBattle = enemy;
	GS = BATTLE;
}

//For the mini game
void SceneGame::TamagucciUpdate(double dt)
{
	SpriteAnimation *tamhappy = dynamic_cast<SpriteAnimation*>(meshList[GEO_TAMHAPPY]);
	if (tamhappy)
	{
		tamhappy->Update(dt);
		tamhappy->m_anim->animActive = true;
	}
	if (SharedData::GetInstance()->SharedData::GetInstance()->tamagucci.GetShowFood())
	{
		SpriteAnimation *tamfood = dynamic_cast<SpriteAnimation*>(meshList[GEO_TAMFOOD]);
		if (tamfood)
		{
			tamfood->Update(dt);
			tamfood->m_anim->animActive = true;
		}
	}
	SpriteAnimation *sleep = dynamic_cast<SpriteAnimation*>(meshList[GEO_TAMSLEEP]);
	if (sleep && SharedData::GetInstance()->SharedData::GetInstance()->tamagucci.GetSleep())
	{
		sleep->Update(dt);
		sleep->m_anim->animActive = true;
	}
	SpriteAnimation *salad = dynamic_cast<SpriteAnimation*>(meshList[GEO_SALAD]);

		foodAnimOver = false;
		if (salad && SharedData::GetInstance()->tamagucci.GetTouchedFood() && salad->m_anim->animActive)
		{
			salad->Update(dt);
		}
		else if (!salad->m_anim->animActive)
		{
			foodAnimOver = true;
			salad->m_anim->animActive = true;
		}
		SharedData::GetInstance()->tamagucci.SetAnimationOver(foodAnimOver);
}

void SceneGame::UpdateInventory(double dt)
{
	if (Application::IsKeyPressed('I') && !SharedData::GetInstance()->IkeyPressed)
	{
		GS = MAP;
		SharedData::GetInstance()->IkeyPressed = true;
	}
	else if (!Application::IsKeyPressed('I') && SharedData::GetInstance()->IkeyPressed)
	{
		SharedData::GetInstance()->IkeyPressed = false;
	}

	SharedData::GetInstance()->inventory.Update(dt);
}

void SceneGame::renderInventoryItems()
{
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
		Render2DMeshWScale(meshList[GEO_CHESTPLATE], false, 44.5, 44.5, 474, 361.5, false);
	}

	if (SharedData::GetInstance()->inventory.getRightArm())
	{
		if (SharedData::GetInstance()->inventory.getRightArm()->getType() == Equipment::SWORD)
		{
			Render2DMeshWScale(meshList[GEO_SWORD], false, 44.5, 44.5, 520,359.5, false);
		}
		else if (SharedData::GetInstance()->inventory.getRightArm()->getType() == Equipment::SHIELD)
		{
			Render2DMeshWScale(meshList[GEO_SHIELD], false, 44.5, 44.5, 520, 359.5, false);
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

	ss.str("");
	ss << "Potions:          " << SharedData::GetInstance()->inventory.GetPotionCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 43.5, 180 + 22);
	ss.str("");
	ss << "Traps:            " << SharedData::GetInstance()->inventory.GetPotionCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 43.5, 180 - 22.5 + 22);
	ss.str("");
	ss << "Encrypted Memory: " << SharedData::GetInstance()->inventory.GetMemoryCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 43.5, 180 - 45 + 22);
}

void SceneGame::renderInventoryMenus()
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

	switch (SharedData::GetInstance()->inventory.getState())
	{
	case Inventory::TAB1:
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
			ss.str("");
			ss << "AVG: " << (SharedData::GetInstance()->inventory.getEquipmentLookAt()->getDefense() + SharedData::GetInstance()->inventory.getEquipmentLookAt()->getDamage()) * 0.5 << endl;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 562.5, 392 - 60);
		}

		if (SharedData::GetInstance()->inventory.getInputState() == Inventory::INVENTORY)
			Render2DMesh(meshList[GEO_INVENTORYSEEKER], false, 1, 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46, 254 + SharedData::GetInstance()->inventory.getSeeker().y * 46);
		if (SharedData::GetInstance()->inventory.getInputState() == Inventory::EQUIP_OPTIONS)
		{ 
			if (SharedData::GetInstance()->inventory.getSeeker().x < 5)
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
			else if(SharedData::GetInstance()->inventory.getSeeker().x >= 5)
			{
				Render2DMeshWScale(meshList[GEO_INVENTORYSECONDBACKGROUND], false, 122, -44 - (float)(SharedData::GetInstance()->inventory.getOptions().size() * 18), 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46 + 36 - 159, 290 + SharedData::GetInstance()->inventory.getSeeker().y * 46);
				for (int i = 0; i < SharedData::GetInstance()->inventory.getOptions().size(); ++i)
				{
					std::ostringstream ss;
					ss.str("");
					ss << SharedData::GetInstance()->inventory.getOptions()[i];
					RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 25, 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46 + 46 - 159, 254 + SharedData::GetInstance()->inventory.getSeeker().y * 46 - (i * 20));
				}
				std::ostringstream ss;
				ss.str("");
				ss << SharedData::GetInstance()->inventory.getOptions()[SharedData::GetInstance()->inventory.getSecondSeeker()];
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46 + 46 - 159, 254 + SharedData::GetInstance()->inventory.getSeeker().y * 46 - (SharedData::GetInstance()->inventory.getSecondSeeker() * 20));
			}
		}
		break;
	case Inventory::TAB2:
		std::ostringstream ss;
		if (SharedData::GetInstance()->inventory.getSeeker().y == Items::POTION)
		{
			ss << "Potions:          " << SharedData::GetInstance()->inventory.GetPotionCount();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 43.5, 180 + 22);
		}
		if (SharedData::GetInstance()->inventory.getSeeker().y == Items::TRAP)
		{
			ss << "Traps:            " << SharedData::GetInstance()->inventory.GetPotionCount();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 43.5, 180 - 22.5 + 22);
		}
		if (SharedData::GetInstance()->inventory.getSeeker().y == Items::ENCRYPTED_MEMORY)
		{
			ss << "Encrypted Memory: " << SharedData::GetInstance()->inventory.GetPotionCount();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 43.5, 180 - 45+ 22);
		}
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
			ss.str("");
			ss << SharedData::GetInstance()->inventory.getOptions()[SharedData::GetInstance()->inventory.getSecondSeeker()];
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 25, 426.5 + SharedData::GetInstance()->inventory.getSeeker().x * 46 + 46, 254 + SharedData::GetInstance()->inventory.getSeeker().y * 46 - (SharedData::GetInstance()->inventory.getSecondSeeker() * 20));
		}
		break;
	}
//	cout << xpos << " " << ypos << endl;
}

void SceneGame::RenderInventory()
{
	RenderBackground(meshList[GEO_INVENTORYBACKGROUND]);
	renderInventoryItems();
	renderInventoryMenus();
}

void SceneGame::RenderItemScreen()
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

void SceneGame::ItemScreenUpdate(double dt)
{
	if (Application::IsKeyPressed('I') && !SharedData::GetInstance()->IkeyPressed)
	{
		GS = MAP;
		SharedData::GetInstance()->IkeyPressed = true;
	}
	else if (!Application::IsKeyPressed('I') && SharedData::GetInstance()->IkeyPressed)
	{
		SharedData::GetInstance()->IkeyPressed = false;
	}

	SharedData::GetInstance()->inventory.Update(dt);
}

void SceneGame::RenderEquipScreen()
{
	std::ostringstream ss;
	ss.str("");
	ss.precision(5);
	ss << "Equips: " << endl;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 60, 300, 450);

}

void SceneGame::Update(double dt)
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
		{
			SharedData::GetInstance()->tamagucci.ResetTamagotchi();
			GS = MAP;
		}
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
	case BATTLE:

		//Updating of catch percentage
		if (EnemyInBattle->GetHealth() > 0)
		SharedData::GetInstance()->enemyCatchPercentage = (EnemyInBattle->GetMaxHealth() - EnemyInBattle->GetHealth()) / EnemyInBattle->GetMaxHealth() * 100 + SharedData::GetInstance()->trapPercentageIncrease;;

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
		if (SharedData::GetInstance()->player->GetHP() < renderedHp)
		{
			renderedHp -= dt * 100;

			if (SharedData::GetInstance()->player->GetHP() > renderedHp)
			{
				renderedHp = SharedData::GetInstance()->player->GetHP();
			}

			//if enemy not dead		
			if (EnemyInBattle->GetHealth() <= 0)
			{
				SceneGame* mainScene = (SceneGame*)Application::GetInstance().GetScene();

				//Player win
				battleScene.Reset();
				mainScene->RemoveEnemy();

				if (SharedData::GetInstance()->gameState == SharedData::GAME_BOSS)
					GS = WIN;
				//destory enemy here
			}
			else if (renderedHp < 0.0f)
			{
				if (SharedData::GetInstance()->playerLives > 0)
				{
					SharedData::GetInstance()->playerLives--;
					SharedData::GetInstance()->player->SetPosition(Vector3(530, 64, 0));
					SharedData::GetInstance()->player->SetHP(100);
					renderedHp = 100;
					SharedData::GetInstance()->gameState = SharedData::GAME_S1;
					GS = MAP;
					cout << SharedData::GetInstance()->playerLives << endl;
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
				SharedData::GetInstance()->soundManager.StopSingleSound("Sound/battleStart.mp3");
				if (SharedData::GetInstance()->gameState != SharedData::GAME_BOSS)
				RemoveEnemy();
				//SceneGame* mainScene = (SceneGame*)Application::GetInstance().GetScene();

				//mainScene->RemoveEnemy();
			}

		}
		else if (SharedData::GetInstance()->player->GetHP() > renderedHp)
		{
			renderedHp += dt * 100;
			if (SharedData::GetInstance()->player->GetHP() < renderedHp)
			{
				renderedHp = SharedData::GetInstance()->player->GetHP();
			}
		}

		//Decrease RenderedMP on screen
		if (SharedData::GetInstance()->player->GetMP() < renderedMp)
		{
			renderedMp -= dt * 100;
			if (SharedData::GetInstance()->player->GetMP() > renderedMp)
			{
				renderedMp = SharedData::GetInstance()->player->GetMP();
			}
		}

		//Decrease RenderedMP on screen
		if (SharedData::GetInstance()->player->GetMP() > renderedMp)
		{
			renderedMp += dt * 100;
			if (SharedData::GetInstance()->player->GetMP() < renderedMp)
			{
				renderedMp = SharedData::GetInstance()->player->GetMP();
			}
		}

		battleScene.UpdateBattleSystem(SharedData::GetInstance()->UPkeyPressed, SharedData::GetInstance()->DNkeyPressed, SharedData::GetInstance()->LEFTkeyPressed, SharedData::GetInstance()->RIGHTkeyPressed, SharedData::GetInstance()->ENTERkeyPressed, SharedData::GetInstance()->player, EnemyInBattle);
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
		SharedData::GetInstance()->tamagucci.UpdateTamagucci(dt);
		TamagucciUpdate(dt);
		break;
	}
	fps = (float)(1.f / dt);
}

void SceneGame::RenderPlayer()
{
	Render2DMesh(SharedData::GetInstance()->player->GetPlayerMesh(), false, 32.0f, SharedData::GetInstance()->player->GetPosition().x, SharedData::GetInstance()->player->GetPosition().y, SharedData::GetInstance()->player->GetFlipStatus());
}

static bool touched = true;

void SceneGame::RemoveEnemy()
{
	for (int i = 0; i < m_goList.size(); ++i)
	{
		if (m_goList[i] == EnemyInBattle)
		{
			delete m_goList[i];
			m_goList.erase(m_goList.begin() + i);
		}
	}
	EnemyInBattle = NULL;
}

void SceneGame::RenderMonster()
{
	if (battleScene.GetMonsterHitAnimation())
		{
			if (monsterScaleUp)
			{
				battleMonsterScale.x += 3;
				battleMonsterScale.y += 3;
				battleMonsterPos.x -= 2.f;
				battleMonsterPos.y -= 2.f;

				if (battleMonsterScale.x > 330 || battleMonsterScale.y > 330)
					monsterScaleUp = false;
			}
			else if (!monsterScaleUp)
			{
				battleMonsterScale.x -= 3;
				battleMonsterScale.y -= 3;
				battleMonsterPos.x += 2.f;
				battleMonsterPos.y += 2.f;

				if (battleMonsterScale.x < 300 || battleMonsterScale.y < 300)
				{
					battleScene.SetMonsterHitAnimation(false);
					battleScene.SetFirstChoice(true);
					battleScene.SetSecondChoice(false);
					battleScene.SetBattleSelection(BattleSystem::BS_ATTACK);
					SharedData::GetInstance()->enemyTurn = false;
					SharedData::GetInstance()->playerTurn = true;
					//battleMonsterScale.x = 300.f;
					//battleMonsterScale.y = 300.f;
					battleMonsterPos.x = 280;
					battleMonsterPos.y = 240;
					monsterScaleUp = true;
				}
			}
		}
		if (SharedData::GetInstance()->gameState == SharedData::GAME_S1)
		{
		Render2DMeshWScale(meshList[GEO_MONSTERBANSHEE], false, battleMonsterScale.x, battleMonsterScale.y, battleMonsterPos.x, battleMonsterPos.y, false);

		}

	if (SharedData::GetInstance()->gameState == SharedData::GAME_S4)
	{
		Render2DMeshWScale(meshList[GEO_DRAGONDOWN], false, battleMonsterScale.x, battleMonsterScale.y, battleMonsterPos.x, battleMonsterPos.y, false);
	}
	if (SharedData::GetInstance()->gameState == SharedData::GAME_S2)
	{
		Render2DMeshWScale(meshList[GEO_MONSTER], false, battleMonsterScale.x, battleMonsterScale.y, battleMonsterPos.x, battleMonsterPos.y, false);
	}
	if (SharedData::GetInstance()->gameState == SharedData::GAME_BOSS)
	{
		Render2DMeshWScale(meshList[GEO_BOSS], false, battleMonsterScale.x, battleMonsterScale.y, battleMonsterPos.x, battleMonsterPos.y, false);
	}

	
}

void SceneGame::renderFirstTamagotchiFirstMenu(float yoffset)
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

void SceneGame::renderTamagotchiMenu()
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
	switch (SharedData::GetInstance()->tamagucci.getMenuState())
	{
	case TAMAGUCCI::FIRSTMENU:
		//RENDER OPTIONS
		renderFirstTamagotchiFirstMenu();
		//RENDER OPTION SEEKER
		switch (SharedData::GetInstance()->tamagucci.getFirstMenuOption())
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
		switch (SharedData::GetInstance()->tamagucci.getFirstMenuOption())
		{
		case TAMAGUCCI::FOOD:
			renderFirstTamagotchiFirstMenu(27.5f);
			//RENDER MENU
			switch (SharedData::GetInstance()->tamagucci.getFoodChoice())
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
			switch (SharedData::GetInstance()->tamagucci.getFoodChoice())
			{
			case TAMAGUCCI::FC_SALAD:
				arrowPos.Set(36.5, 59.5);
				if (SharedData::GetInstance()->tamagucci.GetShowFood())
				{
					Render2DMeshWScale(meshList[GEO_SALAD], false, SharedData::GetInstance()->tamagucci.GetTamFood()->scale.x, SharedData::GetInstance()->tamagucci.GetTamFood()->scale.y, SharedData::GetInstance()->tamagucci.GetTamFood()->position.x, SharedData::GetInstance()->tamagucci.GetTamFood()->position.y, false);
					Render2DMeshWScale(meshList[GEO_TAMFOOD], false, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.x, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.y, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.x, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.y, SharedData::GetInstance()->tamagucci.GetMoveLeft());
				}
				break;
			case TAMAGUCCI::FC_HAMBURGER:
				arrowPos.Set(36.5, 32.5);
				if (SharedData::GetInstance()->tamagucci.GetShowFood())
				{
					Render2DMeshWScale(meshList[GEO_HAMGURGER], false, SharedData::GetInstance()->tamagucci.GetTamFood()->scale.x, SharedData::GetInstance()->tamagucci.GetTamFood()->scale.y, SharedData::GetInstance()->tamagucci.GetTamFood()->position.x, SharedData::GetInstance()->tamagucci.GetTamFood()->position.y, false);
					Render2DMeshWScale(meshList[GEO_TAMFOOD], false, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.x, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.y, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.x, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.y, SharedData::GetInstance()->tamagucci.GetMoveLeft());
				}
				break;
			case TAMAGUCCI::FC_PORK:
				if (SharedData::GetInstance()->tamagucci.GetShowFood())
				{
					Render2DMeshWScale(meshList[GEO_PORK], false, SharedData::GetInstance()->tamagucci.GetTamFood()->scale.x, SharedData::GetInstance()->tamagucci.GetTamFood()->scale.y, SharedData::GetInstance()->tamagucci.GetTamFood()->position.x, SharedData::GetInstance()->tamagucci.GetTamFood()->position.y, false);
					Render2DMeshWScale(meshList[GEO_TAMFOOD], false, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.x, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.y, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.x, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.y, SharedData::GetInstance()->tamagucci.GetMoveLeft());
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
								 if (SharedData::GetInstance()->tamagucci.GetSleep())
								 {
									RenderBackground(meshList[GEO_BLACK]);
									Render2DMeshWScale(meshList[GEO_TAMSLEEP], false, 100, 100, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.x + 60, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.y + 70, false);
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
			switch (SharedData::GetInstance()->tamagucci.getGameChoice())
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
		case TAMAGUCCI::STATS:
		{
			ss.str("");
			ss << "Press Enter to go back";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 284, 49);
		}
			break;
		}
		break;
	}

	if (SharedData::GetInstance()->tamagucci.getCurrentTama())
	for (int i = 0; i < SharedData::GetInstance()->tamagucci.getCurrentTama()->pooPositions.size(); ++i)
	{
		Render2DMeshWScale(meshList[GEO_POOP], false, 32, 32, SharedData::GetInstance()->tamagucci.getCurrentTama()->pooPositions[i].x, SharedData::GetInstance()->tamagucci.getCurrentTama()->pooPositions[i].y);
	}
}

void SceneGame::renderTamagotchiGame()
{
	ostringstream ss;
	switch (SharedData::GetInstance()->tamagucci.getGameChoice())
	{
	case TAMAGUCCI::CATCHING:
		RenderBackground(meshList[GEO_TAMBG1]);
		Render2DMeshWScale(meshList[GEO_STAR], false, SharedData::GetInstance()->tamagucci.GetTamDrop()->scale.x, SharedData::GetInstance()->tamagucci.GetTamDrop()->scale.y, SharedData::GetInstance()->tamagucci.GetTamDrop()->position.x, SharedData::GetInstance()->tamagucci.GetTamDrop()->position.y, false);
		Render2DMeshWScale(meshList[GEO_POOP], false, SharedData::GetInstance()->tamagucci.GetTamDrop2()->scale.x, SharedData::GetInstance()->tamagucci.GetTamDrop2()->scale.y, SharedData::GetInstance()->tamagucci.GetTamDrop2()->position.x, SharedData::GetInstance()->tamagucci.GetTamDrop2()->position.y, false);
		renderTamagotchiUI();
		ss << "Score: " << SharedData::GetInstance()->tamagucci.GetScore() << "/ 20";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 50, 520);
		break;
	}
}

void SceneGame::renderTamagotchiUI()
{
	Render2DMeshWScale(meshList[GEO_TAMAGUCCIUIBACKGROUND], false, 1, 1, 400, -291, false);
	Render2DMeshWScale(meshList[GEO_TAMAGUCCIUIBACKGROUND], false, 1, 1, 400, 892, false);
	if (SharedData::GetInstance()->tamagucci.getCurrentTama())
	{
		Render2DMeshWScale(meshList[GEO_GREEN], false, 60, SharedData::GetInstance()->tamagucci.getCurrentTama()->GetTamHunger() * 12, 549, 516.5, false);
		Render2DMeshWScale(meshList[GEO_GREEN], false, 60, SharedData::GetInstance()->tamagucci.getCurrentTama()->GetTamEnergy() * 12, 630, 516.5, false);
		Render2DMeshWScale(meshList[GEO_GREEN], false, 60, SharedData::GetInstance()->tamagucci.getCurrentTama()->GetTamHappy() * 12, 717, 516.5, false);
	}
	Render2DMeshWScale(meshList[GEO_HUNGERFRAME], false, 60, 60, 549, 516.5, false);
	Render2DMeshWScale(meshList[GEO_ENERGYFRAME], false, 60, 60, 630, 516.5, false);
	Render2DMeshWScale(meshList[GEO_HAPPINESSFRAME], false, 60, 60, 717, 516.5, false);
}

void SceneGame::RenderTamagucci()
{
	//RenderBackground(meshList[GEO_TAMAGUCCIBACKGROUND]);
	RenderBackground(meshList[GEO_TAMLIVINGROOM]);
	renderTamagotchiUI();
	ostringstream ss;

	switch (SharedData::GetInstance()->tamagucci.getTamagotchiState())
	{
	case TAMAGUCCI::MENU:
		renderTamagotchiMenu();
		break;
	case TAMAGUCCI::GAME:
		renderTamagotchiGame();
		break;
	}
	if (SharedData::GetInstance()->tamagucci.getCurrentTama())
	{
		RenderTextOnScreen(meshList[GEO_TEXT], SharedData::GetInstance()->tamagucci.getCurrentTama()->getName(), Color(1, 1, 1), 30, 100, 500);
		if (SharedData::GetInstance()->tamagucci.GetScore() < 20 && !SharedData::GetInstance()->tamagucci.GetShowFood() && !SharedData::GetInstance()->tamagucci.GetSleep())
			Render2DMeshWScale(meshList[GEO_TAMAGUCCI], false, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.x, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.y, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.x, SharedData::GetInstance()->tamagucci.getCurrentTama()->position.y, false);
		if (SharedData::GetInstance()->tamagucci.GetScore() >= 20)
			Render2DMeshWScale(meshList[GEO_TAMHAPPY], false, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.x, SharedData::GetInstance()->tamagucci.GetTamTam()->scale.y, SharedData::GetInstance()->tamagucci.GetTamTam()->position.x + (SharedData::GetInstance()->tamagucci.GetTamTam()->scale.x * 0.5), SharedData::GetInstance()->tamagucci.GetTamTam()->position.y + (SharedData::GetInstance()->tamagucci.GetTamTam()->scale.y * 0.5), false);
	}
}

void SceneGame::RenderCatch()
{
	RenderBackground(meshList[GEO_BATTLESCENE]);
	RenderMonster();

	Render2DMeshWScale(meshList[GEO_RED], false, redbar->scale.x, redbar->scale.y, redbar->position.x, redbar->position.y, false);
	Render2DMeshWScale(meshList[GEO_GREEN], false, greenbar->scale.x, greenbar->scale.y, greenbar->position.x, greenbar->position.y, false);
	Render2DMeshWScale(meshList[GEO_BAR], false, chargebar->scale.x, chargebar->scale.y, chargebar->position.x, chargebar->position.y, false);
}

void SceneGame::RenderBattleDialogue()
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
			ss << "You Slash Enemy for " << SharedData::GetInstance()->player->GetDMG() << ", Enemy HP left " << EnemyInBattle->GetHealth();

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
			ss << "You stab Enemy for " << SharedData::GetInstance()->player->GetDMG() << ", Enemy HP left " << EnemyInBattle->GetHealth();

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
				ss << "You damage Enemy for " << SharedData::GetInstance()->player->GetDMG() << ", Enemy HP left " << EnemyInBattle->GetHealth();
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

void SceneGame::RenderBattleAnimation()
{
	if (SharedData::GetInstance()->BS_SlashRender)
		Render2DMeshWScale(meshList[GEO_SLASHANIMATION], false, 200.0f, 200.0f, battleMonsterPos.x, battleMonsterPos.y, false);
	if (SharedData::GetInstance()->BS_StabRender)
		Render2DMeshWScale(meshList[GEO_STABANIMATION], false, 200.0f, 200.0f, battleMonsterPos.x, battleMonsterPos.y, false);
	if (SharedData::GetInstance()->BS_BiteRender)
		Render2DMeshWScale(meshList[GEO_BITEANIMATION], false, 200.0f, 200.0f, battleMonsterPos.x, battleMonsterPos.y, false);
	if (SharedData::GetInstance()->BS_ScreamRender)
		Render2DMeshWScale(meshList[GEO_SCREAMANIMATION], false, 200.0f, 200.0f, battleMonsterPos.x, battleMonsterPos.y, false);
	if (SharedData::GetInstance()->BS_RoarRender)
		Render2DMeshWScale(meshList[GEO_ROARANIMATION], false, 200.0f, 200.0f, battleMonsterPos.x, battleMonsterPos.y, false);
	if (SharedData::GetInstance()->BS_SkinRender)
		Render2DMeshWScale(meshList[GEO_SKINANIMATION], false, 200.0f, 200.0f, battleMonsterPos.x, battleMonsterPos.y, false);
}

void SceneGame::RenderBattleHUD()
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
		case BattleSystem::BATTLE_SELECTION::BS_POTION:
		case BattleSystem::BATTLE_SELECTION::BS_SLASH:
		case BattleSystem::BATTLE_SELECTION::BS_ATTACK:
			battleScene.SetArrowPos(120, 91, 0);
			break;
		case BattleSystem::BATTLE_SELECTION::BS_TRAP:
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
		if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice() && !battleScene.GetOpenItemBag())
			ss << "Attack";
		else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice() && !battleScene.GetOpenItemBag())
			ss << "Slash";
		else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice() && battleScene.GetOpenItemBag())
			ss << "Potion x " << SharedData::GetInstance()->inventory.GetPotionCount();

		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 200, 100);

		ss.str("");
		ss.precision(5);
		if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice() && !battleScene.GetOpenItemBag())
			ss << "Item";
		else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice() && !battleScene.GetOpenItemBag())
			ss << "Stab";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 500, 100);

		ss.str("");
		ss.precision(5);
		if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice() && !battleScene.GetOpenItemBag())
			ss << "Capture";
		else if (battleScene.GetFirstChoice() && !battleScene.GetSecondChoice() && battleScene.GetOpenItemBag())
			ss << "Trap x " << SharedData::GetInstance()->inventory.GetTrapCount();
		else if (battleScene.GetSecondChoice() && !battleScene.GetFirstChoice() && !battleScene.GetOpenItemBag())
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
		ss << "Capture Rate : " << SharedData::GetInstance()->enemyCatchPercentage << "%";
		ss.precision(5);
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 25, 580, 180);
	}

}

void SceneGame::RenderBattleScene()
{
	RenderBackground(meshList[GEO_BATTLESCENE]);
	RenderMonster();
	RenderBattleAnimation();
	RenderBattleHUD();
	RenderBattleDialogue();
}

void SceneGame::Render()
{
	SceneBase::Render();
	// Check for which GameState we are in
}

void SceneGame::Exit()
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

void SceneGame::RenderTileMap(Mesh* mesh, CMap* map, Vector3 speed)
{
	for (int y = 0; y < map->theNumOfTiles_Height; ++y)
	{
		for (int x = 0; x < map->theNumOfTiles_Width; ++x)
		{
			//if (map->theMap[y][x].BlockID != 0)
			{
				RenderTile(mesh, map->theMap[y][x].BlockID, 32, x*map->GetTileSize() - (SharedData::GetInstance()->player->GetMapOffset().x * speed.x), y*map->GetTileSize() - (SharedData::GetInstance()->player->GetMapOffset().y* speed.y));
			}
		}
	}
}