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
		MipMap mipMap = {};
		TexImgData texImg = {};
		TextureBuffer texBuff = {};
		TextureFile texfile = {};
		ShaderResource* srvptr = nullptr;
		unsigned short int incrementNum = 0;
		Texture() {};
		Texture(const Texture& texture) {};
		~Texture();

		void CreateTexture(const wchar_t* FileName,unsigned short int incrementNum);

		void CreateTexture(const std::string& directoryPath, const std::string& filename, unsigned short int incrementNum);

		void CreateTexture(const wchar_t* FileName);

		void CreateTexture( const std::string& directoryPath, const std::string& filename);

		void CreateNoTextureFileIsTexture();

		void CreateNoTextureFileIsTexture(unsigned short int incrementNum);

		void SetSrvHeap();

	};

}