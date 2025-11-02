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
					UpdateState(state);
				});

		}
		
	private:

		void UpdateState(ECS::State& state) {
			if (state.pendingStateChange) {
				state.previousState = state.currentState;
				state.currentState = state.requestedState;
				state.stateTimer = 0.0f;
				state.pendingStateChange = false;
			}
			EnterState(state);
		}

		void RequestPlayerState(Entity entity, State& state, Physical& physics,
			float delta_time, InputManager& input_mgr) {

			state.stateTimer += delta_time;

			if (!state.isStateLocked) {
				HandleStateChange(physics,state, input_mgr);
			}
		}

		void HandleStateChange(ECS::Physical& physical, ECS::State& state,InputManager& input) {

			if (physical.velocity.x > 0.0f || physical.velocity.x < 0.0f) {
				if (input.IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
					TryChangeState(state, "player_run");
				}
				else TryChangeState(state, "player_walking");
			}
			else {
				TryChangeState(state,"player_idle");
			}
			
		}

		void TryChangeState(ECS::State& state,std::string newstate) {
			if (state.currentState != newstate) {
				state.RequestStateChange(newstate);
			}
		}

		void EnterState(ECS::State& state) {
			//std::cout << "Entering State: " << state.currentState << std::endl;
		}

	};

}