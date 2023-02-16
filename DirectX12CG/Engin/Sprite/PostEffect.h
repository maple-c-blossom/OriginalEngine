#pragma once
#include "Sprite.h"

namespace MCB
{
	class PostEffect :public Sprite
	{
	public:
		PostEffect();
		TextureCell* tex;
		void Draw();
	};
}

