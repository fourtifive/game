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
			ecs_mgr.Traverse<ECS::Translate,ECS::Physical>([this,delta_time](Entity id,InputManager& input_mgr,ECS::Translate& trans,ECS::Physical& phy) {
					HandleMoveMent(delta_time,input_mgr,trans,phy);
				});
		}
	private:
		void HandleMoveMent(float delta_time,InputManager& input_mgr, ECS::Translate& trans, ECS::Physical& phy)
		{
            /*float move_input = 0.0f;
            if (input_mgr.IsKeyDown(GLFW_KEY_A)) move_input -= 1.0f;
            if (input_mgr.IsKeyDown(GLFW_KEY_D)) move_input += 1.0f;

            float target_speed = move_input * phy.maxSpeed;  

            float speed_diff = target_speed - phy.velocity.x;
            float acceleration = speed_diff * phy.acceleration * delta_time;

            phy.velocity.x += acceleration*move_input;

            if (std::abs(phy.velocity.x) > phy.maxSpeed) {
                phy.velocity.x = (phy.velocity.x > 0) ? phy.maxSpeed : -phy.maxSpeed;
            }

            trans.position.x += phy.velocity.x * delta_time;

            if (move_input > 0.1f) {
                trans.isfacingright = true;
            }
            else if (move_input < -0.1f) {
                trans.isfacingright = false;
            }
			
			std::cout << "velocity x: " << phy.velocity.x << std::endl;*/

			if (input_mgr.IsKeyDown(GLFW_KEY_A)) trans.position.x-=5;
			if (input_mgr.IsKeyDown(GLFW_KEY_D))trans.position.x+=5;

		}
	};


}