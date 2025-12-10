#pragma once
#include<glew.h>
#include<glfw3.h>
#include<ECS/ECSManager.h>
#include<core/ResourceManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using Entity = unsigned int;

constexpr int MAX_BATCH_VERTICES = 1000; // 最大顶点数
constexpr int MAX_BATCH_INDICES = MAX_BATCH_VERTICES * 6 / 4; // 每4个顶点对应6个索引
constexpr int MAX_TEXTURE_SLOTS = 16; // OpenGL 最多支持的纹理单元数

struct Vertex {
    Point position; // 顶点位置 (x, y)
    Point texcoord; // 纹理坐标 (u, v)
    float texIndex;    // 纹理索引
};

class RenderManager {
public:
    RenderManager() = default;
    ~RenderManager() = default;

    static RenderManager& Get_Instance() {
        static RenderManager instance;
        return instance;
	}

    void Init(int screenW,int screenH);

    void Render(GLFWwindow* window);

	void Shutdown();

	void RenderTestTriangle();

private:
    ResourceManager* resource_mgr = nullptr;
	ECS::ECSManager* ecs_mgr = nullptr;

	GLuint shader_program = 0;
	GLuint vao = 0, vbo = 0, ebo = 0;

	// projection matrix for 2D rendering
	glm::mat4 projectionMatrix;

	std::vector<Vertex> batchVertices;//vertices in the current batch
	std::vector<GLuint*> boundtextures;//currently bound textures in the batch
	int texture_count = 0;//number of texture in the current batch
	int vertex_count = 0;//number of vertices in the current batch
	
	float screenWidth=0.0f;
	float screenHeight = 0.0f;

	
private:

	void CompileShaders();

	GLuint CompileShader(const char* vertexSource, const char* fragmentSource);

    void BeginBatch();

    void AddTextureToBatch(GLuint* texture,Entity id, int srcWidth, int srcHeight);

    void AddSpriteToBatch(GLuint* spritetex,Entity id, int srcX, int srcY, int srcWidth, int srcHeight,int spriteWidth,int spriteHeight);

    void EndBatch();

	void FlushBatch();
private:
	void DebugBatchVertices();

	void DebugProjectionMatrix();

	bool IsInNDC(const glm::vec4& point);

	void DebugBoundTextures();
};
