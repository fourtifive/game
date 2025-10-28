#pragma once
#include<core/Math.h>

namespace ECS {
	struct Translate
	{
		Point position;
	};
	

	enum class RenderType {
		SingleImage,
		Sprite
	};

	struct Renderable {
		Renderable() 
		{}
		Renderable(RenderType t, std::string id, int x, int y, int width, int height) :
			type(t), spriteId(id), srcX(x), srcY(y), srcWidth(width), srcHeight(height)
		{}

		RenderType type;          // render type
		std::string spriteId;     // ID of the sprite/image to render
		
		//sprite specific
		int srcX = 0;
		int srcY = 0;
		int srcWidth = 0;
		int srcHeight = 0;
	};


}

