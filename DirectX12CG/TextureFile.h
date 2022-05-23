#pragma once
#include <DirectXTex.h>
#include <string>
namespace MCB
{
	class TextureFile
	{
	public:
		DirectX::TexMetadata metadata{};
		DirectX::ScratchImage scratchImg{};

		~TextureFile();

		HRESULT LoadTexture(const wchar_t *FileName,DirectX::WIC_FLAGS flag);
		HRESULT LoadTexture(const std::string& directoryPath, const std::string& filename);
	};


}

