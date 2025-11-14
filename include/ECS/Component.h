#pragma once
#include<core/Math.h>


namespace ECS {
	struct Translate
	{
		Point position;
		Point scale = {1,1};
		Point rotation = {0,0};

        bool isfacingright = true;
	};
	
	struct Physical
	{
		Physical() = default;
		Physical(float maxSpeed, float acceleration,float deceleration) : maxSpeed(maxSpeed),acceleration(acceleration),deceleration(deceleration)
		{}
		Point velocity = {0,0};
		float acceleration;
		float deceleration;
		float maxSpeed;
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

		RenderData(RenderType t, std::string id) :
			type(t), Id(id)
		{}

		RenderType type;          // render type
		std::string Id;     // ID of the sprite/image to render

		int srcWidth = 0;           // width to render
		int srcHeight = 0;          // height to render

	};

	struct AnimationConfig
	{
		AnimationConfig() = default;
		AnimationConfig(int x, int y, int spriteWidth, int spriteHeight, int totalFrames, int frameDelay, int width, int height) :
			srcX(x), srcY(y), spriteWidth(spriteWidth), spriteHeight(spriteHeight), totalFrames(totalFrames), frameDelay(frameDelay), srcWidth(width), srcHeight(height)
		{}
		int spriteWidth;  // width of a single frame
		int spriteHeight; // height of a single frame
		int totalFrames;
		int frameDelay; // milliseconds
		//bool isfacingright = true;
		//bool isloop = true;
		int srcWidth;
		int srcHeight;
		int srcX = 0;
		int srcY = 0;
	};

	struct AnimationData
	{
		AnimationData() = default;
		AnimationData(std::string id) : 
			animID(id)
		{}

		std::string animID;

		std::unordered_map<std::string, AnimationConfig> animations;

		// For sprite rendering
		int currentFrame = 0;
		float elapsedTime = 0; // milliseconds
	};

	struct State{
		State(std::string current) :
			currentState(current)
		{}
		State() = default;
		std::string currentState;
		std::string previousState;     
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
		std::string requestedState;
		bool pendingStateChange = false;
	};

	enum CollisionLayer
	{
		PLAYER = 1,
		ENEMY = 2,
		GROUND = 4,
		WALL = 8
	};

	struct ColliderBox
	{
		ColliderBox() = default;
		ColliderBox(Entity id,float width, float height, CollisionLayer layer, int mask) :
			owner(id), width(width), height(height), layer(layer), mask(mask)
		{}
		float width;
		float height;
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		float xL, xR, yU,yD; // AABB bounds

		CollisionLayer layer;
		int mask;

		bool is_trigger = false;
		bool is_active = true;
		bool is_static = false;

		Entity owner; // Reference to the owning entity
	};

	struct CollisionEvent
	{

	};

	struct MapData
	{
		Point map_size = { 2000.0f,1000.0f };
		Point start;
		Point end;
		std::vector<Entity>TerrainChunks;
	};
}

