#include "BattleSystem.h"
#include "Enemy.h"
#include "Application.h"
#include "SceneText.h"
BattleSystem::BattleSystem():
	 battleStart(false),
	 firstChoice(true),
	 secondChoice(false),
	 arrowPos(120, 90, 0),
	 battleSelection(BS_ATTACK),
	 monsterHitAnimation(false)
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
	SceneText* mainScene = (SceneText*)Application::GetInstance().GetScene();

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
				cout << "ESCAPE LOHHHHHHHHHH!" << endl;
				battleSelection = BS_ATTACK;
				mainScene->SetGS("TESTMAP");
				mainScene->RemoveEnemy();
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
			theHero->SetDMG(10);
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

			theHero->SetDMG(15);
			enemy->TakeDamage(theHero->GetDMG());
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
			cout << " Monster's skills " << battleSelection << endl;

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
void BattleSystem::GetBattleChoiceInput(static bool& UPkeyPressed, static bool& DNkeyPressed, static bool& LEFTkeyPressed, static bool& RIGHTkeyPressed, static bool& ENTERkeyPressed)
{
	if (Application::IsKeyPressed(VK_UP) && !UPkeyPressed)
	{
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
		UPkeyPressed = false;
	}

	if (Application::IsKeyPressed(VK_DOWN) && !DNkeyPressed)
	{
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
		DNkeyPressed = false;
	}

	if (Application::IsKeyPressed(VK_LEFT) && !LEFTkeyPressed)
	{
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
		LEFTkeyPressed = false;
	}

	if (Application::IsKeyPressed(VK_RIGHT) && !RIGHTkeyPressed)
	{
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
			theHero->TakeDMG(enemy->GetDamage());
			SetMonsterHitAnimation(true);

			if (theHero->GetHP() < 0)
			{
				//Render Game over here
				Reset();
				cout << "Reset !!" << endl;
			}
			SharedData::GetInstance()->enemyHitPlayer = false;
		}
		cout << "Monster hit player for " << enemy->GetDamage() << " player HP left " << theHero->GetHP() << endl;

	}
}