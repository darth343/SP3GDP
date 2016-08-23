#include "Tamagucci.h"
#include "Application.h"

TAMAGUCCI::TAMAGUCCI()
{
	state = FIRSTMENU;
	choice = T_FOOD;
	entertainmentChoice = E_NOTHING;
	foodChoice = FC_KB;
	tamtam = new GameObject;
	tamtam->position.Set(350, 250, 1);
	tamtam->scale.Set(64, 64, 1);

	tamdrop = new GameObject;
	tamdrop->position.Set(Math::RandFloatMinMax(0, 730), 600, 1);
	tamdrop->type = GameObject::GO_TAMDROP1;
	tamdrop->scale.Set(64, 64, 1);

	tamdrop2 = new GameObject;
	tamdrop2->position.Set(Math::RandFloatMinMax(0, 730), 650, 1);
	tamdrop2->type = GameObject::GO_TAMDROP2;
	tamdrop2->scale.Set(64, 64, 1);

	minigame1Score = 0;
}

TAMAGUCCI::~TAMAGUCCI()
{
}

std::ostream& operator<<(std::ostream& cout, TAMAGUCCI::TAMABUTTONS buttons)
{
	switch (buttons)
	{
		case TAMAGUCCI::T_NOTHING:
			cout << "NOTHING";
			break;
		case TAMAGUCCI::T_FOOD:
			cout << "FOOD";
			break;
		case TAMAGUCCI::T_SLEEP:
			cout << "SLEEP";
			break;
		case TAMAGUCCI::T_ENTERTAINMENT:
			cout << "ENTERTAINMENT";
			break;
		case TAMAGUCCI::T_CLEAN:
			cout << "CLEAN";
			break;
		case TAMAGUCCI::T_STATS:
			cout << "STATS";
			break;
		case TAMAGUCCI::TOTAL_TBUTTONS:
			cout << "TOTAL BUTTONS";
			break;
	}
	return cout;
}

std::ostream& operator<<(std::ostream& cout, TAMAGUCCI::FOODCHOICES foodchoice)
{
	switch (foodchoice)
	{
	case TAMAGUCCI::FC_KB:
		cout << "KILOBYTE";
		break;
	case TAMAGUCCI::FC_MB:
		cout << "MEGABYTE";
		break;
	case TAMAGUCCI::FC_GB:
		cout << "GIGABYTE";
		break;
	case TAMAGUCCI::FC_BACK:
		cout << "BACK";
		break;
	}
	return cout;
}

void TAMAGUCCI::UpdateTamagucci(double dt)
{
	if (state == FIRSTMENU || state == SECONDMENU)
	GetTamagucciInput();
	if (state == RUNCHOICE)
	{
		switch (runChoice)
		{
		case R_ENTERTAINMENTCHOICES:
		{
			if (entertainmentChoice == E_CATCHING)
			{
				 MiniGame(dt);
			}
		}
		}
	}
}

void TAMAGUCCI::GetTamagucciInput()
{
	switch (state)
	{
	case FIRSTMENU:
		// RIGHT BUTTON
		if (Application::IsKeyPressed(VK_RIGHT) && !SharedData::GetInstance()->RIGHTkeyPressed)
		{
			SharedData::GetInstance()->RIGHTkeyPressed = true;
			choice = static_cast<TAMABUTTONS>(1 + choice);
			if (choice == TOTAL_TBUTTONS)
				choice = T_FOOD;
			cout << choice << endl;
		}
		else if (!Application::IsKeyPressed(VK_RIGHT) && SharedData::GetInstance()->RIGHTkeyPressed)
		{
			SharedData::GetInstance()->RIGHTkeyPressed = false;
		}

		// LEFT BUTTON
		if (Application::IsKeyPressed(VK_LEFT) && !SharedData::GetInstance()->LEFTkeyPressed)
		{
			SharedData::GetInstance()->LEFTkeyPressed = true;
			choice = static_cast<TAMABUTTONS>(choice - 1);
			if (choice == T_NOTHING)
				choice = T_STATS;
			cout << choice << endl;
		}
		else if (!Application::IsKeyPressed(VK_LEFT) && SharedData::GetInstance()->LEFTkeyPressed)
		{
			SharedData::GetInstance()->LEFTkeyPressed = false;
		}
		// ENTER BUTTON
		if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = true; 
			state = SECONDMENU;
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = false;
		}
		break;

	case SECONDMENU:
		switch (choice)
		{
		case T_FOOD:
		{
			// RIGHT BUTTON
			if (Application::IsKeyPressed(VK_RIGHT) && !SharedData::GetInstance()->RIGHTkeyPressed)
			{
				SharedData::GetInstance()->RIGHTkeyPressed = true;
				foodChoice = static_cast<FOODCHOICES>(1 + foodChoice);
				if (foodChoice == TOTAL_FC)
					foodChoice = FC_KB;
				cout << foodChoice << endl;
			}
			else if (!Application::IsKeyPressed(VK_RIGHT) && SharedData::GetInstance()->RIGHTkeyPressed)
			{
				SharedData::GetInstance()->RIGHTkeyPressed = false;
			}

			// LEFT BUTTON
			if (Application::IsKeyPressed(VK_LEFT) && !SharedData::GetInstance()->LEFTkeyPressed)
			{
				SharedData::GetInstance()->LEFTkeyPressed = true;
				foodChoice = static_cast<FOODCHOICES>(foodChoice - 1);
				if (foodChoice == FC_NOTHING)
					foodChoice = FC_BACK;
				cout << foodChoice << endl;
			}
			else if (!Application::IsKeyPressed(VK_LEFT) && SharedData::GetInstance()->LEFTkeyPressed)
			{
				SharedData::GetInstance()->LEFTkeyPressed = false;
			}

			// ENTER BUTTON
			if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
			{
				SharedData::GetInstance()->ENTERkeyPressed = true;
				if (foodChoice == FC_BACK)
					state = FIRSTMENU;
			}
			else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
			{
				SharedData::GetInstance()->ENTERkeyPressed = false;
			}

		}
			break;
		case T_ENTERTAINMENT:
		{
				// RIGHT BUTTON
				if (Application::IsKeyPressed(VK_RIGHT) && !SharedData::GetInstance()->RIGHTkeyPressed)
				{
					SharedData::GetInstance()->RIGHTkeyPressed = true;
					entertainmentChoice = static_cast<ENTERTAINMENTCHOICES>(1 + entertainmentChoice);
					if (entertainmentChoice == E_TOTAL)
						entertainmentChoice = E_CATCHING;
					cout << entertainmentChoice << endl;
				}
				else if (!Application::IsKeyPressed(VK_RIGHT) && SharedData::GetInstance()->RIGHTkeyPressed)
				{
					SharedData::GetInstance()->RIGHTkeyPressed = false;
				}

				// LEFT BUTTON
				if (Application::IsKeyPressed(VK_LEFT) && !SharedData::GetInstance()->LEFTkeyPressed)
				{
					SharedData::GetInstance()->LEFTkeyPressed = true;
					entertainmentChoice = static_cast<ENTERTAINMENTCHOICES>(entertainmentChoice - 1);
					if (entertainmentChoice == E_NOTHING)
						entertainmentChoice = E_RPS;
					cout << entertainmentChoice << endl;
				}
				else if (!Application::IsKeyPressed(VK_LEFT) && SharedData::GetInstance()->LEFTkeyPressed)
				{
					SharedData::GetInstance()->LEFTkeyPressed = false;
				}

				// ENTER BUTTON
				if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = true;
					entertainmentChoice = E_CATCHING;
					if (entertainmentChoice == E_CATCHING)
					{
						runChoice = R_ENTERTAINMENTCHOICES;
						state = RUNCHOICE;
					}
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
int TAMAGUCCI::GetScore()
{
	return minigame1Score;
}
void TAMAGUCCI::MiniGameUpdatePosition(double dt)
{
	tamtam->position.y = 100;
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
		entertainmentChoice = E_NOTHING;
		choice = T_FOOD;
		state = FIRSTMENU;
		runChoice = R_NOTHING;
		GoBack = true;
		tamtam->position.Set(350, 250, 1);
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
TAMAGUCCI::CHOICES TAMAGUCCI::GetState()
{
	return runChoice;
}
TAMAGUCCI::TAMABUTTONS TAMAGUCCI::GetStates()
{
	return choice;
}
TAMAGUCCI::ENTERTAINMENTCHOICES TAMAGUCCI::GetEntertainChoices()
{
	return entertainmentChoice;
}