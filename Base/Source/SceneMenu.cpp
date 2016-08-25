#include "SceneMenu.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SpriteAnimation.h"
#include "Enemy.h"
#include "Items.h"
SceneMenu::SceneMenu()
//, NPCInConvo(NULL)
{
	m_gs = GS_GAME;
}

SceneMenu::~SceneMenu()
{
	
}

void SceneMenu::Init()
{
	SceneBase::Init();
	//cout << npcvec[1].GetDialogue() << endl;
	//cout << npcvec[0].GetDialogue() << endl;
	//Init GameState Here for testing purposes
}


void SceneMenu::Update(double dt)
{
	//SharedData::GetInstance()->soundManager.SoundPlay("Sound/title.mp3", &SharedData::GetInstance()->title, 0.3f, false);
	SpriteAnimation *logo = dynamic_cast<SpriteAnimation*>(meshList[GEO_LOGO]);
	if (logo)
	{
		logo->Update(dt*0.5);
		logo->m_anim->animActive = true;
	}

	SpriteAnimation *play = dynamic_cast<SpriteAnimation*>(meshList[GEO_MENUPLAY]);
	SpriteAnimation *quit = dynamic_cast<SpriteAnimation*>(meshList[GEO_MENUQUIT]);
	SpriteAnimation *opt = dynamic_cast<SpriteAnimation*>(meshList[GEO_MENUOPT]);
	SpriteAnimation *inst = dynamic_cast<SpriteAnimation*>(meshList[GEO_MENUINST]);
	SpriteAnimation *story = dynamic_cast<SpriteAnimation*>(meshList[GEO_MENUSTORY]);

	if (play && m_gs == GS_GAME)
	{
		play->Update(dt*0.5);
		play->m_anim->animActive = true;
	}
	if (inst && m_gs == GS_INSTRUCTIONS)
	{
		inst->Update(dt*0.5);
		inst->m_anim->animActive = true;
		opt->m_currentFrame = 0;
		quit->m_currentFrame = 0;
		story->m_currentFrame = 0;
		play->m_currentFrame = 0;
	}
	if (story && m_gs == GS_INTRODUCTION)
	{
		story->Update(dt*0.5);
		story->m_anim->animActive = true;
	}
	if (opt && m_gs == GS_OPTIONS)
	{
		opt->Update(dt*0.5);
		opt->m_anim->animActive = true;
		
	}
	if (quit && m_gs == GS_QUIT)
	{
		quit->Update(dt*0.5);
		quit->m_anim->animActive = true;
	}
		if (Application::IsKeyPressed(VK_DOWN) && !SharedData::GetInstance()->DNkeyPressed)
		{
			SharedData::GetInstance()->DNkeyPressed = true;
			m_gs = static_cast<GAMESTATE_TYPE>(m_gs + 1);
			if (m_gs > GS_QUIT)
				m_gs = GS_GAME;
			play->m_currentFrame = 0;
			quit->m_currentFrame = 0;
			inst->m_currentFrame = 0;
			story->m_currentFrame = 0;
			opt->m_currentFrame = 0;
		}
		else if (!Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
		{
			SharedData::GetInstance()->DNkeyPressed = false;
		}
		if (Application::IsKeyPressed(VK_UP) && !SharedData::GetInstance()->UPkeyPressed)
		{
			SharedData::GetInstance()->UPkeyPressed = true;
			m_gs = static_cast<GAMESTATE_TYPE>(m_gs - 1);
			if (m_gs < GS_GAME)
				m_gs = GS_QUIT;
			play->m_currentFrame = 0;
			quit->m_currentFrame = 0;
			inst->m_currentFrame = 0;
			story->m_currentFrame = 0;
			opt->m_currentFrame = 0;
		}
		else if (!Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
		{
			SharedData::GetInstance()->UPkeyPressed = false;
		}
		if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = true;
			switch (m_gs)
			{
			case GS_GAME:
				SharedData::GetInstance()->stateCheck = true;
				SharedData::GetInstance()->gameState = SharedData::GAME_S1;
				break;
			//Other cases here
			}
		}
		else if (Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = false;
		}
	cout << m_gs << endl;

	fps = (float)(1.f / dt);

}

void SceneMenu::Render()
{
	SceneBase::Render();
	RenderBackground(meshList[GEO_MENUBG]);
	int x = 380;

	Render2DMeshWScale(meshList[GEO_LOGO], false, 350, 100, 200, 500, false);
	Render2DMeshWScale(meshList[GEO_MENUPLAY], false, 150, 60, 600, x, false);
	Render2DMeshWScale(meshList[GEO_MENUSTORY], false, 170, 60, 600, x-70, false);
	Render2DMeshWScale(meshList[GEO_MENUINST], false, 270, 60, 600, x - 140, false);
	Render2DMeshWScale(meshList[GEO_MENUOPT], false, 200, 60, 600, x-210, false);
	Render2DMeshWScale(meshList[GEO_MENUQUIT], false, 150, 60, 600, x-280, false);

	// Check for which GameState we are in
}

void SceneMenu::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}