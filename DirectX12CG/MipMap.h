#pragma once
#include <DirectXTex.h>
namespace MCB
{
	class MipMap
	{
	public:
		DirectX::ScratchImage mipChain{};

		HRESULT GenerateMipMap(DirectX::ScratchImage* scratchimg, DirectX::TEX_FILTER_FLAGS flag, size_t levels, TexMetadata& metadata);
	};
}
