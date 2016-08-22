#ifndef SHARED_DATA_H
#define SHARED_DATA_H

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
private:
	SharedData()
	{
		//Instantiate
		gameState = GAME_S1;
	}
	~SharedData()
	{

	}
};

#endif // !SHARED_DATA_H
