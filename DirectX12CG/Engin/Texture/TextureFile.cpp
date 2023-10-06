#include "TextureFile.h"

using namespace DirectX;
using namespace std;

MCB::TextureFile::~TextureFile()
{
    scratchImg_.Release();
  
}

MCB::TextureFile::TextureFile()
{
}


HRESULT MCB::TextureFile::LoadTexture(const wchar_t *FileName,const DirectX::WIC_FLAGS& flag)
{
	return LoadFromWICFile(FileName, flag, &metadata_, scratchImg_);
}

HRESULT MCB::TextureFile::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
    string filepath = directoryPath + filename;

    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

    return LoadFromWICFile(wfilepath, WIC_FLAGS_NONE,&metadata_,scratchImg_);
}