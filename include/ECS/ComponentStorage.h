#pragma once
#include<ECS/Entity.h>
#include<unordered_map>
#include<ECS/Component.h>

namespace ECS {
	template<typename Component>
	class CompStorage
	{
	public:
		void Add_Comp(Entity id, Component& comp) {
			component[id] = comp;
		}

		void Remove_Comp(Entity id)
		{
			component.erase(id);
		}

		bool Has_comp(Entity id, Component& comp)const {

		}

		std::unordered_map<Entity, Component>& Get_Comp() {
			return component;
		}

	private:
		std::unordered_map<Entity,Component>component;
	};
}