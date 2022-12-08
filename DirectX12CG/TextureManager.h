#pragma once
#include "Texture.h"
#include <vector>
#include <memory>
namespace MCB
{

	typedef struct TextureCell
	{
		std::unique_ptr<Texture> texture;
		bool deleteFlag = false;
		TextureCell() { texture = std::make_unique<Texture>(); }

	};
	class TextureManager
	{
		TextureManager() { };
		TextureManager(const TextureManager& textureManager) = delete;
		TextureManager& operator=(const TextureManager& textureManager) = delete;
		~TextureManager() {};
		 std::vector<std::unique_ptr<TextureCell>> textures;
		//std::vector<int> texincrement;
	public:
		//int LoadTexture(const wchar_t* FileName, unsigned short int incrementNum);
		static void DeleteInstace();
		static TextureManager* GetInstance();
		//int LoadTexture(const std::string& directoryPath, const std::string& filename, unsigned short int incrementNum);

		int LoadTexture(const wchar_t* FileName);

		int LoadTexture(const std::string& directoryPath, const std::string& filename);

		int CreateNoTextureFileIsTexture();

		static TextureManager* instance;

		void Clear();
		
		void Erase();

		void SetDelete(int index);

		Texture* GetTexture(unsigned short int incrementNum);
	};
}