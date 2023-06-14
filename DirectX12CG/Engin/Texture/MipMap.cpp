#include "MipMap.h"

using namespace DirectX;


HRESULT MCB::MipMap::GenerateMipMap(TextureFile* scratchimg,const DirectX::TEX_FILTER_FLAGS& flag, size_t levels)
{
	HRESULT result;
	//ミップマップ生成
	result = GenerateMipMaps(scratchimg->scratchImg_.GetImages(), scratchimg->scratchImg_.GetImageCount(), scratchimg->scratchImg_.GetMetadata(), flag, levels, mipChain_);
	if (SUCCEEDED(result))
	{
		scratchimg->scratchImg_ = std::move(mipChain_);
		scratchimg->metadata_ = scratchimg->scratchImg_.GetMetadata();
	}
	scratchimg->metadata_.format = MakeSRGB(scratchimg->metadata_.format);
	
	return result;
}
