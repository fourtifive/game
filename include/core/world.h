#pragma once

#include"SceneManager.h"
#include<glfw3.h>
#include"Timer.h"
#include"InuputManager.h"

class GameWorld
{
public:
	static GameWorld& Get_Instance() {
		static GameWorld* world = new GameWorld();
		return *world;
	}

	void game_on();

	bool Init();

private:
	GameWorld() {};

private:
	SceneManager scene_mgr;
	InputManager input_mgr;
	Timer timer;
};