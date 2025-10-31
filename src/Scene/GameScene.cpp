#include <Scene/GameScene.h>
#include<iostream>
#include<ECS/TransSystem.h>


void GameScene::Enter(SceneManager* mgr)
{
	scene_mgr = mgr;
	render_mgr = &RenderManager::Get_Instance();
	ecs = &ECS::ECSManager::Get_Instance();

	ecs->Register_System<ECS::TransSystem>();

	std::cout << "Enter GameScene..." << std::endl;

	player = new Player();
	player->Init();
}

void GameScene::Exit()
{
	std::cout << "Exit Game" << std::endl;
}

void GameScene::Update(float dt)
{
	ecs->Update(dt);
}
void GameScene::Render()
{
	render_mgr->Render();
}



