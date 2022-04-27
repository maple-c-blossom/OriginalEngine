#include "TextureFile.h"

using namespace DirectX;

HRESULT MCB::TextureFile::LoadTexture(const wchar_t *FileName, DirectX::WIC_FLAGS flag)
{
	return LoadFromWICFile(FileName, flag, &metadata, scratchImg);
}
