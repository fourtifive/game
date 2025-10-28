#pragma once
#include<glew.h>
#include "Sprite.h"
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

struct GLSpriteDeleter {
	void operator()(Sprite* sprite) const {
		if (sprite) {
			GLuint texture = sprite->Get_Texture();
			if (texture != 0) {
				glDeleteTextures(1, &texture);  // 删除OpenGL纹理
			}
			delete sprite;  // 删除Sprite对象
		}
	}
};

// 使用GLuint指针的unique_ptr
using GLTexturePtr = std::unique_ptr<GLuint, GLTextureDeleter>;
using SpritePtr = std::unique_ptr<Sprite, GLSpriteDeleter>;

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
	GLTexturePtr& Get_Texture(const std::string& tetureid);

	bool Load_Sprite(const std::string& spriteid, const std::string& filepath, int width, int height);
	Sprite* Get_Sprite(const std::string& spriteid);

private:
	std::unordered_map<std::string, GLTexturePtr> textures; // store textures
	std::unordered_map<std::string, SpritePtr> sprites; // store sprites

};
