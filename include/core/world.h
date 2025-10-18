#pragma once

#include"SceneManager.h"
#include<glfw3.h>
#include"Timer.h"
#include"InuputManager.h"

class GameWorld
{
public:
	static GameWorld& Get_Instance();

	void game_on();

	bool Init();

	GameWorld()=default;
	~GameWorld()=default;

private:
	SceneManager scene_mgr;
	InputManager input_mgr;
	Timer timer;
};