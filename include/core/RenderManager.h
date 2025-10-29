#pragma once
#include<ECS/ECSManager.h>
#include<core/ResourceManager.h>

class RenderManager {
public:
    RenderManager() = default;
    ~RenderManager() = default;

    static RenderManager& Get_Instance() {
        static RenderManager instance;
        return instance;
	}

    void Init(ResourceManager& resource_mgr) {
        this->resource_mgr = &resource_mgr;
		this->ecs_mgr = &ECS::ECSManager::Get_Instance();
		this->AddSpriteToBatch(nullptr, 0, 0, 0, 0); // 占位调用，防止未使用警告
    }

    void Render() {
        auto& renderables = ecs_mgr->Get_Comp_Renderable();

        // 开始批量渲染
        BeginBatch();

        for (const auto& i : renderables.Get_Comp()) {
            GLuint* texture = nullptr; 
            Sprite* sprite = nullptr; 

            if (i.second.type == ECS::RenderType::SingleImage) {
                texture = resource_mgr->Get_Texture(i.second.Id);
            } 
            else {
                sprite = resource_mgr->Get_Sprite(i.second.Id);
            }

            if (texture != nullptr) {
                AddTextureToBatch(texture, i.second.srcX, i.second.srcY, i.second.srcWidth, i.second.srcHeight);
            }
            else {
				AddSpriteToBatch(sprite, i.second.srcX, i.second.srcY, i.second.srcWidth, i.second.srcHeight);
            }
        }

        // 提交批量渲染
        EndBatch();
    }

private:
    ResourceManager* resource_mgr = nullptr;
	ECS::ECSManager* ecs_mgr = nullptr;

    void BeginBatch();

    void AddTextureToBatch(GLuint* texture, int srcX, int srcY, int srcWidth, int srcHeight) {
        // 将渲染数据添加到批量渲染的缓冲区
        // 计算纹理坐标（UV）
        // 添加顶点数据到缓冲区（假设已经设置了顶点格式）
	}

    void AddSpriteToBatch(Sprite* sprite, int srcX, int srcY, int srcWidth, int srcHeight) {
        // 将渲染数据添加到批量渲染的缓冲区
        // 计算纹理坐标（UV）
        float u1 = static_cast<float>(srcX) / sprite->Get_Width();
        float v1 = static_cast<float>(srcY) / sprite->Get_Height();
        float u2 = static_cast<float>(srcX + srcWidth) / sprite->Get_Width();
        float v2 = static_cast<float>(srcY + srcHeight) / sprite->Get_Height();

        // 添加顶点数据到缓冲区（假设已经设置了顶点格式）
    }

    void EndBatch();
};
