
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

#include "SharedData.h"
#include "SceneText.h"
#include "Scene1.h"
#include "Scene2.h"
#include "SceneMenu.h"
#include "SceneStory.h"
#include "SceneBoss.h"
#include "Scene4.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	bool GetMouseUpdate();
	bool GetKeyboardUpdate();
	int GetScreenHeight();
	int GetScreenWidth();
	Scene* GetScene();
	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

	

private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	const static int m_window_deadzone = 100;
	const static int m_window_width = 1024;
	const static int m_window_height = 800;
	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

	// The handler for the scene

	Scene *scene;

};

#endif