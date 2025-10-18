#pragma once
#include<ECS/Entity.h>
#include<memory>
#include<vector>
#include<iostream>
#include<set>

namespace ECS {
	class ECSManager;

	class System
	{
	public:
		System() = default;
		~System() = default;
		
		virtual void Update(float dt) = 0;

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
			auto sys = std::make_unique<T>(std::forward<Args>(arg)...);
			T* ptr = sys.get();
			systems.push_back(std::move(sys));
			return ptr;
		}

		//system's update function
		void Update(float delta_time)
		{
			for (auto& system : systems)
				system->Update(delta_time);
		}


		//get my system's container


	private:
		std::vector<std::unique_ptr<System> > systems;
	};

}
