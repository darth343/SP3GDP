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
	SharedData::GetInstance()->soundManager.playMusic("Sound//Menu.mp3");
}


void SceneMenu::Update(double dt)
{
	SharedData::GetInstance()->menuCoolDown -= dt;


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
	if (Application::IsKeyPressed(VK_DOWN) && !SharedData::GetInstance()->DNkeyPressed  && !renderInstructionNow && !renderStoryNow)
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
			SharedData::GetInstance()->soundManager.playSE("Sound//Click.mp3");
		}
		else if (!Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
		{
			SharedData::GetInstance()->DNkeyPressed = false;
		}
		if (Application::IsKeyPressed(VK_UP) && !SharedData::GetInstance()->UPkeyPressed && !renderInstructionNow && !renderStoryNow)
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
			SharedData::GetInstance()->soundManager.playSE("Sound//Click.mp3");
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
				move = true;
				break;
			case GS_INTRODUCTION:
				renderStoryNow = true;
				move = true;
				break;
			case GS_INSTRUCTIONS:
				renderInstructionNow = true;
				move = true;
				break;
			}
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = false;
			SharedData::GetInstance()->soundManager.playSE("Sound//Click.mp3");
		}
		if (move)
		{
			othericonx += 200 * dt;
			realizeiconx -= 200 * dt;
		}
		if (movein && realizeiconx < 20)
		{
			othericonx -= 200 * dt;
			realizeiconx += 200 * dt;
		}
		if (realizeiconx > 20)
		{
			movein = false;
		}
		if (realizeiconx <= -350)
		{
			move = false;
			switch (m_gs)
			{
			case GS_GAME:
				if (SharedData::GetInstance()->menuCoolDown <= 0)
				{
					SharedData::GetInstance()->gameState = SharedData::STORY;
					SharedData::GetInstance()->menuCoolDown = 3;
				}
				movein = true;

				m_gs = GS_GAME;
				break;

			case GS_INSTRUCTIONS:
				if (renderInstructionNow && Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = true;
				}
				else if (renderInstructionNow && !Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = false;
					renderInstructionNow = false;
					m_gs = GS_GAME;
				}
				break;
			case GS_INTRODUCTION:
				if (renderStoryNow && Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = true;
				}
				else if (renderStoryNow && !Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = false;
					renderStoryNow = false;
					m_gs = GS_GAME;
				}
				break;
				//Other cases here
			}
		}

	fps = (float)(1.f / dt);

}

void SceneMenu::RenderInstruction()
{
	RenderBackground(meshList[GEO_INSTRUCTIONBG]);
}

void SceneMenu::RenderStory()
{
	RenderBackground(meshList[GEO_INTRODUCTION]);
}

void SceneMenu::Render()
{
	SceneBase::Render();
	RenderBackground(meshList[GEO_STORYBACKGROUND]);
	int y = 380;

	Render2DMeshWScale(meshList[GEO_LOGO], false, 350, 100, realizeiconx, 500, false,2);
	Render2DMeshWScale(meshList[GEO_MENUPLAY], false, 150, 60, othericonx, y, false, 2);
	Render2DMeshWScale(meshList[GEO_MENUSTORY], false, 170, 60, othericonx, y - 70, false, 2);
	Render2DMeshWScale(meshList[GEO_MENUINST], false, 270, 60, othericonx, y - 140, false, 2);
	Render2DMeshWScale(meshList[GEO_MENUOPT], false, 200, 60, othericonx, y - 210, false, 2);
	Render2DMeshWScale(meshList[GEO_MENUQUIT], false, 150, 60, othericonx, y - 280, false, 2);
	
	if (renderInstructionNow)
		RenderInstruction();
	if (renderStoryNow)
		RenderStory();
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