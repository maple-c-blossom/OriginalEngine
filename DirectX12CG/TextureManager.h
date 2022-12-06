#pragma once
#include "Texture.h"
#include <vector>
#include <memory>
namespace MCB
{
	class TextureManager
	{
		typedef struct TextureCell
		{
			std::unique_ptr<Texture> texture;
			bool deleteFlag = false;
		};
		std::vector<std::unique_ptr<TextureCell>> textures;
		std::vector<int> texincrement;
	public:
		//int LoadTexture(const wchar_t* FileName, unsigned short int incrementNum);

		//int LoadTexture(const std::string& directoryPath, const std::string& filename, unsigned short int incrementNum);

		int LoadTexture(const wchar_t* FileName);

		int LoadTexture(const std::string& directoryPath, const std::string& filename);

		int CreateNoTextureFileIsTexture();

		void Clear();
		
		void Erase();

		Texture* GetTexture(unsigned short int incrementNum);
	};
}