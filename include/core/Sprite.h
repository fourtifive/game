#pragma once
#include <string>
#include<glew.h>
#include<glfw3.h>

class Sprite {
public:
    Sprite(GLuint Texture, int Width, int Height)
        : texture(Texture),width(Width), height(Height) { }

    GLuint Get_Texture() const { return texture; }
    int Get_Width() const { return width; }
    int Get_Height() const { return height; }

private:
    GLuint texture;
    int width, height;
};