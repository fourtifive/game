#include <Scene/GameScene.h>
#include<core/SceneManager.h>
#include<Scene/MenuScene.h>
#include<iostream>
#include<ECS/Player.h>
#include<ECS/TransSystem.h>


void GameScene::Enter(SceneManager* mgr)
{
	scene_mgr = mgr;
	ECS::ECSManager::Get_Instance().Register_System<ECS::TransSystem>();
	/*std::cout << "Enter GameScene" << std::endl;*/

	Player player;

}

void GameScene::Exit()
{
	std::cout << "Exit Game" << std::endl;
}

void GameScene::Update(float dt)
{
	delta_time += dt;
	if (delta_time >= 1.f) {
		//scene_mgr->Switch_Scene(std::make_unique<MenuScene>());
		ECS::ECSManager::Get_Instance().Update(dt);
		
	}
}
void GameScene::Render()
{

	/*std::cout << "Render" << std::endl;*/
}



