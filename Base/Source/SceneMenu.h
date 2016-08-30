#ifndef SCENE_MENU_H
#define SCENE_MENU_H
#include "SceneBase.h"

#include "Camera3.h"

#include "SharedData.h"

class SceneMenu : public SceneBase
{
	enum GAMESTATE_TYPE
	{
		GS_GAME,
		GS_INTRODUCTION,
		GS_INSTRUCTIONS,
		GS_OPTIONS,
		GS_QUIT,
		GS_MAX,
	};

public:
	SceneMenu();
	~SceneMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	GAMESTATE_TYPE m_gs;

private:
	float realizeiconx = 20;
	float othericonx = 500;
	bool move = false;
	bool movein = false;
};

#endif