#pragma once
#include<memory>
#include"Scene.h"

class SceneManager
{
public:
	void Switch_Scene(std::unique_ptr<Scene> scene)
	{
		if(current_scene)current_scene->Exit();
		current_scene = std::move(scene);
		if (current_scene)current_scene->Enter(this);
	}

	bool Is_Empty()
	{
		if (current_scene)return false;
	}

	Scene* Get_current_Scene()
	{
		return current_scene.get();
	}

	void Update(float delta_time)
	{
		current_scene->Update(delta_time);
	}

	void Render()
	{
		current_scene->Render();
	}

private:
	std::unique_ptr<Scene> current_scene;

};
