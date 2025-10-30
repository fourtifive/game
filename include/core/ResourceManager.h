#pragma once
#include<glew.h>
#include<glfw3.h>
#include<stb_image.h>
#include<memory>
#include<unordered_map>
#include<string>
#include<iostream>

// OpenGL纹理删除器
struct GLTextureDeleter {
	void operator()(GLuint* textureId) const {
		if (textureId && *textureId != 0) {
			glDeleteTextures(1, textureId);
			delete textureId;
		}
	}
};

// 使用GLuint指针的unique_ptr
using GLTexturePtr = std::unique_ptr<GLuint, GLTextureDeleter>;

class ResourceManager
{
public:
	ResourceManager()=default;
	~ResourceManager()=default;

	static ResourceManager& Get_Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	bool Init();

	void Clear();

	bool Load_Texture(const std::string& textureid, const std::string& filepath);
	GLuint* Get_Texture(const std::string& tetureid);
	GLuint* Get_Sprite(const std::string& spriteid, int x, int y, int width, int height);
	
private:
	std::unordered_map<std::string, GLTexturePtr> textures; // store textures
	
};
