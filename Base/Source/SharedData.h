#ifndef SHARED_DATA_H
#define SHARED_DATA_H
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
		GAME_BOSS,
		MAX,
	};
	static SharedData* GetInstance()
	{
		static SharedData data;
		return &data;
	}
	GAMESTATE gameState;
	bool UPkeyPressed;
	bool DNkeyPressed;
	bool LEFTkeyPressed;
	bool RIGHTkeyPressed;
	bool ENTERkeyPressed;
	bool IkeyPressed;

private:
	SharedData()
	{
		//Instantiate
		gameState = GAME_S1;
		UPkeyPressed = false;
		DNkeyPressed = false;
		LEFTkeyPressed = false;
		RIGHTkeyPressed = false;
		ENTERkeyPressed = false;
		IkeyPressed = false;
	}
	~SharedData()
	{

	}
};

#endif // !SHARED_DATA_H
