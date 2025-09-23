#pragma once

class SceneManager;

class Scene
{
public:
	virtual void Update(float delta_time)=0;
	virtual void Render()=0;
	virtual void Enter(SceneManager* scene_mgr) = 0;
	virtual void Exit() = 0;
	virtual bool Is_Quit() { return false; }
protected:
	SceneManager* scene_mgr=nullptr;
};