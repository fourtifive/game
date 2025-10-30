#pragma
#include<ECS/ECSManager.h>
#include<ECS/Component.h>

class Player
{
public:
	
	void Init() {
		auto& ecs = ECS::ECSManager::Get_Instance();

		ecs.Create_Entity(PLAYER);

		ecs.Add_Comp(PLAYER, { 0,0 });

		ECS::RenderData player_walking_sprite(ECS::RenderType::Sprite,"player_walking",1152,128);
		ECS::AnimationData player_walking_animation(9,100,0,0,128,128);
		ecs.Add_Comp(PLAYER, player_walking_sprite);
		ecs.Add_Comp(PLAYER, player_walking_animation);

	}

private:

};
