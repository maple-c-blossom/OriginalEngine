#pragma once
#include <DirectXTex.h>
#include "TextureFile.h"
namespace MCB
{
	class MipMap
	{
	public:
		DirectX::ScratchImage mipChain{};

		HRESULT GenerateMipMap(TextureFile* scratchimg, DirectX::TEX_FILTER_FLAGS flag, size_t levels);

	};
}
