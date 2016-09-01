#include "SharedData.h"
#include "Application.h"
#include "SceneGame.h"

void SharedData::Reset()
{
	enemyCatchPercentage = 0;
	menuCoolDown = 0;
	playerLives = 3;
	//inventory.ResetInventory();
	player->Reset();
	Application::GetInstance().GetScene5()->Reset();
	Application::GetInstance().GetScene4()->Reset();
	Application::GetInstance().GetScene3()->Reset();
	Application::GetInstance().GetScene2()->Reset();
	Application::GetInstance().GetScene1()->Reset();
	SharedData::GetInstance()->player->SetPosition(Vector3(530, 64, 0));
	SharedData::GetInstance()->player->SetMapOffset(Vector3(0, 0, 0));
}