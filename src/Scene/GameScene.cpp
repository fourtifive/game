#include <Scene/GameScene.h>
#include<iostream>
#include<ECS/TransSystem.h>
#include<ECS/StateSystem.h>
#include<ECS/SpriteAnimationSystem.h>
#include<ECS/CollisionSystem.h>
#include<ECS/TerrainSystem.h>

void GameScene::Enter(SceneManager* mgr)
{
	scene_mgr = mgr;
	render_mgr = &RenderManager::Get_Instance();
	ecs = &ECS::ECSManager::Get_Instance();

	ecs->Register_System<ECS::TransSystem>();
	ecs->Register_System<ECS::StateSystem>();
	ecs->Register_System<ECS::SpriteAnimationSystem>();
	ecs->Register_System<ECS::CollisionSystem>();
	ecs->Register_System<ECS::TerrainSystem>()->GenerateTerrain(*ecs,1280,800);


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
void GameScene::Render(GLFWwindow* window)
{
	render_mgr->Render(window);
}



