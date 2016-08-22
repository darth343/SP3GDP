#include "Tamagucci.h"
#include "Application.h"
TAMAGUCCI::TAMAGUCCI()
{
	button = T_FOOD;
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
			case TAMAGUCCI::T_BOND:
				cout << "BOND";
				break;
			case TAMAGUCCI::TOTAL_TBUTTONS:
				cout << "TOTAL BUTTONS";
				break;
	}
	return cout;
}

void TAMAGUCCI::UpdateTamagucci()
{
	GetTamagucciInput();
	cout << button << endl;
}

void TAMAGUCCI::GetTamagucciInput()
{
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		button = static_cast<TAMABUTTONS>(1 + button);
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		button = static_cast<TAMABUTTONS>(1 - button);
	}
	if (button == TOTAL_TBUTTONS)
		button = T_FOOD;
	else if (button == T_NOTHING)
		button = T_BOND;
}