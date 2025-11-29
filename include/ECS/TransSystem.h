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
			float move = 0.0f, max=0;
			if (state.currentState.find("attack") != std::string::npos) {
				max=HandleAttcackMovement(delta_time, input_mgr, trans, phy, state);
			}
			else if (state.currentState=="player_jump") {
				max=HandleJumpMovement(delta_time, input_mgr, trans, phy, state);
			}
			else if (state.currentState == "player_walking") {
				max = phy.maxSpeed;
			}
			else if (state.currentState == "player_run"){
				max = phy.runSpeed;
			}

			move = (state.isFacingRight ? 1.0f : -1.0f);
            if (std::abs(move) > 0.1f)phy.velocity.x += move * phy.acceleration*delta_time;

			trans.isfacingright = state.isFacingRight;

			if (std::abs(phy.velocity.x) > max ) {
				phy.velocity.x = (phy.velocity.x > 0) ? max: -max;
			}
			
			trans.position.x += phy.velocity.x;
			trans.position.y += phy.velocity.y;
			//std::cout << max <<" "<<delta_time <<" "<<phy.velocity.x << std::endl;
		}

		float HandleAttcackMovement(float delta_time, InputManager& input_mgr, ECS::Translate& trans, ECS::Physical& phy, ECS::State& state)
		{
			if (state.previousState == "player_walking")
				return phy.attackSpeed;
			else if (state.previousState == "player_run")
				return phy.runToattackSpeed;
			else if (state.previousState == "player_jump")
				return 0;
			return 0;
		}

		float HandleJumpMovement(float delta_time, InputManager& input_mgr, ECS::Translate& trans, ECS::Physical& phy, ECS::State& state)
		{
			//phy.velocity.y = 2;

			if (state.previousState == "player_walking")
				return phy.jumpSpeed;
			else if (state.previousState == "player_run")
				return phy.runTojumpSpeed;
			else if (state.previousState.find("attack") != std::string::npos)
				return 0;
			return 0;
		}
	};
}