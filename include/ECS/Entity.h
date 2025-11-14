#pragma once
const size_t MAX_COMPONENTS = 32;
const unsigned int PLAYER = 0;
#include<bitset>
#include<algorithm>
#include<vector>

namespace ECS {
	using Entity = unsigned int;
	
	class EntityManager
	{
	public:
		EntityManager() = default;
		~EntityManager()= default;
		Entity Create_Entity() {
			Entity id;
			if (!free_id.empty())
			{
				id = free_id.back();
				free_id.pop_back();	
			}
			else
				id = id_count++;
			entities.push_back(id);
			compmasks.resize(id_count);
			return id;
		}

		Entity Create_Entity(Entity id) {
			entities.push_back(id);
			compmasks.resize(id_count);
			return id;
		}

		void Destroy_Entity(Entity id)
		{
			entities.erase(std::remove(entities.begin(), entities.end(), id), entities.end());
			free_id.push_back(id);
			compmasks[id].reset();//reset component mask
		}

		bool Is_Alive(Entity id)
		{
			return 1;
		}

		void Add_Comp(Entity id, unsigned int component_type) {
			compmasks[id].set(component_type);
		}

		void Remove_Comp(Entity id,unsigned int component_type) {
			compmasks[id].reset(component_type);
		}

		const std::bitset<MAX_COMPONENTS>& Get_Component_Mask(Entity id) const {
			return compmasks[id];
		}

		const std::vector<Entity>& Get_Entities() const {
			return entities;
		}

	private:
		Entity id_count=1;
		std::vector<Entity> entities;
		std::vector<Entity> free_id;
		std::vector<std::bitset<MAX_COMPONENTS>> compmasks;
	};
}
