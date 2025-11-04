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
            float move = 0.0f;
            if (input_mgr.IsKeyDown(GLFW_KEY_A)) move = -1.0f;
            if (input_mgr.IsKeyDown(GLFW_KEY_D)) move = 1.0f;
			if (input_mgr.IsKeyDown(GLFW_KEY_LEFT_SHIFT)) move *= phy.runMutiplier;

            if (std::abs(move) > 0.1f)phy.velocity.x += move * phy.maxSpeed*phy.acceleration;
            else phy.velocity.x -= phy.velocity.x*phy.deceleration;

			if (move > 0.1f) trans.isfacingright = true;
			else if (move < -0.1f) trans.isfacingright = false;

			if (std::abs(phy.velocity.x) > phy.maxSpeed ) {
				phy.velocity.x = (phy.velocity.x > 0) ? phy.maxSpeed : -phy.maxSpeed;
			}
			if(phy.velocity.x>=1.0f|| phy.velocity.x <= -1.0f)
				trans.position.x += phy.velocity.x;
			std::cout << trans.position.x << "\n";
		}
	};
}