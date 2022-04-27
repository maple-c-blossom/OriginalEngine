#pragma once
#include <DirectXTex.h>
namespace MCB
{
	class TextureFile
	{
	public:
		DirectX::TexMetadata metadata{};
		DirectX::ScratchImage scratchImg{};

		HRESULT LoadTexture(const wchar_t *FileName,DirectX::WIC_FLAGS flag);
	};


}

