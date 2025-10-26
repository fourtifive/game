#include <core/ResourceManager.h>

bool ResourceManager::Init()
{


	return true;
}

void ResourceManager::Clear()
{
}

void ResourceManager::Load_Texture(const std::string& textureid, const std::string& filePath)
{
	if (textures.find(textureid) == textures.end()) {
		return;
	}
	int width, height, nrChannels;
	unsigned char* texture=stbi_load;


}

GLTexturePtr& ResourceManager::Get_Texture(const std::string& name)
{
	// TODO: 在此处插入 return 语句
}
