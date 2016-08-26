#ifndef SCENE_STORY_H
#define SCENE_STORY_H
#include "SceneBase.h"

#include "Camera3.h"

#include "SharedData.h"

class SceneStory : public SceneBase
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
	SceneStory();
	~SceneStory();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void RenderText();
	GAMESTATE_TYPE m_gs;

private:
	bool flip = false;
	int i = 0;
	float herox = -15;
	ostringstream ss;
	float cooldown = 1.f;
	bool showFirst = false;
	bool showSecond = false;
};

#endif