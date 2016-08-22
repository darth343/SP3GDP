#include "Tamagucci.h"
#include "Application.h"

TAMAGUCCI::TAMAGUCCI()
{
	state = FIRSTMENU;
	choice = T_NOTHING;
	entertainmentChoice = E_NOTHING;
	foodChoice = FC_KB;
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

void TAMAGUCCI::UpdateTamagucci()
{
	if (state == FIRSTMENU || state == SECONDMENU)
	GetTamagucciInput();
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
		}
			break;
		}
		break;
	}
}