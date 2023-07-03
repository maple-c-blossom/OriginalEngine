#pragma once
#include <DirectXTex.h>
#include <string>
namespace MCB
{
	class TextureFile
	{
	public:
		DirectX::TexMetadata metadata_{};
		DirectX::ScratchImage scratchImg_{};

		~TextureFile();

		HRESULT LoadTexture(const wchar_t *FileName, const DirectX::WIC_FLAGS& flag);
		HRESULT LoadTexture(const std::string& directoryPath, const std::string& filename);
	};


}

