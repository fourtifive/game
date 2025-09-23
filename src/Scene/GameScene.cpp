#include <Scene/GameScene.h>
#include<core/SceneManager.h>
#include<Scene/MenuScene.h>
#include<iostream>

void GameScene::Enter(SceneManager* mgr)
{
	scene_mgr = mgr;
	std::cout << "Enter GameScene" << std::endl;
}

void GameScene::Exit()
{
	std::cout << "Exit Game" << std::endl;
}

void GameScene::Update(float dt)
{
	delta_time += dt;
	if (delta_time >= 1.f) {
		scene_mgr->Switch_Scene(std::make_unique<MenuScene>());
	}
}
void GameScene::Render()
{
	std::cout << "Render" << std::endl;
}



