#pragma once

#include<ECS/System.h>
#include<ECS/ComponentStorage.h>

namespace ECS {
	class ECSManager
	{
	public:
		ECSManager()=default;
		~ECSManager()=default;

		//create entity
		Entity Create_entity()
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
		void Add_Comp(Entity id, Translate& trans) { translate.Add_Comp(id, trans); }

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
			for (auto& [id, comp] : translate) {
				func(id, comp, dt);
			}
		}

		//update my system
		void Update(float dt)
		{
			system_mgr.Update(*this,dt);
		}


	private:
		EntityManager entity_mgr;
		SystemManager system_mgr;
		CompStorage<Translate> translate;

		SystemManager system_mgr;
	};
}
