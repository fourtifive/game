#pragma once
#include "ECS/System.h"
#include"ECSManager.h"
#include"ComponentStorage.h"


namespace ECS
{
	class TerrainSystem : public System
	{
	public:
		TerrainSystem() = default;
		~TerrainSystem() = default;

		void GenerateTerrain(ECSManager& ecs_mgr, float width, float height) {
			
			Entity map = ecs_mgr.Create_Entity();
			MapData mapdata;
			mapdata.map_size = { width,height };
			mapdata.start = {0.0f,0.0f};
			mapdata.end = { width,0.0f };
			ecs_mgr.Add_Comp(map, mapdata);

			CreateBackgroundAndDecration(ecs_mgr, width, height, map);

			CreateGround(ecs_mgr, width, height,map);

			CreatePlatform(ecs_mgr, width, height, map);

			//CreateWall(ecs_mgr, width, height, map);
		}

		void CreateGround(ECSManager& ecs_mgr, float width, float height,Entity map) {
			auto& Map = ecs_mgr.Get_Component<MapData>(map);
			const float tile_w = 16.0f;
			const float tile_h = 48.0f;
			//create collider ground
			Entity ground = ecs_mgr.Create_Entity();
			Translate groundtrans;
			groundtrans.position = { 0.0f,0.0f };

			ColliderBox groundbox;
			groundbox.is_trigger = false;
			groundbox.owner = ground;
			groundbox.is_active = true;
			groundbox.is_static = true;
			groundbox.offsetX = -8.0f;
			groundbox.height = tile_h+groundbox.offsetX;
			groundbox.width = width+groundbox.offsetY;
			groundbox.mask = ECS::CollisionLayer::PLAYER | ECS::CollisionLayer::ENEMY;
			groundbox.layer = ECS::CollisionLayer::GROUND;

			ecs_mgr.Add_Comp(ground, groundtrans);
			ecs_mgr.Add_Comp(ground, groundbox);

			Map.TerrainChunks.push_back(ground);

			//create visual ground
			int tilecount = static_cast<int>(width / tile_w);
			std::cout << "Creating tile entity: ";
			for (int i = 0; i <= tilecount; i++) {
				Entity tile = ecs_mgr.Create_Entity();
				//std::cout<< tile <<" ";
				Translate tiletrans;
				tiletrans.position = { i*tile_w , 0.0f };
				tiletrans.scale = { 1.0f,1.0f };

				RenderData tiletexture;
				if(i%2==0)tiletexture.Id="tile_02";
				else tiletexture.Id = "tile_01";
				tiletexture.srcWidth = tile_w;
				tiletexture.srcHeight = tile_h;
				tiletexture.type = RenderType::SingleImage;

				ecs_mgr.Add_Comp(tile, tiletrans);
				ecs_mgr.Add_Comp(tile, tiletexture);
			}
		}

		void CreatePlatform(ECSManager& ecs_mgr, float width, float height, Entity map) {
			auto& Map = ecs_mgr.Get_Component<MapData>(map);
			//create collider platform



		}

		void CreateWall(ECSManager& ecs_mgr, float width, float height, Entity map) {

		}

		void CreateBackgroundAndDecration(ECSManager& ecs_mgr, float width, float height, Entity map) {
			Entity background = ecs_mgr.Create_Entity();
			Entity middleground = ecs_mgr.Create_Entity();
			//std::cout << "Background Entity: " << background << std::endl;
			//std::cout << "Middleground Entity: " << middleground << std::endl;

			Translate bgtrans,mgtrans;
			bgtrans.position = { 0.0f,0.0f };
			mgtrans.position = { 0.0f,0.0f };
			bgtrans.scale = { width / 384.0f,height / 288.0f };
			mgtrans.scale = { width / 384.0f,height / 288.0f };

			ecs_mgr.Add_Comp(background, bgtrans);
			ecs_mgr.Add_Comp(middleground, mgtrans);
			
			RenderData bgtexture,mgtexture;
			bgtexture.Id = "background";
			bgtexture.type = RenderType::SingleImage;
			bgtexture.srcWidth = 384;
			bgtexture.srcHeight = 288;

			mgtexture.Id = "middleground";
			mgtexture.type = RenderType::SingleImage;
			mgtexture.srcWidth = 384;
			mgtexture.srcHeight = 288;

			ecs_mgr.Add_Comp(background, bgtexture);
			ecs_mgr.Add_Comp(middleground, mgtexture);
		}

	};
}

