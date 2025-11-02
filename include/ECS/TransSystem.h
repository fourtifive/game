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
			ecs_mgr.Traverse<ECS::Translate>([delta_time](Entity id,InputManager& input_mgr, Translate& comp) {
					if(input_mgr.IsKeyDown(GLFW_KEY_D)) comp.position.x += 5;
					if (input_mgr.IsKeyDown(GLFW_KEY_A)) comp.position.x -= 5; 
					if(input_mgr.IsKeyDown(GLFW_KEY_W)) comp.position.y += 5;
					if(input_mgr.IsKeyDown(GLFW_KEY_S)) comp.position.y -= 5;
				});
		}
	private:

	};


}