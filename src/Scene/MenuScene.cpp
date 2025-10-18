#include <Scene/MenuScene.h>
#include<core/SceneManager.h>
#include<Scene/GameScene.h>
#include<iostream>

void MenuScene::Enter(SceneManager* mgr)
{
	scene_mgr = mgr;
	std::cout << "Enter MenuScene"<<std::endl;

}

void MenuScene::Exit()
{
	std::cout << "Exit Menu" << std::endl;
}

void MenuScene::Update(float dt)
{
	delta_time += dt;
	if (delta_time >= 1.f) {
		//scene_mgr->Switch_Scene(std::make_unique<GameScene>());

	}
}
void MenuScene::Render()
{
	std::cout << "Render" << std::endl;
}



