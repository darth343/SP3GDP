#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "EquipmentManager.h"
#include "Inventory.h"
#include "Monster.h"
#include "Enemy.h"
#include <vector>
#include "SharedData.h"

class SharedData
{
public:
	enum GAMESTATE
	{
		MENU,
		GAME_S1,
		GAME_S2,
		GAME_S3,
		GAME_S4,
		GAME_SECRET,
		GAME_BOSS,
		MAX,
	};
	static SharedData* GetInstance()
	{
		static SharedData data;
		return &data;
	}
	GAMESTATE gameState;
	bool stateCheck = false;
	
	EquipmentManager equipManager; // Equipment Functions
	Monster monType; // Monster Calls
	Inventory inventory;
	std::vector<Enemy*> enemyInventory;
	bool UPkeyPressed;
	bool DNkeyPressed;
	bool LEFTkeyPressed;
	bool RIGHTkeyPressed;
	bool ENTERkeyPressed;
	bool BACKkeyPressed;
	bool IkeyPressed;


private:
	SharedData()
	{
		//Instantiate
		gameState = MENU;
		UPkeyPressed = false;
		DNkeyPressed = false;
		LEFTkeyPressed = false;
		RIGHTkeyPressed = false;
		ENTERkeyPressed = false;
		BACKkeyPressed = false;
		IkeyPressed = false;
	}
	~SharedData()
	{

	}
};

#endif // !SHARED_DATA_H
