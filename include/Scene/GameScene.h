#pragma once
#include<Scene/GameScene.h>
#include<core/SceneManager.h>

class GameScene:public Scene
{
public:
	void Update(float delta_time)override;
	void Render()override;
	void Exit()override;
	void Enter(SceneManager* mgr)override;
private:
	float delta_time=0;
};