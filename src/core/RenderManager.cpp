#include "core/RenderManager.h"
#include<ECS/Component.h>

void RenderManager::Init(int w,int h)
{
	std::cout << "Initializing RenderManager..." << std::endl;

	screenHeight = float(h);
    screenWidth = float(w);

	this->resource_mgr =&ResourceManager::Get_Instance();
	this->ecs_mgr = &ECS::ECSManager::Get_Instance();

	//declare buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

	// initialize vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_BATCH_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	// initialize index buffer
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

	// set vertex attributes
    glEnableVertexAttribArray(0); // 位置
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1); // 纹理坐标
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2); // 纹理索引
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex));

	// compile shaders
    CompileShaders();

	//initialize projection matrix
	projectionMatrix = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);

    //DebugProjectionMatrix();

    glBindVertexArray(0);


}

void RenderManager::Render(GLFWwindow* window)
{
	BeginBatch();

	//batch all renderable entities
    auto& renderble = ecs_mgr->Get_Comp_Storage<ECS::RenderData>();

    for (auto& data : renderble.Get_Comp()) {
        GLuint* texture = resource_mgr->Get_Texture(data.second.Id);
        if (data.second.type == ECS::RenderType::SingleImage)
            AddTextureToBatch(texture, data.first, data.second.srcWidth, data.second.srcHeight);
        else if (data.second.type == ECS::RenderType::Sprite) {
            auto& anim = ecs_mgr->Get_Component<ECS::AnimationData>(data.first);
            AddSpriteToBatch(texture, data.first, anim.animations[anim.animID].srcX, anim.animations[anim.animID].srcY, anim.animations[anim.animID].srcWidth, anim.animations[anim.animID].srcHeight, anim.animations[anim.animID].spriteWidth, anim.animations[anim.animID].spriteHeight);
        }
        //std::cout << "Added entity " << data.second.Id << " to batch." << std::endl;
    }

	EndBatch();

    glfwSwapBuffers(window);


}

void RenderManager::Shutdown()
{

}

void RenderManager::CompileShaders()
{
	const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 aPosition;
        layout(location = 1) in vec2 aTexCoord;
        layout(location = 2) in float aTexIndex;
        
        uniform mat4 uProjection;

        out vec2 TexCoord;
        out float TexIndex;
        
        void main()
        {
            gl_Position = uProjection * vec4(aPosition, 0.0f, 1.0f);
            TexCoord = aTexCoord;
            TexIndex = aTexIndex;
        }
    )";

	const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoord;
        in float TexIndex;
        uniform sampler2D uTextures[16];
        void main()
        {
            int index = int(TexIndex);
            FragColor = texture(uTextures[index],TexCoord);

            if(index<0||index>16)
                FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Magenta for error

        }
    )";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	// check for shader compile errors
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	// check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// link shaders
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertexShader);
	glAttachShader(shader_program, fragmentShader);
	glLinkProgram(shader_program);


    // check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void RenderManager::BeginBatch()
{
	texture_count = 0;
	vertex_count = 0;
	batchVertices.clear();
	boundtextures.clear();

    glViewport(0, 0, screenWidth, screenHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    
    auto& trans = ecs_mgr->Get_Component<ECS::Translate>(id);
    
	
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

    auto& trans = ecs_mgr->Get_Component<ECS::Translate>(id);

    // create 4 vertices for the quad
    Point position[4];

    if (trans.isfacingright) {
        position[0] = { trans.position.x,trans.position.y };
        position[1]= { trans.position.x + spriteWidth, trans.position.y };
        position[2]={ trans.position.x + spriteWidth, trans.position.y + spriteHeight };
        position[3]={ trans.position.x, trans.position.y + spriteHeight };

    }
    else {
        position[0]={ trans.position.x + spriteWidth, trans.position.y };
        position[1]={ trans.position.x, trans.position.y };
        position[2]={ trans.position.x, trans.position.y + spriteHeight };
        position[3]={trans.position.x + spriteWidth, trans.position.y + spriteHeight};
    }

    
    float u1 = (float)srcX / srcWidth;          
    float v1 = (float)srcY / srcHeight;          
    float u2 = (float)(srcX + spriteWidth) / srcWidth;   
    float v2 = (float)(srcY + spriteHeight) / srcHeight; 
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
    FlushBatch();
}

void RenderManager::FlushBatch()
{
    if (batchVertices.empty()) {
        std::cout << "No vertices to flush!" << std::endl;
        return;
    }
	// upload vertex data to GPU

	glUseProgram(shader_program);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, batchVertices.size() * sizeof(Vertex), batchVertices.data());
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << err << std::endl;
    }

	// set projection matrix uniform
	
	GLint projLocation = glGetUniformLocation(shader_program, "uProjection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // bind textures
    for (int i = 0; i < boundtextures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, *boundtextures[i]);

		std::string uniformName = "uTextures[" + std::to_string(i) + "]";
        GLint location = glGetUniformLocation(shader_program, uniformName.c_str());
		glUniform1i(location, i);

	}
    // draw call
	glDrawElements(GL_TRIANGLES, (vertex_count / 4) * 6, GL_UNSIGNED_INT, 0);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << err << std::endl;
    }

	//DebugBatchVertices();
	//DebugBoundTextures();

	glBindVertexArray(0);
}

void RenderManager::DebugBatchVertices()
{
    std::cout << "=== Batch Vertices Debug ===" << std::endl;
    for (size_t i = 0; i < batchVertices.size(); i++) {
        const Vertex& v = batchVertices[i];
        std::cout << "Vertex " << i << ": Pos(" << v.position.x << ", " << v.position.y << "), TexCoord(" << v.texcoord.x << ", " << v.texcoord.y << "), TexIndex(" << v.texIndex << ")" << std::endl;
	}
}

void RenderManager::DebugProjectionMatrix()
{
    std::cout << "=== Projection Matrix Debug ===" << std::endl;

	//output the projection matrix
        const float* m = glm::value_ptr(projectionMatrix);
        std::cout << "current projection:" << std::endl;
        printf("[ %8.3f %8.3f %8.3f %8.3f ]\n", m[0], m[4], m[8], m[12]);
        printf("[ %8.3f %8.3f %8.3f %8.3f ]\n", m[1], m[5], m[9], m[13]);
        printf("[ %8.3f %8.3f %8.3f %8.3f ]\n", m[2], m[6], m[10], m[14]);
        printf("[ %8.3f %8.3f %8.3f %8.3f ]\n", m[3], m[7], m[11], m[15]);

		// test some points
        glm::vec4 testPoints[] = {
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),     //left down
			glm::vec4(1280.0f, 800.0f, 0.0f, 1.0f),      //right up
			glm::vec4(640.0f, 400.0f, 0.0f, 1.0f),      //center
			glm::vec4(1152.0f, 128.0f, 0.0f, 1.0f)  // your vertex
        };

        const char* names[] = { "left down", "right up", "center", "your vertex" };

        for (int i = 0; i < 4; i++) {
            glm::vec4 result = projectionMatrix * testPoints[i];
            std::cout << names[i] << "transfrom:" << std::endl;
            std::cout << "  input: (" << testPoints[i].x << ", " << testPoints[i].y << ")" << std::endl;
            std::cout << "  out: (" << result.x << ", " << result.y << ", " << result.z << ")" << std::endl;
            std::cout << "  in screen: " << (IsInNDC(result) ? "yes" : "no") << std::endl;
        }
}

bool RenderManager::IsInNDC(const glm::vec4& point) 
{
    return point.x >= -1.0f && point.x <= 1.0f &&
        point.y >= -1.0f && point.y <= 1.0f &&
        point.z >= -1.0f && point.z <= 1.0f;
}

void RenderManager::DebugBoundTextures()
{
    std::cout << "=== Bound Textures Debug ===" << std::endl;
    for (size_t i = 0; i < boundtextures.size(); i++) {
        if(boundtextures[i]!=nullptr)
            std::cout << "Texture Slot " << i << ": ID(" << *boundtextures[i] << ")" << std::endl;
        else
			std::cout << "Texture Slot " << i << ": ID(NULL)" << std::endl;
	}
}



GLuint RenderManager::CompileShader(const char* vertexSource, const char* fragmentSource)
{
    GLuint TestProgram = 0;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // link shaders
    TestProgram = glCreateProgram();
    glAttachShader(TestProgram, vertexShader);
    glAttachShader(TestProgram, fragmentShader);
    glLinkProgram(TestProgram);


    // check for linking errors
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return TestProgram;
}

void RenderManager::RenderTestTriangle() {
    std::cout << "=== 渲染测试三角形 ===" << std::endl;

    // 使用简单的着色器
    const char* TestVS = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
        }
    )";

    const char* TestFS = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // 纯红色
        }
    )";

    // 编译简单着色器
    GLuint testProgram = CompileShader(TestVS, TestFS);
    glUseProgram(testProgram);

    // 简单的三角形数据
    float vertices[] = {
        -0.5f, -0.5f,  // 左下
         0.5f, -0.5f,  // 右下
         0.0f,  0.5f   // 上
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, 3);
    std::cout << "测试三角形绘制完成" << std::endl;

    // 检查错误
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "测试三角形绘制错误: " << err << std::endl;
    }

    // 清理
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(testProgram);
}