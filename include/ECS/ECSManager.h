#pragma once

#include<ECS/System.h>
#include<ECS/ComponentStorage.h>
#include<core/InuputManager.h>

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
		void Add_Comp(Entity id, RenderData render) { renderdata.Add_Comp(id, render); }
		void Add_Comp(Entity id, AnimationData anim) { animationdata.Add_Comp(id, anim); }

		//get componentstorge
		CompStorage<Translate>& Get_Comp_Translate() { return translate; }
		CompStorage<RenderData>& Get_Comp_RenderData() { return renderdata; }
		CompStorage<AnimationData>& Get_Comp_AnimationData() { return animationdata; }

		//get component by entity id
		RenderData& Get_Comp_RenderData(Entity id) { return renderdata.Get_Comp(id); }
		Translate& Get_Comp_Translate(Entity id) { return translate.Get_Comp(id); }
		AnimationData& Get_Comp_AnimationData(Entity id) { return animationdata.Get_Comp(id); }

		//entity remove component
		void Remove_Comp_Translate(Entity id) { translate.Remove_Comp(id); }
		void Remove_Comp_RenderData(Entity id) { renderdata.Remove_Comp(id); }
		void Remove_Comp_AnimationData(Entity id) { animationdata.Remove_Comp(id); }

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
				func(i.first,i.second,input_mgr);
				
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

		InputManager& input_mgr = InputManager::Get_Instance();

		CompStorage<Translate> translate;
		CompStorage<RenderData> renderdata;
		CompStorage<AnimationData> animationdata;

	};
}
