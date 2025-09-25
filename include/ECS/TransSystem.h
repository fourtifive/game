#pragma once
#include<ECS/System.h>

namespace ECS {
	class TransSystem:public System
	{
	public:
		TransSystem()=default;
		~TransSystem()=default;

		void Update(ECSManager& mgr, float delta_time) {

		}
	private:

	};


}