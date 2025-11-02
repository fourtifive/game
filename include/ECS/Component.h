#pragma once
#include<core/Math.h>


namespace ECS {
	struct Translate
	{
		Point position;
		Point scale = {1,1};
		Point rotation;

        bool isfacingright = true;
	};
	
	struct Physical
	{
		Point velocity;
		Point acceleration;
		float maxSpeed = 0.0f;
		float runMutiplier=1.5f;

		float mass = 1.0f;
		float gravity = 0.0f;
		//float friction;
		
	};
	
	struct PlayerControlled
	{};

	enum class RenderType {
		SingleImage,
		Sprite
	};

	struct RenderData {
		RenderData() = default;

		RenderData(RenderType t, std::string id, int width, int height) :
			type(t), Id(id), srcWidth(width), srcHeight(height)
		{}

		RenderType type;          // render type
		std::string Id;     // ID of the sprite/image to render

		int srcWidth = 0;
		int srcHeight = 0;

	};

	struct AnimationData
	{
		AnimationData() = default;
		AnimationData(int totalFrames, int frameDelay,int x,int y, int spriteWidth, int spriteHeight) :
			totalFrames(totalFrames), frameDelay(frameDelay),srcX(x),srcY(y), spriteWidth(spriteWidth), spriteHeight(spriteHeight)
		{}

		// For sprite rendering
		int currentFrame = 0;
		int totalFrames = 1;
		int frameDelay = 100; // milliseconds
		int elapsedTime = 0; // milliseconds

		int spriteWidth = 0;  // width of a single frame
		int spriteHeight = 0; // height of a single frame

		bool isfacingright = true;
		bool isloop = true;

		int srcX = 0;
		int srcY = 0;
	};

	struct State{
		std::string currentState = "IDLE";      
		std::string previousState = "IDLE";     
		float stateTimer = 0.0f;                

		bool isStateLocked = false;             
		bool canReceiveInput = true;

		void RequestStateChange(const std::string& newState) {
			if (!pendingStateChange) {
				requestedState = newState;
				pendingStateChange = true;
			}
		}

		bool IsInState(const std::string& state) const {
			return currentState == state;
		}

	private:
		std::string requestedState;
		bool pendingStateChange = false;
	};

}

