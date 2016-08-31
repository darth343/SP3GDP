#include "Tamagucci.h"
#include "Application.h"
#include "Equipment.h"
#include "SharedData.h"

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
	tamfood = new GameObject;
	tamfood->position.Set(Math::RandFloatMinMax(0, 730), 150, 0);
	tamfood->scale.Set(64, 64, 1);

	SharedData::GetInstance()->hungerLevel = 3;
	SharedData::GetInstance()->energyLevel = 3;
	SharedData::GetInstance()->happinessLevel = 3;
}

TAMAGUCCI::~TAMAGUCCI()
{
	if (tamtam)
	delete tamtam;
	if (tamdrop)
	delete tamdrop;
	if (tamdrop2)
	delete tamdrop2;
	if (tamfood)
	delete tamfood;
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
void TAMAGUCCI::TamagucciBackgroundUpdate(double dt)
{
	moveUpdate(dt);
	for (int i = 0; i <= Inventory::LEGS; ++i)
	{
		if (SharedData::GetInstance()->inventory.getEquippedItems()[i])
		{
			if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->GetTamHunger() > 0)
			SharedData::GetInstance()->inventory.getEquippedItems()[i]->hungerLosstimer += dt * 3;
			if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->GetTamEnergy() > 0)
			SharedData::GetInstance()->inventory.getEquippedItems()[i]->energyLosstimer += dt * 3;
			if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->GetTamHappy() > 0)
			SharedData::GetInstance()->inventory.getEquippedItems()[i]->happinessLosstimer += dt * 3;

			if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->energyLosstimer > ENERGYLOSSTIME)
			{
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->energyLosstimer -= ENERGYLOSSTIME;
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->DecrementTamEnergy();
			}
			if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->hungerLosstimer > HUNGERLOSSTIME)
			{
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->hungerLosstimer -= HUNGERLOSSTIME;
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->DecrementTamHunger();
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->pooPositions.push_back(SharedData::GetInstance()->inventory.getEquippedItems()[i]->position);
			}
			if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->happinessLosstimer > HAPPYLOSSTIME)
			{
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->happinessLosstimer -= HAPPYLOSSTIME;
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->DecrementTamHappy();
			}
		}
	}
}
void TAMAGUCCI::moveUpdate(double dt)
{
	for (int i = 0; i <= Inventory::LEGS; ++i)
	{
		if (menuState == FIRSTMENU && SharedData::GetInstance()->inventory.getEquippedItems()[i])
		{
			if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->direction)
			{
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->position.x -= 100.f * dt;
				if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->position.x < 0)
				{
					SharedData::GetInstance()->inventory.getEquippedItems()[i]->position.x = 0;
					SharedData::GetInstance()->inventory.getEquippedItems()[i]->direction = false;
				}
			}
			else
			{
				SharedData::GetInstance()->inventory.getEquippedItems()[i]->position.x += 100.f * dt;
				if (SharedData::GetInstance()->inventory.getEquippedItems()[i]->position.x + tamtam->scale.x > 800)
				{
					SharedData::GetInstance()->inventory.getEquippedItems()[i]->position.x = 800 - tamtam->scale.x;
					SharedData::GetInstance()->inventory.getEquippedItems()[i]->direction = true;
				}
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
			if (SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x > tamfood->position.x - 30 && !touchedFood)
			{
				moveLeft = false;
				SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x -= 100 * dt;
				if (SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x < tamfood->position.x + 30)
				{
					SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x = tamfood->position.x + 30;
					touchedFood = true;
				}
			}
			else if (SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x < tamfood->position.x + 70 && !touchedFood)
			{
				moveLeft = true;
				SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x += 100 * dt;
				if (SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x > tamfood->position.x - 70)
				{
					SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x = tamfood->position.x - 70;
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
	for (int i = 0; i < Inventory::EI_TOTAL; ++i)
	{

	}

	switch (tamagotchiState)
	{
	case MENU:
		TamagucciBackgroundUpdate(dt);
		GetTamagucciInput();
		break;
	case GAME:
		switch (gameChoice)
		{
		case CATCHING:
			MiniGame1(dt);
		case ROCKPAPERSCISSORS:
			MiniGame2(dt);
		}
		break;
	}
}
void TAMAGUCCI::GetTamagucciInput()
{
	if (SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex] == NULL)
		return;

	switch (menuState)
	{
	case FIRSTMENU:
		// UP BUTTON
		if (Application::IsKeyPressed(VK_UP) && !SharedData::GetInstance()->UPkeyPressed)
		{
			SharedData::GetInstance()->UPkeyPressed = true;
			if (EquippedItemIndex < Inventory::LEGS && SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex + 1])
			{
				EquippedItemIndex++;
			}
			else
			{
				for (int i = 0; i < Inventory::LEGS+1; ++i)
				{
					if (i == EquippedItemIndex)
						continue;
					if (SharedData::GetInstance()->inventory.getEquippedItems()[i])
					{
						EquippedItemIndex = i;
						break;
					}
				}
			}
		}
		else if (!Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
		{
			SharedData::GetInstance()->UPkeyPressed = false;
		}

		// DOWN BUTTON
		if (Application::IsKeyPressed(VK_DOWN) && !SharedData::GetInstance()->DNkeyPressed)
		{
			SharedData::GetInstance()->DNkeyPressed = true;
			if (EquippedItemIndex > 0 && SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex - 1])
			{
				EquippedItemIndex--;
			}
			else
			{
				for (int i = Inventory::LEGS; i > 0; --i)
				{
					if (i == EquippedItemIndex)
						continue;
					if (SharedData::GetInstance()->inventory.getEquippedItems()[i])
					{
						EquippedItemIndex = i;
						break;
					}
				}
			}
		}
		else if (!Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
		{
			SharedData::GetInstance()->DNkeyPressed = false;
		}

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
							 if (foodChoice == FC_SALAD)
							 SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->IncrementTamHunger();
							 if (foodChoice == FC_HAMBURGER)
								 SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->IncrementTamHunger(2);
							 if (foodChoice == FC_PORK)
								 SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->IncrementTamHunger(3);

							 SharedData::GetInstance()->ENTERkeyPressed = true;
							 //SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->DecrementTamEnergy();
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
					  sleep = true;
					  // ENTER BUTTON
					  if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
					  {
						  SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->DecrementTamHunger();
						  SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->IncrementTamEnergy();
						  SharedData::GetInstance()->ENTERkeyPressed = true;
						  sleep = false;
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
				break;
				
		}
		case CLEAN:
		{
					  getCurrentTama()->pooPositions.clear();
					  menuState = FIRSTMENU;
		}
			break;
		case STATS:
		{
					  // ENTER BUTTON
					  if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
					  {
						  SharedData::GetInstance()->ENTERkeyPressed = true;
						 menuState = FIRSTMENU;
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
	return SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->GetTamHunger();
}
int TAMAGUCCI::getEnergylevel()
{
	return SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->GetTamEnergy();
}
int TAMAGUCCI::getHappinesslevel()
{
	return SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->GetTamHappy();
}
void TAMAGUCCI::SetIndex(int index)
{
	this->EquippedItemIndex = index;
}
Equipment* TAMAGUCCI::getCurrentTama()
{
	return SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex];
}
void TAMAGUCCI::MiniGame1UpdatePosition(double dt)
{
	if (minigame1Score < 20)
	{
		if (SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x < 730)
		{
			if (Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
			{
				SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x += 300 * dt;
			}
		}
		if (SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x > 0)
		{
			if (Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
				SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position.x -= 300 * dt;
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
void TAMAGUCCI::MiniGame1(double dt)
{
	bool GoBack = false;
	MiniGame1UpdatePosition(dt);
	tamtam->position = SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->position;
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
		SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->IncrementTamHappy();
		SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->DecrementTamEnergy();
		SharedData::GetInstance()->inventory.getEquippedItems()[EquippedItemIndex]->DecrementTamHunger();
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