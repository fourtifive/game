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

		//�������ϵͳ�ڲ�Addʵ��ķ����Լ�removeʵ��ķ���

	private:

		//���ϵͳ���ĵ�ʵ�������

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
