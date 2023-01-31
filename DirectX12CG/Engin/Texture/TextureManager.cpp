#include "TextureManager.h"

using namespace MCB;
using namespace std;

//int MCB::TextureManager::LoadTexture(const wchar_t* FileName, unsigned short int incrementNum)
//{
//	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
//	temp->texture->CreateTexture(FileName, incrementNum);
//	textures.push_back(move(temp));
//}
//
//int MCB::TextureManager::LoadTexture(const std::string& directoryPath, const std::string& filename, unsigned short int incrementNum)
//{
//	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
//	temp->texture->CreateTexture(directoryPath, filename,incrementNum);
//	textures.push_back(move(temp));
//}

TextureCell* MCB::TextureManager::LoadTexture(const wchar_t* FileName)
{
	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
	int tempin = 0;
	for (auto& itr : textures)
	{
		if (tempin == itr->texture->incrementNum)
		{
			tempin++;
			continue;
		}
	}

	temp->texture->CreateTexture(FileName,tempin);
	textures.push_back(move(temp));
	//assert(textures.size() < 20);
	//texincrement.push_back(tempin);
	return textures.rbegin()->get();
}

TextureCell* MCB::TextureManager::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
	int tempin = 0;
	for (auto& itr : textures)
	{
		if (tempin == itr->texture->incrementNum)
		{
			tempin++;
			continue;
		}
	}
		temp->texture->CreateTexture(directoryPath, filename, tempin);
		textures.push_back(move(temp));
		//assert(textures.size() < 20);

	//texincrement.push_back(tempin);
	return textures.rbegin()->get();
}

TextureCell* MCB::TextureManager::CreateNoTextureFileIsTexture()
{
	std::unique_ptr<TextureCell> temp = make_unique<TextureCell>();
	int tempin = 0;
	for (auto& itr : textures)
	{
		if (tempin == itr->texture->incrementNum)
		{
			tempin++;
			continue;
		}
	}
	temp->texture->CreateNoTextureFileIsTexture(tempin);
	textures.push_back(move(temp));
	//assert(textures.size() < 20);
	//texincrement.push_back(tempin);
	return textures.rbegin()->get();
}

void MCB::TextureManager::Clear()
{
	textures.clear();
}

void MCB::TextureManager::Erase()
{
	//for (auto& itr : textures)
	//{
	//	std::remove_if(texincrement.begin(), texincrement.end(), [&itr](int itr2) { return (itr2 == itr.get()->texture.get()->incrementNum) && itr.get()->deleteFlag; });
	//}
	textures.remove_if([](std::unique_ptr<TextureCell>& itr) { return itr->free; });
}

//Texture* MCB::TextureManager::GetTexture(unsigned short int incrementNum)
//{
//	//return textures[incrementNum]->texture.get();
//}

//void MCB::TextureManager::SetDelete(int index)
//{
//	//textures[index]->free = true;
//	//textures[index]->texture.reset();
//}


TextureManager* MCB::TextureManager::GetInstance()
{
	static TextureManager instance;// = new TextureManager();
	return &instance;
}

void MCB::TextureManager::DeleteInstace()
{
	//delete TextureManager::GetInstance();
}
