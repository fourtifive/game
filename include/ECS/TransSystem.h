#pragma once
#include<ECS/System.h>
#include<ECS/ComponentStorage.h>
#include<ECS/ECSManager.h>

namespace ECS {
	class TransSystem:public System
	{
	public:
		TransSystem()=default;
		~TransSystem()=default;

		void Update(float delta_time) {
			ECSManager::Get_Instance().Traverse_Eachtrans([delta_time](Entity id, Translate& comp) {
				comp.trans.x += 1 * delta_time;
				comp.trans.y += 1 * delta_time;
				});
		}
	private:

	};


}