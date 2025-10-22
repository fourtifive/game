#pragma once

#include"SceneManager.h"
#include<glfw3.h>
#include"Timer.h"
#include<ECS/ECSManager.h>

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
	InputManager& input_mgr=InputManager::Get_Instance();
	/*ECS::ECSManager& ecs_mgr = ECS::ECSManager::Get_Instance();*/
	GLFWwindow* window=nullptr;
	Timer timer;
};