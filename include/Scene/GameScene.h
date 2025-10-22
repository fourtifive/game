#pragma once
#include<core/SceneManager.h>
#include<ECS/ECSManager.h>
#include<core/Scene.h>
#include<ECS/Player.h>

class GameScene:public Scene
{
public:
	void Update(float delta_time)override;
	void Render()override;
	void Exit()override;
	void Enter(SceneManager* mgr)override;
private:
	//float delta_time=0;

	Player* player=nullptr;
};