#pragma once
#include <d3dx12.h>
#include "MipMap.h"
#include "TexImgData.h"
#include "TextureBuffer.h"
#include "TextureFile.h"
#include "Descriptor.h"
#include "Dx12.h"
#include "MCBFloat.h"
#include <assert.h>

namespace MCB
{
	class Texture
	{
	public:
		MipMap mipMap = {};
		TexImgData texImg = {};
		TextureBuffer texBuff = {};
		TextureFile texfile = {};
		Dx12* dx12 = Dx12::GetInstance();
		ShaderResource* srvptr = nullptr;
		unsigned short int incrementNum = 0;
		Texture() {};
		Texture(const Texture& texture) {};
		~Texture();

		void CreateTexture(const wchar_t* FileName,unsigned short int incrementNum);

		void CreateTexture(const std::string& directoryPath, const std::string& filename, unsigned short int incrementNum);

		void CreateTexture(const wchar_t* FileName);

		void CreateTexture( const std::string& directoryPath, const std::string& filename);

		void CreateNoTextureFileIsTexture(bool postEffect = false);

		void CreateNoTextureFileIsTexture(unsigned short int incrementNum, bool postEffect = false);
		
		[[nodiscard]] ID3D12Resource* UploadTexData();

		void SetSrvHeap();

	};

}