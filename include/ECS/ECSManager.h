#pragma once

#include<ECS/System.h>
#include<ECS/ComponentStorage.h>
#include<core/InuputManager.h>
#include<typeindex>

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

		// 添加组件
		template<typename T>
		void Add_Comp(Entity id, T component) {
			Get_Comp_Storage<T>().Add_Comp(id, component);
			entity_mgr.Add_Comp(id, Get_Component_Type<T>());
		}

		// 移除组件
		template<typename T>
		void Remove_Comp(Entity id) {
			Get_Comp_Storage<T>().Remove_Comp(id);
			entity_mgr.Remove_Comp(id, Get_Component_Type<T>());
		}

		// get componentstorge
		template<typename T>
		CompStorage<T>& Get_Comp_Storage();

		//get component by entity id
		template<typename T>
		T& Get_Component(Entity id) {
			return Get_Comp_Storage<T>().Get_Comp(id);
		}

		// get component type id
		template<typename T>
		unsigned int Get_Component_Type() {
			auto it = component_type_map.find(typeid(T));
			if (it == component_type_map.end()) {
				component_type_map[typeid(T)] = component_type++;
			}
			return component_type_map[typeid(T)];
		}

		//register system
		template<typename T,typename...Args>
		T* Register_System(Args&&...args)
		{
			return system_mgr.Register_System<T>(std::forward<Args>(args)...);
		}

		//update my system
		void Update(float dt)
		{
			system_mgr.Update(*this,dt);
		}

	private:

		template<typename... ComponentTypes>
		std::bitset<MAX_COMPONENTS> CalculateComponentMask() {
			std::bitset<MAX_COMPONENTS> mask;

			((mask.set(Get_Component_Type<ComponentTypes>())), ...);

			return mask;
		}
		
        // Modify the Traverse method to make it accessible by marking it as public.  
        public:  
        template<typename... ComponentTypes, typename Func>  
        void Traverse(Func&& func) {  
            std::bitset<MAX_COMPONENTS> target_mask = CalculateComponentMask<ComponentTypes...>();  

            const auto& entities = entity_mgr.Get_Entities();  

            for (Entity id : entities) {  
                if (!entity_mgr.Is_Alive(id)) continue;  

                const auto& entity_mask = entity_mgr.Get_Component_Mask(id);  

                if ((entity_mask & target_mask) == target_mask) {  
                    if constexpr (sizeof...(ComponentTypes) == 0) {  
                        func(id,input_mgr);  
                    }  
                    else {  
                        func(id,input_mgr, Get_Component<ComponentTypes>(id)...); 
                    }  
                }  
            }  
        }

		template<typename... ComponentTypes, typename Func>
		void TraverseNoInput(Func&& func) {
			std::bitset<MAX_COMPONENTS> target_mask = CalculateComponentMask<ComponentTypes...>();
			const auto& all_entities = entity_mgr.Get_Entities();

			for (Entity id : all_entities) {
				if (!entity_mgr.Is_Alive(id)) continue;

				const auto& entity_mask = entity_mgr.Get_Component_Mask(id);
				if ((entity_mask & target_mask) == target_mask) {
					if constexpr (sizeof...(ComponentTypes) == 0) {
						func(id);
					}
					else {
						func(id, Get_Component<ComponentTypes>(id)...);
					}
				}
			}
		}

	private:
		EntityManager entity_mgr;
		SystemManager system_mgr;

		InputManager& input_mgr = InputManager::Get_Instance();

		CompStorage<Translate> translate;
		CompStorage<RenderData> renderdata;
		CompStorage<AnimationData> animationdata;
		CompStorage<Physical> physical;
		CompStorage<State> state;
		CompStorage<PlayerControlled> playercontrolled;
		CompStorage<ColliderBox> colliderbox;
		CompStorage<MapData> mapdata;

		std::unordered_map<std::type_index,unsigned int> component_type_map;
		unsigned int component_type = 0;

	};

	// 特化组件存储
	template<>
	inline CompStorage<MapData>& ECSManager::Get_Comp_Storage<MapData>() {
		return mapdata;
	}

	template<>
	inline CompStorage<Translate>& ECSManager::Get_Comp_Storage<Translate>() {
		return translate;
	}

	template<>
	inline CompStorage<RenderData>& ECSManager::Get_Comp_Storage<RenderData>() {
		return renderdata;
	}

	template<>
	inline CompStorage<AnimationData>& ECSManager::Get_Comp_Storage<AnimationData>() {
		return animationdata;
	}

	template<>
	inline CompStorage<Physical>& ECSManager::Get_Comp_Storage<Physical>() {
		return physical;
	}

	template<>
	inline CompStorage<State>& ECSManager::Get_Comp_Storage<State>() {
		return state;
	}

	template<>
	inline CompStorage<PlayerControlled>& ECSManager::Get_Comp_Storage<PlayerControlled>() {
		return playercontrolled;
	}

	template<>
	inline CompStorage<ColliderBox>& ECSManager::Get_Comp_Storage<ColliderBox>() {
		return colliderbox;
	}
}
