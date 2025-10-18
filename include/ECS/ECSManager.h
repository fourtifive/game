#pragma once

#include<ECS/System.h>
#include<ECS/ComponentStorage.h>

namespace ECS {
	class ECSManager
	{
	public:
		ECSManager()=default;
		~ECSManager()=default;

		static ECSManager& Get_Instance() {
			static ECSManager ECS_mgr;
			return ECS_mgr;
		}

		//create entity
		Entity Create_Entity()
		{
			return entity_mgr.Create_Entity();

		}

		Entity Create_Entity(Entity id)
		{
			return entity_mgr.Create_Entity(id);
		}

		//destory entity
		void Destory_Entity(Entity id)
		{
			entity_mgr.Destroy_Entity(id);
		}

		//entity add component
		void Add_Comp(Entity id, Translate trans) { translate.Add_Comp(id, trans); }

		//entity remove component
		void Remove_Comp(Entity id) { translate.Remove_Comp(id); }

		//register system
		template<typename T,typename...Args>
		T* Register_System(Args&&...args)
		{
			return system_mgr.Register_System<T>(std::forward<Args>(args)...);
		}

		//traverse component
		template<typename Func>
		void Traverse_Eachtrans(Func&& func)
		{
			for (auto& i : translate.Get_Comp()) {
				func(i.first,i.second);
				//std::cout << i.second.trans.x << " " << i.second.trans.y<<std::endl;
			}
		}

		//update my system
		void Update(float dt)
		{
			system_mgr.Update(dt);
		}


	private:
		EntityManager entity_mgr;
		SystemManager system_mgr;

		CompStorage<Translate> translate;

		/*ECSManager* ECS_mgr=nullptr;*/
	};
}
