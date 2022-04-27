#include "MipMap.h"

using namespace DirectX;


HRESULT MCB::MipMap::GenerateMipMap(TextureFile* scratchimg, DirectX::TEX_FILTER_FLAGS flag, size_t levels)
{
	HRESULT result;
	//ミップマップ生成
	result = GenerateMipMaps(scratchimg->scratchImg.GetImages(), scratchimg->scratchImg.GetImageCount(), scratchimg->scratchImg.GetMetadata(), flag, levels, mipChain);
	if (SUCCEEDED(result))
	{
		scratchimg->scratchImg = std::move(mipChain);
		scratchimg->metadata = scratchimg->scratchImg.GetMetadata();
	}
	scratchimg->metadata.format = MakeSRGB(scratchimg->metadata.format);
	
	return result;
}
