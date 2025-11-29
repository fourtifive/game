#pragma once
#include<ECS/ECSManager.h>

namespace ECS {
	class StateSystem:public System
	{
	public:
		StateSystem() = default;
		~StateSystem() = default;

		void Update(ECS::ECSManager& ecs_mgr, float delta_time) {
			ecs_mgr.Traverse<ECS::State, ECS::PlayerControlled, ECS::Physical>([this, delta_time](Entity id, InputManager& intput,
				ECS::State& state,
				ECS::PlayerControlled& player,
				ECS::Physical& physical) {
					RequestPlayerState(id, state, physical,delta_time,intput);
				});

			ecs_mgr.TraverseNoInput<ECS::State>([this, delta_time](Entity id,ECS::State& state) {
					UpdateState(state,delta_time);
				});

		}
		
	private:

		void UpdateState(ECS::State& state,float deltatime) {
			if (state.pendingStateChange) {
				state.previousState = state.currentState;
				state.currentState = state.requestedState;
				state.stateTimer = 0.0f;
				state.pendingStateChange = false;

				if (state.currentState.find("attack") != std::string::npos) {
					state.isStateLocked = true;
				}
			}

			if (state.currentState.find("attack") != std::string::npos)Attackupdata(state, deltatime);
			EnterState(state);
		}

		void Attackupdata(ECS::State& state, float delta_time) {
		
			if (state.stateTimer > GetAttackDuration(state.currentState)) {
				state.isStateLocked = false;
				if (state.attackCombo == 3) state.attackCombo = 0;
			}

			if (state.attackCombo > 0) {
				if (state.comboTimer >= state.COMBORESET) {
					state.attackCombo = 0;
					state.comboTimer = 0.0f;
					state.isStateLocked = false;
				}
			}
		}

		void RequestPlayerState(Entity entity, State& state, Physical& physics,
			float delta_time, InputManager& input_mgr) {

			if(state.currentState.find("attack") == std::string::npos)state.comboTimer += delta_time;
			state.stateTimer += delta_time;

			if (state.currentState == "player_jump") {
				if (input_mgr.IsMouseButtonDown(0)) {
					HandleAttackState(physics, state, input_mgr);
					return;
				}
				if (input_mgr.IsKeyDown(GLFW_KEY_SPACE)) {
					if (state.stateTimer >= 0.25f) state.isfalling = true;
					return;
				}
				else(!input_mgr.IsKeyDown(GLFW_KEY_SPACE)) {
					if (state.stateTimer >= 0.05f) state.isfalling = true;
					return;
				}
			}

			if (!state.isStateLocked&&state.stateTimer>=0.1f) {
				HandleStateChange(physics,state, input_mgr,delta_time);
			}
		}

		void HandleStateChange(ECS::Physical& physical, ECS::State& state,InputManager& input,float deltatime) {

			if (input.IsMouseButtonDown(0)) {
				HandleAttackState(physical, state, input);
				return;
			}

			if(input.IsKeyDown(GLFW_KEY_SPACE)){
				TryChangeState(state, "player_jump");
				return;
			}

			bool movingRight = input.IsKeyDown(GLFW_KEY_D);
			bool movingLeft = input.IsKeyDown(GLFW_KEY_A);
			bool wantsRun = input.IsKeyDown(GLFW_KEY_LEFT_SHIFT);

			if (movingRight || movingLeft) {
				std::string moveState = wantsRun ? "player_run" : "player_walking";
				TryChangeState(state, moveState);
				state.isFacingRight = movingRight; 
			}
			else {
				TryChangeState(state, "player_idle");
			}

		}

		void TryChangeState(ECS::State& state,std::string newstate) {
			if (state.currentState != newstate) {
				state.RequestStateChange(newstate);
			}
		}

		void HandleAttackState(ECS::Physical& physical, ECS::State& state, InputManager& input) {
			switch (state.attackCombo)
			{
			case 0:
				TryChangeState(state, "player_attack_1");
				state.attackCombo = 1;
				break;
			case 1:
				if (state.comboTimer < state.COMBORESET) {
					TryChangeState(state, "player_attack_2");
					state.attackCombo = 2;
				}
				break;
			case 2:
				if (state.comboTimer < state.COMBORESET) {
					TryChangeState(state, "player_attack_3");
					state.attackCombo = 3;
				}
				break;
			case 3:
				TryChangeState(state, "player_attack_1");
				state.attackCombo = 1;
				break;
			default:
				state.attackCombo = 1;
				TryChangeState(state, "player_attack_1");
				break;
			}
			state.comboTimer = 0.0f;
		}

		void EnterState(ECS::State& state) {
			std::cout << "Entering State: " << state.currentState << std::endl;
		}

		float GetAttackDuration(const std::string& attackState) {
			if (attackState == "player_attack_1") return 0.35f;
			if (attackState == "player_attack_2") return 0.35f;
			if (attackState == "player_attack_3") return 0.40f;
			return 0.3f;
		}

	};

}