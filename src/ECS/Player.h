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
		//ecs.Add_Comp<ECS::Translate>(PLAYER, { 0,40 });
		ecs.Add_Comp<ECS::Physical>(PLAYER,ECS::Physical(2,0.1f,0.3f));
		ecs.Add_Comp<ECS::RenderData>(PLAYER,ECS::RenderData(ECS::RenderType::Sprite,"player_idle"));
		ecs.Add_Comp(PLAYER,ECS::AnimationData("player_idle"));
		auto& comp=ecs.Get_Component<ECS::AnimationData>(PLAYER);

		/*comp.animations["player_idle"] = {0,0,128,128,6,160,768,128};
		comp.animations["player_run"] = {0,0,128,128,8,125,1024,128};
		comp.animations["player_walking"] = {0,0,128,128,9,111,1152,128};
		comp.animations["player_jump"] = { 0,0,128,128,9,70,1152,128 };
		comp.animations["player_attack_1"] = {0,0,128,128,4,87,512,128};
		comp.animations["player_attack_2"] = { 0,0,128,128,4,87,512,128 };
		comp.animations["player_attack_3"] = { 0,0,128,128,5,80,640,128 };*/

		ecs.Add_Comp<ECS::Translate>(PLAYER, { 0,40,1.5f,1.5f} );
		comp.animations["player_idle"] = { 0,0,96,84,7,142,672,84 };
		comp.animations["player_run"] = { 0,0,96,84,8,125,768,84 };
		comp.animations["player_walking"] = { 0,0,96,84,8,125,768,84 };
		comp.animations["player_jump"] = { 0,0,96,84,5,200,480,84 };
		comp.animations["player_attack_1"] = { 0,0,96,84,6,50,576,84 };
		comp.animations["player_attack_2"] = { 0,0,96,84,5,60,480,84 };
		comp.animations["player_attack_3"] = { 0,0,96,84,6,66,576,84 };

		ecs.Add_Comp<ECS::State>(PLAYER, ECS::State("player_idle"));

		ecs.Add_Comp<ECS::ColliderBox>(PLAYER, ECS::ColliderBox(PLAYER,128,128,ECS::CollisionLayer::PLAYER
			,ECS::ENEMY|ECS::GROUND|ECS::WALL));

	}

private:

};