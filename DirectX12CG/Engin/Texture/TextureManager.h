#pragma once
#include "Texture.h"
#include <list>
#include <memory>
namespace MCB
{

	typedef struct TextureCell
	{
		std::unique_ptr<Texture> texture;
		bool free = false;
		TextureCell() { texture = std::make_unique<Texture>(); }

	}TextureCell;
	class TextureManager
	{
		TextureManager() { };
		TextureManager(const TextureManager& textureManager) = delete;
		TextureManager& operator=(const TextureManager& textureManager) = delete;
		~TextureManager() {};
		 std::list<std::unique_ptr<TextureCell>> textures;
		//std::vector<int> texincrement;
	public:
		//int LoadTexture(const wchar_t* FileName, uint16_t incrementNum);
		static void DeleteInstace();
		static TextureManager* GetInstance();
		//int LoadTexture(const std::string& directoryPath, const std::string& filename, uint16_t incrementNum);

		TextureCell* LoadTexture(const wchar_t* FileName);

		TextureCell* LoadTexture(const std::string& directoryPath, const std::string& filename);

		TextureCell* CreateNoTextureFileIsTexture(bool postEffect = false);

		static TextureManager* instance;

		void Clear();
		
		void Erase();

		//void SetDelete(int index);

		//Texture* GetTexture(uint16_t incrementNum);
	};
}