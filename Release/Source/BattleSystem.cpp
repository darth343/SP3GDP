#include "BattleSystem.h"
#include "Enemy.h"
#include "Application.h"
BattleSystem::BattleSystem() :
battleStart(false),
firstChoice(true),
secondChoice(false),
arrowPos(120, 90, 0),
battleSelection(BS_ATTACK),
monsterHitAnimation(false), 
openItemBag(false)
{
}


BattleSystem::~BattleSystem()
{
}

void BattleSystem::SetBattleStart(bool set)
{
	this->battleStart = set;
}

void BattleSystem::SetArrowPos(float x, float y, float z)
{
	this->arrowPos.x = x;
	this->arrowPos.y = y;
	this->arrowPos.z = z;
}

void BattleSystem::SetFirstChoice(bool set)
{
	this->firstChoice = set;
}

void BattleSystem::SetSecondChoice(bool set)
{
	this->secondChoice = set;
}

void BattleSystem::SetEscapeChance(float percentage)
{
	this->escapePercentage = percentage;
}

void BattleSystem::SetBattleSelection(BATTLE_SELECTION selection)
{
	this->battleSelection = selection;
}

void BattleSystem::SetOpenItemBag(bool set)
{
	this->openItemBag = set;
}

bool BattleSystem::GetOpenItemBag()
{
	return openItemBag;
}

bool BattleSystem::GetBattleStart()
{
	return battleStart;
}

float BattleSystem::GetArrowPosX()
{
	return arrowPos.x;
}

float BattleSystem::GetArrowPosY()
{
	return arrowPos.y;
}

bool BattleSystem::GetFirstChoice()
{
	return firstChoice;
}

bool BattleSystem::GetSecondChoice()
{
	return secondChoice;
}

BattleSystem::BATTLE_SELECTION BattleSystem::GetBattleSelection()
{
	return battleSelection;
}

//new
void BattleSystem::EndPlayerTurn()
{
	SharedData::GetInstance()->enemyTurn = true;
	SharedData::GetInstance()->enemyHitPlayer = true;
	SharedData::GetInstance()->playerTurn = false;
}

void BattleSystem::EndEnemyTurn()
{
	SharedData::GetInstance()->enemyTurn = false;
	SharedData::GetInstance()->enemyHitPlayer = false;
	SharedData::GetInstance()->playerTurn = true;
}

void BattleSystem::ResetAnimation()
{
	SharedData::GetInstance()->BS_BiteRender = false;
	SharedData::GetInstance()->BS_SkinRender = false;
	SharedData::GetInstance()->BS_RoarRender = false;
	SharedData::GetInstance()->BS_ScreamRender = false;
	SharedData::GetInstance()->BS_SlashRender = false;
	SharedData::GetInstance()->BS_StabRender = false;
}

void BattleSystem::Reset()
{
	SharedData::GetInstance()->soundManager.StopSingleSound("Sound/battleStart.mp3");
	battleStart = false;
	SharedData::GetInstance()->enemyTurn = false;
	SharedData::GetInstance()->playerTurn = true;
	arrowPos = Vector3(120, 90, 0);
	firstChoice = true;
	secondChoice = false;
	escapePercentage = 25.0f;
	battleSelection = BS_ATTACK;
	SetMonsterHitAnimation(false);
	ResetAnimation();
}

void BattleSystem::SetMonsterHitAnimation(bool set)
{
	this->monsterHitAnimation = set;
}

bool BattleSystem::GetMonsterHitAnimation()
{
	return monsterHitAnimation;
}

void BattleSystem::RunBattleChoice(CPlayerInfo* theHero, Enemy* enemy)
{
	SceneGame * mainScene = (SceneGame*)Application::GetInstance().GetScene();

	if (firstChoice)
	{
		switch (battleSelection)
		{
		case BS_ATTACK:
			//Render Attack skills choices (Slash, Stab, (Monster's Skills), Back)
			battleSelection = BS_SLASH;
			firstChoice = false;
			secondChoice = true;
			break;

		case BS_ITEM:
			//Render Item bag design
			//Battle selection set to Item's
			firstChoice = false;
			secondChoice = true;
			SetOpenItemBag(true);
			battleSelection = BS_POTION;
			break;

		case BS_CAPTURE:
			//Start Capture function && render capture function
			SharedData::GetInstance()->playerBattleDialogue = false;
			mainScene->SetGS("CATCH");
			battleStart = false;
			break;


		case BS_RUN:
			escapePercentage += Math::RandFloatMinMax(0.0f, 50.0f);
			if (escapePercentage > 50.0f && SharedData::GetInstance()->gameState != SharedData::GAMESTATE::GAME_BOSS || 
				escapePercentage > 70.0f && SharedData::GetInstance()->gameState == SharedData::GAMESTATE::GAME_BOSS)
			{
				SharedData::GetInstance()->playerTurn = true;
				SharedData::GetInstance()->enemyTurn = false;
				escapePercentage = 25.0f;
				SetBattleSelection(BattleSystem::BS_ATTACK);
				SetFirstChoice(true);
				SetSecondChoice(false);
				if (SharedData::GetInstance()->soundPlay)
				{
					SharedData::GetInstance()->soundPlay = false;
					SharedData::GetInstance()->soundManager.stopMusic("Sound//battleStart.mp3");
				}

				battleSelection = BS_ATTACK;
				mainScene->SetGS("MAP");
				if (SharedData::GetInstance()->gameState != SharedData::GAMESTATE::GAME_BOSS)
					mainScene->RemoveEnemy();
				Reset();
			}
			else
			{

				escapePercentage = 25.0f;
				// new
				SharedData::GetInstance()->playerBattleDialogue = true;
			}
			break;
		}
	}
	else if (secondChoice)
	{
		//Second Choice only applys to Attack and Item as it need to display a new numbers of choices
		switch (battleSelection)
		{
		case BS_SLASH:
			//minus enemy hp, then enemy turn = true, player turn = false

			SharedData::GetInstance()->playerHitenemy = true;
			SharedData::GetInstance()->soundManager.playSE("Sound/slash.mp3");

			if (SharedData::GetInstance()->inventory.GetTotalATK() <= 0)
				theHero->SetDMG(10);
			else
				theHero->SetDMG(SharedData::GetInstance()->inventory.GetTotalATK());

			enemy->TakeDamage(theHero->GetDMG());

			SharedData::GetInstance()->BS_SlashRender = true;

			break;

		case BS_STAB:
			//minus enemy hp, then enemy turn = true, player turn = false
			SharedData::GetInstance()->playerHitenemy = true;
			SharedData::GetInstance()->soundManager.playMusic("Sound//stab.mp3");

			if (SharedData::GetInstance()->inventory.GetTotalATK() <= 0)
				theHero->SetDMG(15);
			else
				theHero->SetDMG(SharedData::GetInstance()->inventory.GetTotalATK());
			enemy->TakeDamage(theHero->GetDMG());
			SharedData::GetInstance()->BS_StabRender = true;
			break;
		case BS_SKILL:

			if (theHero->GetMP() >= 15)
			{
				if (SharedData::GetInstance()->inventory.getArmour() != NULL)
				{
					//theHero->UseMP(30);
					switch (SharedData::GetInstance()->inventory.getArmour()->getMonster().GetType())
					{
						//Different equided skill damage and mana cost
					case 0:
						theHero->UseMP(15);
						enemy->TakeDamage(theHero->GetDMG()* 2.35);
						SharedData::GetInstance()->BS_ScreamRender = true;
						SharedData::GetInstance()->soundManager.playSE("Sound//skill.mp3");
						SharedData::GetInstance()->playerHitenemy = true;
						break;
					case 1:
						if (theHero->GetMP() >= 35)
						{
							theHero->UseMP(35);
							enemy->TakeDamage(theHero->GetDMG()* 2.85);
							SharedData::GetInstance()->BS_BiteRender = true;
							SharedData::GetInstance()->soundManager.playSE("Sound//skill.mp3");
							SharedData::GetInstance()->playerHitenemy = true;
						}
						break;
					case 2:
						if (theHero->GetMP() >= 60)
						{
							theHero->UseMP(60);
							enemy->TakeDamage(theHero->GetDMG()* 3.15);
							SharedData::GetInstance()->BS_RoarRender = true;
							SharedData::GetInstance()->soundManager.playSE("Sound//skill.mp3");
							SharedData::GetInstance()->playerHitenemy = true;
						}
						break;
					case 3:
						if (theHero->GetMP() >= 20)
						{
							theHero->UseMP(20);
							theHero->SetDEF(theHero->GetDEF() + 15);
							SharedData::GetInstance()->BS_SkinRender = true;
							SharedData::GetInstance()->soundManager.playSE("Sound//skill.mp3");
							SharedData::GetInstance()->playerHitenemy = true;
						}
						break;
					}
				}
				else
				{
					battleSelection = BS_SKILL;
				}
				if (enemy->GetHealth() < 0)
				{
					//Player win
					Reset();
					mainScene->SetGS("TESTMAP");
					//destory enemy here
				}
			}
			else
			{
				//Display no mana
			}
			break;

		case BS_POTION:
			if (SharedData::GetInstance()->inventory.GetPotionCount() > 0)
			{
				SharedData::GetInstance()->inventory.usePotion();
			}
			break;
		case BS_TRAP:
			if (SharedData::GetInstance()->inventory.GetTrapCount() > 0)
			{
				if (SharedData::GetInstance()->trapPercentageIncrease < 20.0f)
				{
					SharedData::GetInstance()->inventory.removeFromInventory(Items::TRAP);
					SharedData::GetInstance()->trapPercentageIncrease = 20.0f;
					SharedData::GetInstance()->enemyCatchPercentage += SharedData::GetInstance()->trapPercentageIncrease;
				}
				if (SharedData::GetInstance()->enemyCatchPercentage > 100.0f)
					SharedData::GetInstance()->enemyCatchPercentage = 100.0f;

			}
			break;
		case BS_BACK2:
		case BS_BACK:

			firstChoice = true;
			secondChoice = false;
			SetOpenItemBag(false);
			battleSelection = BS_ATTACK;
			//Render back 1st page choices (Attack, Item bag, Capture, Run)
			break;

		}
		if (battleSelection == BS_ATTACK)
		{
			secondChoice = false;
			firstChoice = true;
		}
	}

}
void BattleSystem::GetBattleChoiceInput(static bool& UPkeyPressed, static bool& DNkeyPressed, static bool& LEFTkeyPressed, static bool& RIGHTkeyPressed, static bool& ENTERkeyPressed)
{
	if (!SharedData::GetInstance()->playerBattleDialogue)
	{
		if (Application::IsKeyPressed(VK_UP) && !UPkeyPressed)
		{
			SharedData::GetInstance()->soundManager.playSE("Sound/click.mp3");

			UPkeyPressed = true;
			if (openItemBag)
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 1);
			else
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 2);

			if (firstChoice && battleSelection < BS_ATTACK)
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 4);
			else if (secondChoice && battleSelection < BS_SLASH && !openItemBag)
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 4);
			else if (secondChoice && battleSelection < BS_POTION && openItemBag)
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 2);
		}
		else if (!Application::IsKeyPressed(VK_UP) && UPkeyPressed)
		{
			UPkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_DOWN) && !DNkeyPressed)
		{

			SharedData::GetInstance()->soundManager.playSE("Sound/click.mp3");

			DNkeyPressed = true;
			if (openItemBag)
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 1);
			else
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 2);

			if (firstChoice && battleSelection > BS_RUN)
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 4);
			else if (secondChoice && battleSelection > BS_BACK && !openItemBag)
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 4);
			else if (secondChoice && battleSelection > BS_TRAP && openItemBag)
				battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 2);
		}
		else if (!Application::IsKeyPressed(VK_DOWN) && DNkeyPressed)
		{


			DNkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_LEFT) && !LEFTkeyPressed)
		{

			SharedData::GetInstance()->soundManager.playSE("Sound/click.mp3");

			LEFTkeyPressed = true;
			battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 1);

			if (firstChoice && battleSelection < BS_ATTACK)
				battleSelection = BS_RUN;
			else if (secondChoice && battleSelection < BS_SLASH && !openItemBag)
				battleSelection = BS_BACK;
			else if (secondChoice && battleSelection < BS_POTION && openItemBag)
				battleSelection = BS_BACK2;
		}
		else if (!Application::IsKeyPressed(VK_LEFT) && LEFTkeyPressed)
		{


			LEFTkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_RIGHT) && !RIGHTkeyPressed)
		{

			SharedData::GetInstance()->soundManager.playSE("Sound/click.mp3");

			RIGHTkeyPressed = true;
			battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 1);

			if (firstChoice && battleSelection > BS_RUN)
				battleSelection = BS_ATTACK;
			else if (secondChoice && battleSelection > BS_BACK && !openItemBag)
				battleSelection = BS_SLASH;
			else if (secondChoice && battleSelection > BS_BACK2 && openItemBag)
				battleSelection = BS_POTION;
		}
		else if (!Application::IsKeyPressed(VK_RIGHT) && RIGHTkeyPressed)
		{

			RIGHTkeyPressed = false;
		}
	}
}
void BattleSystem::UpdateBattleSystem(static bool& UPkeyPressed, static bool& DNkeyPressed, static bool& LEFTkeyPressed, static bool& RIGHTkeyPressed, static bool& ENTERkeyPressed, CPlayerInfo* theHero, Enemy* enemy)
{

	if (SharedData::GetInstance()->playerTurn && !SharedData::GetInstance()->enemyTurn && !SharedData::GetInstance()->playerHitenemy)
	{
		GetBattleChoiceInput(UPkeyPressed, DNkeyPressed, LEFTkeyPressed, RIGHTkeyPressed, ENTERkeyPressed);
		if (Application::IsKeyPressed(VK_RETURN) && !ENTERkeyPressed)
		{
			ENTERkeyPressed = true;
			RunBattleChoice(theHero, enemy);
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && ENTERkeyPressed)
		{
			ENTERkeyPressed = false;
		}

	}
	else if (SharedData::GetInstance()->enemyTurn && !SharedData::GetInstance()->playerTurn)
	{
		if (SharedData::GetInstance()->enemyHitPlayer)
		{
			if (SharedData::GetInstance()->inventory.GetTotalDEF() <= 0)
				theHero->SetDEF(10);
			else
				theHero->SetDEF(SharedData::GetInstance()->inventory.GetTotalDEF()* 0.5);

			int damageToPlayer = enemy->GetDamage() - theHero->GetDEF()/5;
			if (damageToPlayer < 0)
				damageToPlayer = 0;
			theHero->TakeDMG(damageToPlayer);
			SetMonsterHitAnimation(true);
			SharedData::GetInstance()->soundManager.playSE("sound/monsterHit.mp3");

			SharedData::GetInstance()->enemyHitPlayer = false;
		}
	}
}