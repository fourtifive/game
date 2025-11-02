#pragma
#include<ECS/ECSManager.h>
#include<ECS/Component.h>

class Player
{
public:
	
	void Init() {
		auto& ecs = ECS::ECSManager::Get_Instance();

		ecs.Create_Entity(PLAYER);

		ecs.Add_Comp<ECS::PlayerControlled>(PLAYER, ECS::PlayerControlled());
		ecs.Add_Comp<ECS::Translate>(PLAYER, { 0,0 });
		ecs.Add_Comp<ECS::Physical>(PLAYER,ECS::Physical(400,200.0f,-200.0f));
		ecs.Add_Comp<ECS::RenderData>(PLAYER,ECS::RenderData(ECS::RenderType::Sprite,"player_idle"));
		ecs.Add_Comp(PLAYER,ECS::AnimationData("player_idle"));
		auto& comp=ecs.Get_Component<ECS::AnimationData>(PLAYER);

		comp.animations["player_idle"] = {0,0,128,128,6,200,768,128};
		comp.animations["player_run"] = {0,0,128,128,8,120,1024,128};
		comp.animations["player_walking"] = {0,0,128,128,9,100,1152,128};

		ecs.Add_Comp<ECS::State>(PLAYER, ECS::State("player_idle"));
	}

private:

};
