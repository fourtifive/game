#pragma once

#include"ResourceManager.h"
#include"SceneManager.h"
#include"RenderManager.h"
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
	ResourceManager& resource_mgr = ResourceManager::Get_Instance();
	RenderManager& render_mgr = RenderManager::Get_Instance();

	/*ECS::ECSManager& ecs_mgr = ECS::ECSManager::Get_Instance();*/
	GLFWwindow* window=nullptr;
	Timer timer;
};