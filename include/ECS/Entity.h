#pragma once
const unsigned int PLAYER = 1;
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

			return id;
		}

		Entity Create_Entity(Entity id) {
			if (id_count == id)id_count++;

			entities.push_back(id);

			return id;
		}

		void Destroy_Entity(Entity id)
		{
			entities.erase(std::remove(entities.begin(), entities.end(), id), entities.end());
			free_id.push_back(id);
		}

		bool Is_Alive(Entity id)
		{

		}

	private:
		Entity id_count=1;
		std::vector<Entity> entities;
		std::vector<Entity> free_id;
	};
}
