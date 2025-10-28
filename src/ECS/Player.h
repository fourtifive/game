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

		ECS::Renderable walking_sprite(ECS::RenderType::Sprite,"player_walking",0,0,128,128);
		ecs.Add_Comp(PLAYER, walking_sprite);
	}

private:

};
