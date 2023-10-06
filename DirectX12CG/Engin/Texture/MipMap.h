#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <DirectXTex.h>
WarningIgnoreEnd
#include "TextureFile.h"
namespace MCB
{
	class MipMap
	{
	public:
		DirectX::ScratchImage mipChain_{};


		HRESULT GenerateMipMap(TextureFile* scratchimg, const DirectX::TEX_FILTER_FLAGS& flag,  size_t levels);
		MipMap(MipMap& mipmap) = delete;
		MipMap& operator=(MipMap& mipmap) = delete;
		MipMap();
	};
}
