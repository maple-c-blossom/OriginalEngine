#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <DirectXTex.h>
#include <string>
WarningIgnoreEnd
namespace MCB
{
	class TextureFile
	{
	public:
		DirectX::TexMetadata metadata_{};
		DirectX::ScratchImage scratchImg_{};

		~TextureFile();
		TextureFile();
		TextureFile(const TextureFile&) = delete;
		TextureFile& operator=(const TextureFile&) = delete;

		HRESULT LoadTexture(const wchar_t *FileName, const DirectX::WIC_FLAGS& flag);
		HRESULT LoadTexture(const std::string& directoryPath, const std::string& filename);
	};


}

