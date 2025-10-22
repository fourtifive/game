#pragma
#include<ECS/ECSManager.h>

class Player
{
public:
	
	void Init() {
		auto& ecs = ECS::ECSManager::Get_Instance();

		ecs.Create_Entity(PLAYER);

		ecs.Add_Comp(PLAYER, { 0,0 });

	}

private:

};
