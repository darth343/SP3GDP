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
	GameObject* GetTamFood();
	int getHungerlevel();
	int getEnergylevel();
	int getHappinesslevel();
	void moveUpdate(double dt);
	bool GetShowFood();
	bool GetTouchedFood();
	bool GetMoveLeft();
	void SetAnimationOver(bool&);
	bool GetSleep();
	
private:
	bool tamagucciFood = false; //Show the food in scene text
	bool animationFood=false; //For scene text check if food animation is over //BUGGY
	bool touchedFood = false; //For scene text check if food is touched
	bool moveLeft = false; //Sprite Animation Flip
	bool sleep = false;
	GameObject* tamtam;
	GameObject* tamdrop;
	GameObject* tamdrop2;
	GameObject* tamfood;
	float tamDropVel = 100; 
	float coolDown = 3.f;
	vector<Vector3> pooPositions;
	int minigame1Score;
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
	int happyLevel;
	int hungerLevel;
	int energyLevel;
};
#endif // !TAMAGUCCI_H
