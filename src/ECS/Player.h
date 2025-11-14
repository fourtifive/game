#pragma
#include<ECS/ECSManager.h>
#include<ECS/Component.h>
#include<ECS/Entity.h>

class Player
{
public:
	
	void Init() {
		auto& ecs = ECS::ECSManager::Get_Instance();

		const ECS::Entity PLAYER = ecs.Create_Entity();

		ecs.Add_Comp<ECS::PlayerControlled>(PLAYER, ECS::PlayerControlled());
		ecs.Add_Comp<ECS::Translate>(PLAYER, { 0,40 });
		ecs.Add_Comp<ECS::Physical>(PLAYER,ECS::Physical(2,0.1f,0.3f));
		ecs.Add_Comp<ECS::RenderData>(PLAYER,ECS::RenderData(ECS::RenderType::Sprite,"player_idle"));
		ecs.Add_Comp(PLAYER,ECS::AnimationData("player_idle"));
		auto& comp=ecs.Get_Component<ECS::AnimationData>(PLAYER);

		comp.animations["player_idle"] = {0,0,128,128,6,160,768,128};
		comp.animations["player_run"] = {0,0,128,128,8,125,1024,128};
		comp.animations["player_walking"] = {0,0,128,128,9,111,1152,128};

		ecs.Add_Comp<ECS::State>(PLAYER, ECS::State("player_idle"));

		ecs.Add_Comp<ECS::ColliderBox>(PLAYER, ECS::ColliderBox(PLAYER,128,128,ECS::CollisionLayer::PLAYER
			,ECS::ENEMY|ECS::GROUND|ECS::WALL));

	}

private:

};