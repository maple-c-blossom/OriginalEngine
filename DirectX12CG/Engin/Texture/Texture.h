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
		MipMap mipMap_ = {};
		TexImgData texImg_ = {};
		TextureBuffer texBuff_ = {};
		TextureFile texfile_ = {};
		ShaderResource* srvptr_ = nullptr;
		uint16_t incrementNum_ = 0;
		Texture() {};
		Texture(const Texture& texture) {};
		~Texture();

		void CreateTexture(const wchar_t* FileName, uint16_t incrementNum);

		void CreateTexture(const std::string& directoryPath, const std::string& filename,  uint16_t incrementNum);

		void CreateTexture(const wchar_t* FileName);

		void CreateTexture( const std::string& directoryPath, const std::string& filename);

		void CreateNoTextureFileIsTexture( bool postEffect = false);

		void CreateNoTextureFileIsTexture( uint16_t incrementNum, bool postEffect = false);

		void SetSrvHeap();

	};

}