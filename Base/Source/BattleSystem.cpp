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

void BattleSystem::Reset()
{
	battleStart = false;
	SharedData::GetInstance()->enemyTurn = false;
	SharedData::GetInstance()->playerTurn = true;
	arrowPos = Vector3(120, 90, 0);
	firstChoice = true;
	secondChoice = false;
	escapePercentage = 25.0f;
	battleSelection = BS_ATTACK;
	SetMonsterHitAnimation(false);
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
			cout << "ATTACK! " << endl;
			break;

		case BS_ITEM:
			//Render Item bag design
			//Battle selection set to Item's
			firstChoice = false;
			secondChoice = true;
			SetOpenItemBag(true);
			cout << "Item Bag" << battleSelection << endl;
			break;

		case BS_CAPTURE:
			//Start Capture function && render capture function
			mainScene->SetGS("CATCH");
			battleStart = false;
			break;


		case BS_RUN:
			cout << "RUN AWAY" << battleSelection << endl;
			escapePercentage += Math::RandFloatMinMax(0.0f, 50.0f);
			cout << "Escape % = " << escapePercentage << endl;
			if (escapePercentage > 50.0f)
			{
				SharedData::GetInstance()->playerTurn = true;
				SharedData::GetInstance()->enemyTurn = false;
				escapePercentage = 25.0f;
				SetBattleSelection(BattleSystem::BS_ATTACK);
				SetFirstChoice(true);
				SetSecondChoice(false);
				cout << "ESCAPE LOHHHHHHHHHH!" << endl;
				battleSelection = BS_ATTACK;
				mainScene->SetGS("MAP");
				mainScene->RemoveEnemy();
				Reset();
			}
			else
			{

				escapePercentage = 25.0f;
				cout << "WHY YOU NO ESCAPE!!!!" << endl;

				// new
				SharedData::GetInstance()->playerBattleDialogue = true;
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

			SharedData::GetInstance()->soundManager.SoundPlay("Sound/slash.mp3", &SharedData::GetInstance()->slash, 1.0f, false);

			if (SharedData::GetInstance()->inventory.GetTotalATK() <= 0)
				theHero->SetDMG(10);
			else
				theHero->SetDMG(SharedData::GetInstance()->inventory.GetTotalATK()* 0.5);

			// * by 0.5 because it might 1 hit the enemy 
			enemy->TakeDamage(theHero->GetDMG());

			SharedData::GetInstance()->BS_SlashRender = true;

			cout << "Player Slash Enemy for " << theHero->GetDMG() << " Enemy HP left " << enemy->GetHealth() << endl;

			//if enemy not dead		
			if (enemy->GetHealth() < 0)
			{
				//Player win
				Reset();
				mainScene->SetGS("TESTMAP");
				//destory enemy here
			}


			break;

		case BS_STAB:
			//minus enemy hp, then enemy turn = true, player turn = false
			cout << "Stab enemy " << battleSelection << endl;

			SharedData::GetInstance()->soundManager.SoundPlay("Sound/stab.mp3", &SharedData::GetInstance()->stab, 1.0f, false);

			if (SharedData::GetInstance()->inventory.GetTotalATK() <= 0)
				theHero->SetDMG(15);
			else
				theHero->SetDMG(SharedData::GetInstance()->inventory.GetTotalATK() * 0.5);
			enemy->TakeDamage(theHero->GetDMG()* 0.5);
			SharedData::GetInstance()->BS_StabRender = true;

			cout << "Player Stab Enemy for " << theHero->GetDMG() << " Enemy HP left " << enemy->GetHealth() << endl;

			//if enemy not dead		
			if (enemy->GetHealth() < 0)
			{
				//Player win
				Reset();
				mainScene->SetGS("TESTMAP");
				//destory enemy here
			}
			break;
		case BS_SKILL:

			//minus enemy hp, then enemy turn = true, player turn = false
			theHero->UseMP(50);
			battleSelection = BS_ATTACK;
			cout << " Monster's skills " << battleSelection << endl;
			SharedData::GetInstance()->soundManager.SoundPlay("Sound/skill.mp3", &SharedData::GetInstance()->skill, 1.0f, false);

			if (theHero->GetMP() >= 30)
			{
				if (SharedData::GetInstance()->inventory.getArmour() != NULL)
				{
					//theHero->UseMP(30);
					switch (SharedData::GetInstance()->inventory.getArmour()->getMonster().GetType())
					{
						//Different equided skill damage and mana cost
					case 0:
						theHero->UseMP(0);
						enemy->TakeDamage(theHero->GetDMG()* 0.35);
						SharedData::GetInstance()->BS_ScreamRender = true;
						break;
					case 1:
						theHero->UseMP(35);
						enemy->TakeDamage(theHero->GetDMG()* 0.25);
						SharedData::GetInstance()->BS_BiteRender = true;
						break;
					case 2:
						theHero->UseMP(0);
						enemy->TakeDamage(theHero->GetDMG()* 0.15);
						SharedData::GetInstance()->BS_RoarRender = true;
						break;
					case 3:
						theHero->UseMP(0);
						theHero->SetDEF(theHero->GetDEF() + 15);
						SharedData::GetInstance()->BS_SkinRender = true;
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
			SharedData::GetInstance()->inventory.removeFromInventory(Items::POTION);
			theHero->SetHP(theHero->GetHP() + 20);
			break;
		case BS_TRAP:
			SharedData::GetInstance()->inventory.removeFromInventory(Items::TRAP);
			SharedData::GetInstance()->trapPercentageIncrease = 20.0f;
			SharedData::GetInstance()->enemyCatchPercentage += SharedData::GetInstance()->trapPercentageIncrease;
			if (SharedData::GetInstance()->enemyCatchPercentage > 100.0f)
				SharedData::GetInstance()->enemyCatchPercentage = 100.0f;
			break;
		case BS_BACK:
			cout << " Back " << battleSelection << endl;

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
	if (Application::IsKeyPressed(VK_UP) && !UPkeyPressed)
	{
		SharedData::GetInstance()->soundManager.SoundPlay("Sound/click.mp3", &SharedData::GetInstance()->click, 1.0f, false);

		UPkeyPressed = true;
		battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 2);

		if (firstChoice && battleSelection < BS_ATTACK)
			battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 4);
		else if (secondChoice && battleSelection < BS_SLASH)
			battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 4);

		cout << "BS = " << battleSelection << endl;
	}
	else if (!Application::IsKeyPressed(VK_UP) && UPkeyPressed)
	{

		SharedData::GetInstance()->soundManager.SoundPlay("Sound/click.mp3", &SharedData::GetInstance()->click, 1.0f, false);

		UPkeyPressed = false;
	}

	if (Application::IsKeyPressed(VK_DOWN) && !DNkeyPressed)
	{

		SharedData::GetInstance()->soundManager.SoundPlay("Sound/click.mp3", &SharedData::GetInstance()->click, 1.0f, false);

		DNkeyPressed = true;
		battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 2);

		if (firstChoice && battleSelection > BS_RUN)
			battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 4);
		else if (secondChoice && battleSelection > BS_BACK)
			battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 4);

		cout << "BS = " << battleSelection << endl;
	}
	else if (!Application::IsKeyPressed(VK_DOWN) && DNkeyPressed)
	{

		SharedData::GetInstance()->soundManager.SoundPlay("Sound/click.mp3", &SharedData::GetInstance()->click, 1.0f, false);

		DNkeyPressed = false;
	}

	if (Application::IsKeyPressed(VK_LEFT) && !LEFTkeyPressed)
	{

		SharedData::GetInstance()->soundManager.SoundPlay("Sound/click.mp3", &SharedData::GetInstance()->click, 1.0f, false);

		LEFTkeyPressed = true;
		battleSelection = static_cast<BATTLE_SELECTION> (battleSelection - 1);

		if (firstChoice && battleSelection < BS_ATTACK)
			battleSelection = BS_RUN;
		else if (secondChoice && battleSelection < BS_SLASH)
			battleSelection = BS_BACK;

		cout << "BS = " << battleSelection << endl;
	}
	else if (!Application::IsKeyPressed(VK_LEFT) && LEFTkeyPressed)
	{

		SharedData::GetInstance()->soundManager.SoundPlay("Sound/click.mp3", &SharedData::GetInstance()->click, 1.0f, false);

		LEFTkeyPressed = false;
	}

	if (Application::IsKeyPressed(VK_RIGHT) && !RIGHTkeyPressed)
	{

		SharedData::GetInstance()->soundManager.SoundPlay("Sound/click.mp3", &SharedData::GetInstance()->click, 1.0f, false);

		RIGHTkeyPressed = true;
		battleSelection = static_cast<BATTLE_SELECTION> (battleSelection + 1);

		if (firstChoice && battleSelection > BS_RUN)
			battleSelection = BS_ATTACK;
		else if (secondChoice && battleSelection > BS_BACK)
			battleSelection = BS_SLASH;

		cout << "BS = " << battleSelection << endl;
	}
	else if (!Application::IsKeyPressed(VK_RIGHT) && RIGHTkeyPressed)
	{

		SharedData::GetInstance()->soundManager.SoundPlay("Sound/click.mp3", &SharedData::GetInstance()->click, 1.0f, false);

		RIGHTkeyPressed = false;
	}
}
void BattleSystem::UpdateBattleSystem(static bool& UPkeyPressed, static bool& DNkeyPressed, static bool& LEFTkeyPressed, static bool& RIGHTkeyPressed, static bool& ENTERkeyPressed, CPlayerInfo* theHero, Enemy* enemy)
{
	if (SharedData::GetInstance()->playerTurn && !SharedData::GetInstance()->enemyTurn)
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

			SharedData::GetInstance()->enemyHitPlayer = false;
		}
	}
}