#pragma once
#include<core/SceneManager.h>
#include<Scene/MenuScene.h>

class MenuScene:public Scene
{
public:
    void Enter(SceneManager* mgr) override;
    void Exit() override;
    void Update(float delta_time) override;
    void Render() override;

private:
    float delta_time=0;
};
