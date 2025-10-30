#pragma once
#include<ECS/System.h>
#include<ECS/ComponentStorage.h>
#include<ECS/ECSManager.h>

namespace ECS {
	class TransSystem:public System
	{
	public:
		TransSystem()=default;
		~TransSystem()=default;

		void Update(ECS::ECSManager& ecs_mgr,float delta_time) {
			ECSManager::Get_Instance().Traverse_Eachtrans([delta_time](Entity id, Translate& comp,InputManager& input_mgr) {
					if(input_mgr.IsKeyDown(GLFW_KEY_D)) comp.position.x += 2 * delta_time;
					if(input_mgr.IsKeyDown(GLFW_KEY_A)) comp.position.x -= 2 * delta_time;
					if(input_mgr.IsKeyDown(GLFW_KEY_W)) comp.position.y += 2 * delta_time;
					if(input_mgr.IsKeyDown(GLFW_KEY_S)) comp.position.y -= 2 * delta_time;
				});
		}
	private:

	};


}