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
			//std::cout << "TransSystem Update" << std::endl;
			ecs_mgr.Traverse<ECS::Translate,ECS::Physical,ECS::State>([this,delta_time](Entity id,InputManager& input_mgr
				,ECS::Translate& trans
				,ECS::Physical& phy
				,ECS::State& state) {
					HandleMoveMent(delta_time,input_mgr,trans,phy,state);
				});
		}
	private:
		void HandleMoveMent(float delta_time,InputManager& input_mgr, ECS::Translate& trans, ECS::Physical& phy,ECS::State& state)
		{
			float move = 0.0f, max = phy.maxSpeed;
			bool is_attack=0;
			if (state.currentState.find("attack") != std::string::npos) {
				is_attack = 1;
				if (state.previousState == "player_run")phy.velocity.x *= phy.runMutiplier;
			}
			else if (state.currentState == "player_walking") {
				move = (state.isFacingRight ? 1.0f : -1.0f);
			}
			else if (state.currentState == "player_run") {
				move = (state.isFacingRight ? 1.0f : -1.0f)*phy.runMutiplier;
				max = phy.maxSpeed * phy.runMutiplier;
			}

            if (std::abs(move) > 0.1f)phy.velocity.x += move * max*phy.acceleration;
            else if(!is_attack) phy.velocity.x = 0;

			trans.isfacingright = state.isFacingRight;

			if (std::abs(phy.velocity.x) > max ) {
				phy.velocity.x = (phy.velocity.x > 0) ? max: -max;
			}
			
			if(is_attack) trans.position.x += phy.velocity.x*0.35f;
			else trans.position.x += phy.velocity.x;
			//std::cout << phy.velocity.x << std::endl;
		}
	};
}