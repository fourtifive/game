#pragma once
#include<ECS/System.h>
#include<ECS/ComponentStorage.h>
#include<ECS/ECSManager.h>

namespace ECS{
	class SpriteAnimationSystem :public System 
	{
	public:
		SpriteAnimationSystem() = default;
		~SpriteAnimationSystem() = default;

		void Update(ECSManager& ecs_mgr, float deltaTime) {

		}

	};
}