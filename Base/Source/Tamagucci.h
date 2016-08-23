#ifndef TAMAGUCCI_H
#define TAMAGUCCI_H
#include "Vector3.h"
#include "GameObject.h"
#include <vector>
using std::vector;
class TAMAGUCCI
{
public:
	enum MENUSTATE
	{
		FIRSTMENU,
		SECONDMENU,
	};
	
	enum GAMEOPTIONS
	{
		CATCHING,
		ROCKPAPERSCISSORS,
		G_BACK,
	};

	enum FIRSTMENUPOPTIONS
	{
		NOTHING,
		FOOD,
		SLEEP,
		ENTERTAINMENT,
		CLEAN,
		STATS,
		TOTAL_FM,
	};

	enum FOODCHOICES
	{
		FC_SALAD,
		FC_HAMBURGER,
		FC_PORK,
		FC_BACK,
	};

	enum TAMAGOTCHISTATE
	{
		MENU,
		GAME,
	};

	TAMAGUCCI();
	~TAMAGUCCI();
	void UpdateTamagucci(double dt);
	void GetTamagucciInput();
	void MiniGame(double dt);
	void MiniGameUpdatePosition(double dt);
	void ResetTamagotchi();
	int GetScore();
	//friend std::ostream& operator<<(std::ostream& cout, FIRSTMENUPOPTIONS buttons);
	//friend std::ostream& operator<<(std::ostream& cout, FOODCHOICES foodchoice);
	TAMAGOTCHISTATE getTamagotchiState();
	MENUSTATE getMenuState();
	GAMEOPTIONS getGameChoice();
	FIRSTMENUPOPTIONS getFirstMenuOption();
	FOODCHOICES getFoodChoice();
	GameObject* GetTamTam();
	GameObject* GetTamDrop();
	GameObject* GetTamDrop2();
	int getHungerlevel();
	int getEnergylevel();
	int getHappinesslevel();
	void moveUpdate(double dt);
private:
	GameObject* tamtam;
	GameObject* tamdrop;
	GameObject* tamdrop2;
	float tamDropVel = 100; 
	float coolDown = 3.f;
	vector<Vector3> pooPositions;
	int minigame1Score;
	int hungerLevel;
	int energyLevel;
	int happinessLevel;
	//MENU STUFF
	TAMAGOTCHISTATE tamagotchiState;
	MENUSTATE menuState;
	FIRSTMENUPOPTIONS firstMenuOption;
	FOODCHOICES foodChoice;
	GAMEOPTIONS gameChoice;
	//MOVEMENT STUFF
	bool direction;
	Vector3 randPos;
	bool randomPosSet;
};
#endif // !TAMAGUCCI_H
