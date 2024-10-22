#pragma once
#include "Texture.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <list>
#include <memory>
WarningIgnoreEnd
namespace MCB
{

	typedef struct TextureCell
	{
		std::unique_ptr<Texture> texture;
		bool free = false;
		Byte7 pad;
		TextureCell();
		TextureCell(const TextureCell&) = delete;
		TextureCell& operator=(const TextureCell& ) = delete;

	}TextureCell;

	class TextureManager
	{
		TextureManager();
		TextureManager(const TextureManager& textureManager) = delete;
		TextureManager& operator=(const TextureManager& textureManager) = delete;
		~TextureManager();
		 std::list<std::unique_ptr<TextureCell>> textures_;
		//std::vector<int32_t> texincrement;
	public:
		//int32_t LoadTexture(const wchar_t* FileName, uint16_t incrementNum);
		static void DeleteInstace();
		static TextureManager* GetInstance();
		//int32_t LoadTexture(const std::string& directoryPath, const std::string& filename, uint16_t incrementNum);

		TextureCell* LoadTexture(const wchar_t* FileName);

		TextureCell* LoadTexture(const std::string& directoryPath, const std::string& filename);

		TextureCell* CreateNoTextureFileIsTexture( bool postEffect = false);

		static TextureManager* sinstance_;

		void Clear();
		
		void Erase();

		//void SetDelete(int32_t index);

		//Texture* GetTexture(uint16_t incrementNum);
	};
}