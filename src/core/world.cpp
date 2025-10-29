#pragma once
#include<core/world.h>
#include<graphics.h>
#include<iostream>
#include <Scene/MenuScene.h>
#include<Scene/GameScene.h>


GameWorld& GameWorld::Get_Instance()
{
	static GameWorld world;
	return world;
}

void GameWorld::game_on()
{
	//initgraph(1280, 720, EX_SHOWCONSOLE);
	
	//BeginBatchDraw();

	/*scene_mgr.Switch_Scene(std::make_unique<GameScene>());*/

	while (!scene_mgr.Is_Empty() && !scene_mgr.Get_current_Scene()->Is_Quit())
	{
		
		// 添加window有效性检查


			timer.Start_Frame();
			
			input_mgr.Update(timer.Get_Delta());

			scene_mgr.Update(timer.Get_Delta());

			scene_mgr.Render();

			timer.End_frame();
			
	}

	//EndBatchDraw();

}

bool GameWorld::Init() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Demo", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewInit();

	input_mgr.Init_Inputmgr(window);

	timer.Init();

	resource_mgr.Init();

	scene_mgr.Init(std::make_unique<GameScene>());

	return 1;
}

//GameWorld::GameWorld()
//{
//}
//
//GameWorld::~GameWorld()
//{
//}
