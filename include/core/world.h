#pragma once

#include"SceneManager.h"
#include"DeltaTimer.h"

class GameWorld
{
public:
	static GameWorld& Get_Instance() {
		static GameWorld* world = new GameWorld();
		return *world;
	}

	void game_on();

private:
	GameWorld() {};

private:
	SceneManager scene_mgr;
	DeltaTimer timer;
};