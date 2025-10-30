#include "core/RenderManager.h"

void RenderManager::Init(ResourceManager& resource_mgr)
{
	std::cout << "Initializing RenderManager..." << std::endl;

	this->resource_mgr = &resource_mgr;
	this->ecs_mgr = &ECS::ECSManager::Get_Instance();

    // 初始化 OpenGL 批渲染资源
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    // 初始化顶点缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_BATCH_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // 初始化索引缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    std::vector<unsigned int> indices(MAX_BATCH_INDICES);
    unsigned int offset = 0;
    for (int i = 0; i < MAX_BATCH_INDICES; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 设置顶点属性
    glEnableVertexAttribArray(0); // 位置
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1); // 纹理坐标
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2); // 纹理索引
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex));

    glBindVertexArray(0);


}

void RenderManager::Render()
{
	BeginBatch();

	//batch all renderable entities
    auto& renderble = ecs_mgr->Get_Comp_RenderData();

    for (auto& data : renderble.Get_Comp()) {
        GLuint* texture = resource_mgr->Get_Texture(data.second.Id);
        if (data.second.type == ECS::RenderType::SingleImage)
            AddTextureToBatch(texture, data.first, data.second.srcWidth, data.second.srcHeight);
        else if (data.second.type == ECS::RenderType::Sprite) {
            auto& anim = ecs_mgr->Get_Comp_AnimationData(data.first);
            AddSpriteToBatch(texture, data.first, anim.srcX, anim.srcY, data.second.srcWidth, data.second.srcHeight, anim.spriteWidth, anim.spriteHeight);
        }
        //std::cout << "Added entity " << data.second.Id << " to batch." << std::endl;
    }

	EndBatch();

}

void RenderManager::Shutdown()
{

}

void RenderManager::BeginBatch()
{
	texture_count = 0;
	vertex_count = 0;
	batchVertices.clear();
	boundtextures.clear();

}

void RenderManager::AddTextureToBatch(GLuint* texture,Entity id, int srcWidth, int srcHeight)
{
    if (!texture) {
        std::cout << "Texture is null!" << std::endl;
        return;
    }

	// check if texture is already bound
    int textureIndex = -1;
    for (int i = 0; i < boundtextures.size(); i++) {
        if (boundtextures[i] == texture) {
            textureIndex = i;
            break;
        }
    }
	// if not bound, bind it
    if (textureIndex == -1) {
        if (texture_count >= MAX_TEXTURE_SLOTS) {
			// reached max texture slots, flush the batch
            FlushBatch();
            texture_count = 0;
            vertex_count = 0;
            batchVertices.clear();
            boundtextures.clear();
        }
        boundtextures.push_back(texture);
		textureIndex = texture_count;// assign new texture index
        texture_count++;
    }
    
    auto& trans = ecs_mgr->Get_Comp_Translate(id);
    
	// create 4 vertices for the quad
    Point position[4] = {
        {trans.position.x,trans.position.y},
        {trans.position.x + srcWidth,trans.position.y },
        {trans.position.x + srcWidth,trans.position.y + srcHeight },
        {trans.position.x,trans.position.y + srcHeight }
	};

    Point texcoord[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    for (int i = 0; i < 4; i++) {
        vertex_count++;
        Vertex vertex = {};
		vertex.position = position[i]; // set position from transform component
        vertex.texcoord = texcoord[i]; // set default tex coords
		vertex.texIndex = (float)textureIndex; // set texture index
		batchVertices.push_back(vertex);
    }

}

void RenderManager::AddSpriteToBatch(GLuint* sprite,Entity id, int srcX, int srcY, int srcWidth, int srcHeight, int spriteWidth, int spriteHeight)
{
    if (!sprite) {
        std::cout << "Texture is null!" << std::endl;
        return;
    }

    // check if texture is already bound
    int textureIndex = -1;
    for (int i = 0; i < boundtextures.size(); i++) {
        if (boundtextures[i] == sprite) {
            textureIndex = i;
            break;
        }
    }
    // if not bound, bind it
    if (textureIndex == -1) {
        if (texture_count >= MAX_TEXTURE_SLOTS) {
            // reached max texture slots, flush the batch
            FlushBatch();
            texture_count = 0;
            vertex_count = 0;
            batchVertices.clear();
            boundtextures.clear();
        }
        boundtextures.push_back(sprite);
        textureIndex = texture_count;// assign new texture index
        texture_count++;
    }

    auto& trans = ecs_mgr->Get_Comp_Translate(id);

    // create 4 vertices for the quad
    Point position[4] = {
        {trans.position.x,trans.position.y},
        {trans.position.x + srcWidth,trans.position.y },
        {trans.position.x + srcWidth,trans.position.y + srcHeight },
        {trans.position.x,trans.position.y + srcHeight }
    };

    
    float u1 = (float)srcX / srcWidth;          // 左
    float v1 = (float)srcY / srcHeight;         // 下  
    float u2 = (float)(srcX + spriteWidth) / srcWidth;   // 右
    float v2 = (float)(srcY + spriteHeight) / srcHeight; // 上
    Point texcoord[4] = {
        {u1, v1}, 
        {u2, v1},  
        {u2, v2},  
        {u1, v2}   
    };

    for (int i = 0; i < 4; i++) {
        vertex_count++;
        Vertex vertex = {};
        vertex.position = position[i]; // set position from transform component
        vertex.texcoord = texcoord[i]; // set default tex coords
        vertex.texIndex = (float)textureIndex; // set texture index
        batchVertices.push_back(vertex);
    }

}

void RenderManager::EndBatch()
{
    //FlushBatch();
}

void RenderManager::FlushBatch()
{
    if (batchVertices.empty())return;
	// upload vertex data to GPU

	glUseProgram(shader_program);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, batchVertices.size() * sizeof(Vertex), batchVertices.data());

    // bind textures
    for (int i = 0; i < boundtextures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, *boundtextures[i]);


	}
    // draw call
	glDrawElements(GL_TRIANGLES, (vertex_count / 4) * 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
