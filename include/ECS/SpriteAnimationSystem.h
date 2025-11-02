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

			ecs_mgr.Traverse<ECS::AnimationData, ECS::RenderData,ECS::State>([this,deltaTime](Entity id,InputManager& input,ECS::AnimationData& anim, ECS::RenderData& renderdata,ECS::State& state)
				{
					if (renderdata.Id != state.currentState) {
						anim.animations[renderdata.Id].srcX = 0;
						anim.animations[renderdata.Id].srcY = 0;
						anim.animID = state.currentState;
						renderdata.Id = state.currentState;
						anim.currentFrame = 0.0f;
						anim.elapsedTime = 0.0f;
						UpdataSpriteFrame(anim);
						std::cout << "id " << renderdata.Id << std::endl;
					}
					anim.elapsedTime += deltaTime * 1000;
					
					//std::cout << "id " << renderdata.Id << " ElapsedTime: " << anim.elapsedTime << std::endl;

					if (anim.elapsedTime >= anim.animations[anim.animID].frameDelay) {
							anim.currentFrame = (anim.currentFrame + 1) % anim.animations[anim.animID].totalFrames;
							anim.elapsedTime = 0;
							UpdataSpriteFrame(anim);
							//std::cout << "id " << renderdata.Id << " Frame: " << anim.currentFrame << std::endl;
					}

				});
		}
	private:
		
		void UpdataSpriteFrame(ECS::AnimationData& anim) {
			anim.animations[anim.animID].srcX = anim.currentFrame * anim.animations[anim.animID].spriteWidth;
			anim.animations[anim.animID].srcY = 0;
		}

	};
}