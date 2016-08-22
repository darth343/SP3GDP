#ifndef TAMAGUCCI_H
#define TAMAGUCCI_H
#include "Vector3.h"
#include "GameObject.h"
#include <vector>
using std::vector;
class TAMAGUCCI
{
public:
	enum TAMAGUCCISTATES
	{
		FIRSTMENU,
		SECONDMENU,
		RUNCHOICE,
	};

	enum TAMABUTTONS
	{
		T_NOTHING,
		T_FOOD,
		T_SLEEP,
		T_ENTERTAINMENT,
		T_CLEAN,
		T_STATS,
		TOTAL_TBUTTONS,
	};

	enum FOODCHOICES
	{
		FC_NOTHING,
		FC_KB,
		FC_MB,
		FC_GB,
		FC_BACK,
		TOTAL_FC,
	};

	enum ENTERTAINMENTCHOICES
	{
		E_NOTHING,
		E_CATCHING,
		E_RPS,
		E_TOTAL,
	};

	enum CHOICES
	{
		R_NOTHING,
		R_ENTERTAINMENTCHOICES,
	};

	TAMAGUCCI();
	~TAMAGUCCI();
	void UpdateTamagucci(double dt);
	void GetTamagucciInput();
	void MiniGame(double dt);
	void MiniGameUpdatePosition(double dt);
	friend std::ostream& operator<<(std::ostream& cout, TAMABUTTONS buttons);
	friend std::ostream& operator<<(std::ostream& cout, FOODCHOICES foodchoice);
	CHOICES GetState();
	GameObject* GetTamTam();
	GameObject* GetTamDrop();
private:
	GameObject* tamtam;
	GameObject* tamdrop;
	TAMAGUCCISTATES state;
	TAMABUTTONS choice;
	FOODCHOICES foodChoice;
	CHOICES runChoice;
	ENTERTAINMENTCHOICES entertainmentChoice;
	vector<Vector3> pooPositions;
	int hungerLevel;
	int energyLevel;
	int happinessLevel;
};
#endif // !TAMAGUCCI_H
