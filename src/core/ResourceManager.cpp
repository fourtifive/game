#include <core/ResourceManager.h>

bool ResourceManager::Init()
{
	std::cout << "Initializing ResourceManager..." << std::endl;

	if(Load_Texture("player_walking","assert/Samurai/Walk.png"))std::cout<<"Loaded texture successfully : player_waking" << std::endl;
	else std::cout << "Failed to load texture : player_waking" << std::endl;

	return true;
}

void ResourceManager::Clear()
{
    
    std::cout << "Cleaning up " << textures.size() << " textures" << std::endl;

    glFinish();
	//wait for opengl to finish all operations
    textures.clear();

    std::cout << "Cleanup complete" << std::endl;

	textures.clear();
}

bool ResourceManager::Load_Texture(const std::string& textureid, const std::string& filepath)
{
	if (textures.find(textureid) != textures.end()) {
		std::cout << "Texture ID already exists: " << textureid << std::endl;
		return false;
	}
    int width, height, Channels;
    stbi_set_flip_vertically_on_load(true); // OpenGL的y轴原点在底部

    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &Channels, 0);
    if (!data) {
		std::cout << "Failed to load texture: " << textureid << std::endl;
        return false;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGB;
    if (Channels == 1)
        format = GL_RED;
    else if (Channels == 3)
        format = GL_RGB;
    else if (Channels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    textures[textureid] = GLTexturePtr(new GLuint(texture));

    std::cout << "Loaded texture: " << textureid << " (" << width << "x" << height
        << ", channels: " << Channels << ")" << std::endl;

    return true;
}

GLuint* ResourceManager::Get_Texture(const std::string& textureid)
{
    if (textures.find(textureid) == textures.end()) {
        return nullptr;
    }
    return textures[textureid].get();
}

GLuint* ResourceManager::Get_Sprite(const std::string& spriteid, int x, int y, int width, int height)
{
	// For simplicity, we assume sprites are stored as separate textures in this example.
    return nullptr;
}