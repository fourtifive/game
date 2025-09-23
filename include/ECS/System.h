#pragma once
#include<ECS/Entity.h>
#include<memory>
#include<vector>
#include<set>

class ECSManager;

namespace ECS {
	class System
	{
	public:
		System() = default;
		~System() = default;
		
		virtual void Update(float dt,ECSManager& ecs_mgr) = 0;

		//可添加在系统内部Add实体的方法以及remove实体的方法

	private:

		//添加系统关心的实体的容器

	};

	class SystemManager
	{
	public:

		//register system
		template<typename T,typename...Args>
		T* Register_System(Args&&...arg)
		{
			auto system = std::unique_ptr<T>(std::forward<Args>arg...);
			T* ptr = system.get();
			systems.push_back(std::move(system));
			return ptr;
		}

		//system's update function
		void Update(ECSManager& mgr, float delta_time)
		{
			for (auto& system : systems)
				system->Update(delta_time,mgr);
		}


		//get my system's container


	private:
		std::vector<std::unique_ptr<System> > systems;
	};

}
