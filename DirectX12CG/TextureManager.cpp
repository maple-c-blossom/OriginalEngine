#include "TextureManager.h"

using namespace MCB;
using namespace std;

int MCB::TextureManager::LoadTexture(const wchar_t* FileName, unsigned short int incrementNum)
{
	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
	temp->texture->CreateTexture(FileName, incrementNum);
	textures.push_back(move(temp));
}

int MCB::TextureManager::LoadTexture(const std::string& directoryPath, const std::string& filename, unsigned short int incrementNum)
{
	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
	temp->texture->CreateTexture(directoryPath, filename,incrementNum);
	textures.push_back(move(temp));
}

int MCB::TextureManager::LoadTexture(const wchar_t* FileName)
{
	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
	temp->texture->CreateTexture(FileName, textures.size());
	textures.push_back(move(temp));
}

int MCB::TextureManager::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
	temp->texture->CreateTexture(directoryPath, filename, textures.size());
	textures.push_back(move(temp));
}

int MCB::TextureManager::CreateNoTextureFileIsTexture()
{
	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
	temp->texture->CreateNoTextureFileIsTexture(textures.size());
	textures.push_back(move(temp));
}

void MCB::TextureManager::Clear()
{
	textures.clear();
}

void MCB::TextureManager::Erase()
{
	std::remove_if(textures.begin(), textures.end(), [](TextureCell itr) { return itr.deleteFlag; });
}

Texture* MCB::TextureManager::GetTexture(unsigned short int incrementNum)
{
	return textures[incrementNum]->texture.get();
}
