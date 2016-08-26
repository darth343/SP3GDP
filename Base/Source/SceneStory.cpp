#include "SceneStory.h"
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
SceneStory::SceneStory()
//, NPCInConvo(NULL)
{
	m_gs = GS_GAME;
}

SceneStory::~SceneStory()
{
	
}

void SceneStory::Init()
{
	SceneBase::Init();
}


void SceneStory::Update(double dt)
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

	fps = (float)(1.f / dt);
	SpriteAnimation *hero = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEROLR]);
	
	if (herox < 270 || showSecond)
	{
		herox += 40 * dt;
		if (hero)
		{
			flip = true;
			hero->Update(dt);
			hero->m_anim->animActive = true;
		}
	}
	else if (herox >= 270 && cooldown >= 0)
	{
		cooldown -= dt;
		if (ss.str().length() < 20)
			ss << ".";
	}
	else if (cooldown <= 0 && !showFirst && !showSecond)
	{
		showFirst = true;
		cooldown = 5.f;
		cooldown -= dt;
	}
	if (showFirst && cooldown <= 0)
	{
		showFirst = false;
		showSecond = true;
	}
	if (showSecond && herox < 900)
	{
		herox += 70 * dt;
	}
	else if (herox > 810)
	{
		if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
		{
				SharedData::GetInstance()->stateCheck = true;
				SharedData::GetInstance()->gameState = SharedData::GAME_S1;
		}
		else if (Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = false;
		}
	}
}

void SceneStory::Render()
{
	SceneBase::Render();
	RenderBackground(meshList[GEO_STORYBACKGROUND]);
	int x = 380;
	Render2DMesh(meshList[GEO_HEROLR], false, 64, herox, 100, flip);
	if (showFirst)
	{
		Render2DMeshWScale(meshList[GEO_EXCLAIM], false, 1, 1, herox - 18, 170, false);
		RenderTextOnScreen(meshList[GEO_TEXT], "What happened?! All the monsters escaped into the vill-age nearby!", Color(1, 1, 1), 30, 60, 70);
	}
	else if (showSecond && herox <= 800)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "I must recapture them before they cause huge chaos!", Color(1, 1, 1), 30, 70, 70);
	}
	else if (!showFirst && !showSecond)
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 30, 100, 70);

	if (herox >= 800)
		RenderTextOnScreen(meshList[GEO_TEXT], "ENTER to get in the game", Color(1, 1, 1), 30, 280, 70);
}

void SceneStory::Exit()
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