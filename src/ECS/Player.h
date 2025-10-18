#pragma
#include<ECS/ECSManager.h>

class Player
{
public:
	Player();
	~Player();



private:

};

inline Player::Player()
{
	ECS::ECSManager::Get_Instance().Create_Entity(PLAYER);
	
	ECS::ECSManager::Get_Instance().Add_Comp(PLAYER, {0,0});
}

inline Player::~Player()
{

}
