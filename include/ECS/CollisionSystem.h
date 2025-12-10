#pragma once

#include "ECS/System.h"
#include"ECSManager.h"
#include"ComponentStorage.h"

namespace ECS
{
	class CollisionSystem :public System
    {
    public:
        CollisionSystem() = default;
        ~CollisionSystem() = default;

        void Update(ECSManager& ecs_mgr, float deltaTime) {
			//std::cout << "  ---Collision System Update---  \n";
            //clear previous collisions

            // Get all entities with Position and Collision components
            auto boxes = CollectColliders(ecs_mgr);

            // Check for collisions between entities
            std::unordered_map<CollisionLayer, std::vector<size_t>> layers;
            for (size_t i = 0; i < boxes.size(); ++i) {
                layers[boxes[i]->layer].push_back(i);
				//std::cout << "Box " << i << " position: (" << boxes[i]->xL << ", " << boxes[i]->yD << "), (" << boxes[i]->xR << ", " << boxes[i]->yU << ")\n";
            }

            for (size_t boxA = 0; boxA < boxes.size(); ++boxA) {
				auto Layer = GetPossibleCollisionLayers(*boxes[boxA]);
                for (auto layer:Layer) {
                    if (layers.find(layer) == layers.end()) continue;
                    for (size_t boxB : layers[layer]) {
                        if (boxB <= boxA) continue; 
						//std::cout <<" Box " << boxA << "  Against Box " << boxB << "\n";
                        if (CheckCollision(*boxes[boxA], *boxes[boxB]))
                        {
                            ResolvePhysicalCollision(*boxes[boxA], *boxes[boxB],ecs_mgr);

                        }
                    }
                }
            }
        }

	private:

        std::vector<ECS::ColliderBox*> CollectColliders(ECSManager& ecs_mgr) {
            std::vector<ECS::ColliderBox*> activeColliders;
            ecs_mgr.TraverseNoInput<Translate, ColliderBox>([&](Entity id, Translate& trans, ColliderBox& box) 
                {
                    if (box.is_active) {
                        box.owner = id;
                        box.xL = trans.position.x;
						box.xR = trans.position.x + box.width;
                        box.yD = trans.position.y;
						box.yU = trans.position.y + box.height;
                        activeColliders.push_back(&box);
                    }
                });

            return activeColliders;
        }

        std::vector<CollisionLayer> GetPossibleCollisionLayers(const ColliderBox& box) {
            std::vector<CollisionLayer> layer;
            for (int l = 1; l <= 64; l <<= 1) {
                if (box.mask & l) {
                    layer.push_back(static_cast<CollisionLayer>(l));
                }
            }
            return layer;
        }

		bool CheckCollision(const ECS::ColliderBox& boxA, const ECS::ColliderBox& boxB) {
			//std::cout << "Checking collision between BoxA(" << boxA.xL << "," << boxA.yD << "," << boxA.xR << "," << boxA.yU << ") and BoxB(" << boxB.xL << "," << boxB.yD << "," << boxB.xR << "," << boxB.yU << ")\n";
            if (boxA.xR - boxB.xL < 5 || boxA.xL - boxB.xR > -5) return 0;
            if (boxA.yD - boxB.yU > -5 || boxA.yU - boxB.yD < 5) return 0;
			return 1;
		}

		void ResolvePhysicalCollision(ECS::ColliderBox& boxA,ECS::ColliderBox& boxB,ECS::ECSManager& ecs_mgr) {
            if (!boxA.is_static) {
                auto& trans = ecs_mgr.Get_Component<Translate>(boxA.owner);
                auto& phy = ecs_mgr.Get_Component<Physical>(boxA.owner);
				trans.position.x -= phy.velocity.x;
				trans.position.y -= phy.velocity.y;

                boxA.xL = trans.position.x;
                boxA.xR = trans.position.x + boxA.width;
                boxA.yD = trans.position.y;
                boxA.yU = trans.position.y + boxA.height;
            }
			    
            if (!boxB.is_static) {
                auto& trans = ecs_mgr.Get_Component<Translate>(boxB.owner);
                auto& phy = ecs_mgr.Get_Component<Physical>(boxB.owner);
                trans.position.x -= phy.velocity.x;
                trans.position.y -= phy.velocity.y;

                boxB.xL = trans.position.x;
                boxB.xR = trans.position.x + boxB.width;
                boxB.yD = trans.position.y;
                boxB.yU = trans.position.y + boxB.height;
            }
				
		}

		void CreateCollisionEvent() {
		
		}

	};

}