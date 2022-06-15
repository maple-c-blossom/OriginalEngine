#pragma once
#include "MipMap.h"
#include "TexImgData.h"
#include "TextureBuffer.h"
#include "TextureFile.h"
#include "Descriptor.h"

namespace MCB
{
	class Texture
	{
	public:
		MipMap mipMap;
		TexImgData texImg;
		TextureBuffer texBuff;
		TextureFile texfile;

		void CreateTexture(Dx12 dx12, const wchar_t* FileName);

		void CreateTexture(Dx12 dx12, const std::string& directoryPath, const std::string& filename);


	};

}