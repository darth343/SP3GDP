#include "Tamagucci.h"
#include "Application.h"

TAMAGUCCI::TAMAGUCCI()
{
	ResetTamagotchi();
	tamtam = new GameObject;
	tamtam->position.Set(350, 100, 1);
	tamtam->scale.Set(64, 64, 1);

	tamdrop = new GameObject;
	tamdrop->position.Set(Math::RandFloatMinMax(0, 730), 600, 1);
	tamdrop->type = GameObject::GO_TAMDROP1;
	tamdrop->scale.Set(64, 64, 1);

	tamdrop2 = new GameObject;
	tamdrop2->position.Set(Math::RandFloatMinMax(0, 730), 650, 1);
	tamdrop2->type = GameObject::GO_TAMDROP2;
	tamdrop2->scale.Set(64, 64, 1);
	direction = true;
	randomPosSet = false;

	tamfood = new GameObject;
	tamfood->position.Set(Math::RandFloatMinMax(0, 730), 150, 0);
	tamfood->scale.Set(64, 64, 1);

	SharedData::GetInstance()->hungerLevel = 3;
	SharedData::GetInstance()->energyLevel = 3;
	SharedData::GetInstance()->happinessLevel = 3;
}

TAMAGUCCI::~TAMAGUCCI()
{
}

//std::ostream& operator<<(std::ostream& cout, TAMAGUCCI::MENUBUTTONS buttons)
//{
//	switch (buttons)
//	{
//		case TAMAGUCCI::T_NOTHING:
//			cout << "NOTHING";
//			break;
//		case TAMAGUCCI::T_FOOD:
//			cout << "FOOD";
//			break;
//		case TAMAGUCCI::T_SLEEP:
//			cout << "SLEEP";
//			break;
//		case TAMAGUCCI::T_ENTERTAINMENT:
//			cout << "ENTERTAINMENT";
//			break;
//		case TAMAGUCCI::T_CLEAN:
//			cout << "CLEAN";
//			break;
//		case TAMAGUCCI::T_STATS:
//			cout << "STATS";
//			break;
//		case TAMAGUCCI::TOTAL_TBUTTONS:
//			cout << "TOTAL BUTTONS";
//			break;
//	}
//	return cout;
//}
//
//std::ostream& operator<<(std::ostream& cout, TAMAGUCCI::FOODCHOICES foodchoice)
//{
//	switch (foodchoice)
//	{
//	case TAMAGUCCI::FC_KB:
//		cout << "KILOBYTE";
//		break;
//	case TAMAGUCCI::FC_MB:
//		cout << "MEGABYTE";
//		break;
//	case TAMAGUCCI::FC_GB:
//		cout << "GIGABYTE";
//		break;
//	case TAMAGUCCI::FC_BACK:
//		cout << "BACK";
//		break;
//	}
//	return cout;
//}

void TAMAGUCCI::moveUpdate(double dt)
{
	if (!tamagucciFood && !sleep)
	{
		if (direction)
		{
			tamtam->position.x -= 100.f * dt;
			if (tamtam->position.x < 0)
			{
				tamtam->position.x = 0;
				direction = false;
			}
		}
		else
		{
			tamtam->position.x += 100.f * dt;
			if (tamtam->position.x + tamtam->scale.x > 800)
			{
				tamtam->position.x = 800 - tamtam->scale.x;
				direction = true;
			}
		}
	}

	if (animationFood && tamagucciFood)
	{
		tamagucciFood = false;
		touchedFood = false;
		animationFood = false;
	}

	if (!touchedFood && tamagucciFood)
	{
			if (tamtam->position.x > tamfood->position.x - 30 && !touchedFood)
			{
				moveLeft = false;
				tamtam->position.x -= 100 * dt;
				if (tamtam->position.x < tamfood->position.x + 30)
				{
					tamtam->position.x = tamfood->position.x + 30;
					touchedFood = true;
				}
			}
			else if (tamtam->position.x < tamfood->position.x + 70 && !touchedFood)
			{
				moveLeft = true;
				tamtam->position.x += 100 * dt;
				if (tamtam->position.x > tamfood->position.x - 70)
				{
					tamtam->position.x = tamfood->position.x - 70;
					touchedFood = true;
				}
			}
	}
}
void TAMAGUCCI::SetAnimationOver(bool &over)
{
	if (animationFood != true)
		animationFood = over;
	else 	
		animationFood = over;

}
bool TAMAGUCCI::GetTouchedFood()
{
	return touchedFood;
}
bool TAMAGUCCI::GetMoveLeft()
{
	return moveLeft;
}
GameObject* TAMAGUCCI::GetTamFood()
{
	return tamfood;
}
void TAMAGUCCI::UpdateTamagucci(double dt)
{
	switch (tamagotchiState)
	{
	case MENU:
		moveUpdate(dt);
		GetTamagucciInput();
		break;
	case GAME:
		switch (gameChoice)
		{
		case CATCHING:
			MiniGame(dt);
		}
		break;
	}
}

void TAMAGUCCI::GetTamagucciInput()
{
	switch (menuState)
	{
	case FIRSTMENU:
		// RIGHT BUTTON
		if (Application::IsKeyPressed(VK_RIGHT) && !SharedData::GetInstance()->RIGHTkeyPressed)
		{
			SharedData::GetInstance()->RIGHTkeyPressed = true;
			firstMenuOption = static_cast<FIRSTMENUPOPTIONS>(1 + firstMenuOption);
			if (firstMenuOption == TOTAL_FM)
				firstMenuOption = FOOD;
		}
		else if (!Application::IsKeyPressed(VK_RIGHT) && SharedData::GetInstance()->RIGHTkeyPressed)
		{
			SharedData::GetInstance()->RIGHTkeyPressed = false;
		}

		// LEFT BUTTON
		if (Application::IsKeyPressed(VK_LEFT) && !SharedData::GetInstance()->LEFTkeyPressed)
		{
			SharedData::GetInstance()->LEFTkeyPressed = true;
			firstMenuOption = static_cast<FIRSTMENUPOPTIONS>(firstMenuOption - 1);
			if (firstMenuOption == 0)
				firstMenuOption = STATS;
		}
		else if (!Application::IsKeyPressed(VK_LEFT) && SharedData::GetInstance()->LEFTkeyPressed)
		{
			SharedData::GetInstance()->LEFTkeyPressed = false;
		}
		// ENTER BUTTON
		if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = true; 
			menuState = SECONDMENU;
			if (firstMenuOption == SLEEP)
				sleep = true;
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = false;
		}
		break;

	case SECONDMENU:
		switch (firstMenuOption)
		{
		case FOOD:
		{
					 if (!tamagucciFood)
					 {
						 if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
						 {
							 SharedData::GetInstance()->hungerLevel++;
							 SharedData::GetInstance()->ENTERkeyPressed = true;
							 SharedData::GetInstance()->energyLevel--;
							 if (foodChoice != FC_BACK)
							 {
								 tamagucciFood = true;
								 tamfood->position.Set(Math::RandFloatMinMax(0, 730), 150, 0);
								 animationFood = false;
							 }
							 if (foodChoice == FC_BACK)
							 {
								 ResetTamagotchi();
								 menuState = FIRSTMENU;
							 }

						 }
						 else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
						 {
							 SharedData::GetInstance()->ENTERkeyPressed = false;
						 }
						 // RIGHT BUTTON
						 if (Application::IsKeyPressed(VK_DOWN) && !SharedData::GetInstance()->DNkeyPressed)
						 {
							 SharedData::GetInstance()->DNkeyPressed = true;
							 foodChoice = static_cast<FOODCHOICES>(1 + foodChoice);
							 if (foodChoice > FC_BACK)
								 foodChoice = FC_SALAD;
							 tamagucciFood = false;
						 }
						 else if (!Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
						 {
							 SharedData::GetInstance()->DNkeyPressed = false;
						 }

						 // LEFT BUTTON
						 if (Application::IsKeyPressed(VK_UP) && !SharedData::GetInstance()->UPkeyPressed)
						 {
							 SharedData::GetInstance()->UPkeyPressed = true;
							 foodChoice = static_cast<FOODCHOICES>(foodChoice - 1);
							 if (foodChoice < FC_SALAD)
								 foodChoice = FC_BACK;
							 tamagucciFood = false;
						 }
						 else if (!Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
						 {
							 SharedData::GetInstance()->UPkeyPressed = false;
						 }
					 }

		}
			break;
		case SLEEP:
		{
					  // ENTER BUTTON
					  if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
					  {
						  SharedData::GetInstance()->energyLevel++;
						  SharedData::GetInstance()->hungerLevel--;
						  SharedData::GetInstance()->ENTERkeyPressed = true;
						  sleep = false;
						  //ResetTamagotchi();
							  menuState = FIRSTMENU;
					  }
					  else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
					  {
						  SharedData::GetInstance()->ENTERkeyPressed = false;
					  }
		}
			break;
		case ENTERTAINMENT:
		{
				// RIGHT BUTTON
				if (Application::IsKeyPressed(VK_DOWN) && !SharedData::GetInstance()->DNkeyPressed)
				{
					SharedData::GetInstance()->DNkeyPressed = true;
					gameChoice = static_cast<GAMEOPTIONS>(1 + gameChoice);
					if (gameChoice > G_BACK)
						gameChoice = CATCHING;
				}
				else if (!Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
				{
								  SharedData::GetInstance()->DNkeyPressed = false;
				}

				// LEFT BUTTON
				if (Application::IsKeyPressed(VK_UP) && !SharedData::GetInstance()->UPkeyPressed)
				{
					SharedData::GetInstance()->UPkeyPressed = true;
					gameChoice = static_cast<GAMEOPTIONS>(gameChoice - 1);
					if (gameChoice < CATCHING)
						gameChoice = G_BACK;
				}
				else if (!Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
				{
					SharedData::GetInstance()->UPkeyPressed = false;
				}
				// ENTER BUTTON
				if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = true;
					if (gameChoice == G_BACK)
					{
						menuState = FIRSTMENU;
					}
					else
						tamagotchiState = GAME;
				}
				else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = false;
				}
				
		}
			break;
		}
		break;
	}
}
bool TAMAGUCCI::GetSleep()
{
	return sleep;
}
bool TAMAGUCCI::GetShowFood()
{
	return tamagucciFood;
}
int TAMAGUCCI::GetScore()
{
	return minigame1Score;
}

int TAMAGUCCI::getHungerlevel()
{
	return SharedData::GetInstance()->hungerLevel;
}

int TAMAGUCCI::getEnergylevel()
{
	return SharedData::GetInstance()->energyLevel;
}

int TAMAGUCCI::getHappinesslevel()
{
	return SharedData::GetInstance()->happinessLevel;
}

void TAMAGUCCI::MiniGameUpdatePosition(double dt)
{
	if (minigame1Score < 20)
	{
		if (tamtam->position.x < 730)
		{
			if (Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
			{
				tamtam->position.x += 300 * dt;
			}
		}
		if (tamtam->position.x > 0)
		{
			if (Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
				tamtam->position.x -= 300 * dt;
		}
	}
}

void TAMAGUCCI::ResetTamagotchi()
{
	tamagotchiState = MENU;
	menuState = FIRSTMENU;
	gameChoice = CATCHING;
	firstMenuOption = FOOD;
	foodChoice = FC_SALAD;
	minigame1Score = 0;
}

void TAMAGUCCI::MiniGame(double dt)
{
	bool GoBack = false;
	MiniGameUpdatePosition(dt);
	if (tamtam->CheckCollision(tamdrop))
	{
		tamdrop->position.Set(Math::RandFloatMinMax(0, 730), 600, 0);
		tamDropVel = Math::RandFloatMinMax(100, 130);
		minigame1Score += 5;
	}
	if (tamdrop->position.y <= 0)
	{
		tamdrop->position.Set(Math::RandFloatMinMax(0, 730), 600, 0);
		tamDropVel = Math::RandFloatMinMax(100, 130);
	}

	if (tamtam->CheckCollision(tamdrop2))
	{
		tamdrop2->position.Set(Math::RandFloatMinMax(0, 730), 650, 0);
		tamDropVel = Math::RandFloatMinMax(100, 130);
		minigame1Score -= 5;
	}
	if (tamdrop2->position.y <= 0)
	{
		tamdrop2->position.Set(Math::RandFloatMinMax(0, 730), 650, 0);
		tamDropVel = Math::RandFloatMinMax(100, 130);
	}
	cout << minigame1Score << endl;
	cout << coolDown << endl;
	if (minigame1Score >= 20 && !GoBack)
	{
		coolDown -= dt;
	}
	else
	{
		tamdrop->position.y -= tamDropVel * dt;
		tamdrop2->position.y -= tamDropVel * dt;
	}
	if (coolDown <= 0)
	{
		SharedData::GetInstance()->happinessLevel++;
		SharedData::GetInstance()->hungerLevel--;
		SharedData::GetInstance()->energyLevel--;
		ResetTamagotchi();
		GoBack = true;
		minigame1Score = 0;
		coolDown = 3.f;
	}
}

GameObject* TAMAGUCCI::GetTamTam()
{
	return tamtam;
}
GameObject* TAMAGUCCI::GetTamDrop()
{
	return tamdrop;
}
GameObject* TAMAGUCCI::GetTamDrop2()
{
	return tamdrop2;
}

TAMAGUCCI::TAMAGOTCHISTATE TAMAGUCCI::getTamagotchiState()
{
	return tamagotchiState;
}

TAMAGUCCI::MENUSTATE TAMAGUCCI::getMenuState()
{
	return menuState;
}

TAMAGUCCI::GAMEOPTIONS TAMAGUCCI::getGameChoice()
{
	return gameChoice;
}

TAMAGUCCI::FIRSTMENUPOPTIONS TAMAGUCCI::getFirstMenuOption()
{
	return firstMenuOption;
}

TAMAGUCCI::FOODCHOICES TAMAGUCCI::getFoodChoice()
{
	return foodChoice;
}