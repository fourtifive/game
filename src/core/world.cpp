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

	while (!scene_mgr.Is_Empty() && !scene_mgr.Get_current_Scene()->Is_Quit())
	{
		
			timer.Start_Frame();
			
			input_mgr.Update(timer.Get_Delta());

			scene_mgr.Update(timer.Get_Delta());

			scene_mgr.Render();

			timer.End_frame();
			
	}

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

	render_mgr.Init(resource_mgr);

	scene_mgr.Init(std::make_unique<GameScene>());

	return 1;
}