#pragma once
#include<core/world.h>
#include<graphics.h>
#include<iostream>
#include <core/Scene.h>
#include <Scene/MenuScene.h>

GameWorld& GameWorld::Get_Instance()
{
	static GameWorld world;
	return world;
}

void GameWorld::game_on()
{
	//initgraph(1280, 720, EX_SHOWCONSOLE);
	
	BeginBatchDraw();

	scene_mgr.Switch_Scene(std::make_unique<MenuScene>());

	while (!scene_mgr.Is_Empty() && !scene_mgr.Get_current_Scene()->Is_Quit())
	{
			float delta_time = timer.Updata();
			
			scene_mgr.Update(delta_time);

			//scene_mgr.Render();


		cleardevice();
		
		FlushBatchDraw();
	}

	EndBatchDraw();

}

bool GameWorld::Init() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Demo", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	input_mgr.Init_Inputmgr(window);


	return 1;
}

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}
