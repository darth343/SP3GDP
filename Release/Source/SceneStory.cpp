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
	flip = false;
	herox = -15;
	cooldown = 1.f;
	showFirst = false;
	showSecond = false;
}


void SceneStory::Update(double dt)
{

	//SharedData::GetInstance()->soundManager.SoundPlay("Sound/title.mp3", &SharedData::GetInstance()->title, 0.3f, false);

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

	if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
	{
		herox = -15;
		SharedData::GetInstance()->gameState = SharedData::GAME_S1;
		SharedData::GetInstance()->title->stop();
	}
	else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
	{
		SharedData::GetInstance()->ENTERkeyPressed = false;
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

	RenderTextOnScreen(meshList[GEO_TEXT], "ENTER to skip/get in the game", Color(1, 1, 1), 20, 280, 20);
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