#pragma once
#include<core/SceneManager.h>
#include<ECS/ECSManager.h>
#include<core/Scene.h>
#include<ECS/Player.h>
#include<core/RenderManager.h>

class GameScene:public Scene
{
public:
	void Update(float delta_time) override;
	void Render(GLFWwindow* window) override;
	void Exit() override;
	void Enter(SceneManager* scene_mgr) override; // Ensure parameter name matches base class declaration
private:
	ECS::ECSManager* ecs = nullptr;
	Player* player = nullptr;
	RenderManager* render_mgr = nullptr;
};