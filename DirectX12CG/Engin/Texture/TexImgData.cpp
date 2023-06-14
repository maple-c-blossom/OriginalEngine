#include "TexImgData.h"

MCB::TexImgData::~TexImgData()
{
    imageData_.clear();
}

void MCB::TexImgData::SetImageDataRGBA(const DirectX::XMFLOAT4& RGBA)
{
    for (int32_t i = 0; i < imageDataCount_; i++)
    {
    DirectX::XMFLOAT4 imageDataSeed = {0,0,0,0};
      imageDataSeed.x = RGBA.x;//R
      imageDataSeed.y = RGBA.y;//G
      imageDataSeed.z = RGBA.z;//B
      imageDataSeed.w = RGBA.w;//A
      imageData_.push_back(imageDataSeed);
    }
}

void MCB::TexImgData::SetImageDataRGBA(const Float4& RGBA)
{
    for (int32_t i = 0; i < imageDataCount_; i++)
    {
        DirectX::XMFLOAT4 imageDataSeed = { 0,0,0,0 };
        imageDataSeed.x = RGBA.x_;//R
        imageDataSeed.y = RGBA.y_;//G
        imageDataSeed.z = RGBA.z_;//B
        imageDataSeed.w = RGBA.w_;//A
        imageData_.push_back(imageDataSeed);
    }
}

void MCB::TexImgData::SetNoTextureFileImageDataRGBA(const Float4& RGBA)
{
    for (int32_t i = 0; i < imageDataCount_; i++)
    {
        DirectX::XMFLOAT4 imageDataSeed = { 0,0,0,0 };
        imageDataSeed.x = RGBA.x_;//R
        imageDataSeed.y = RGBA.y_;//G
        imageDataSeed.z = RGBA.z_;//B
        imageDataSeed.w = RGBA.w_;//A
        imageData_.push_back(imageDataSeed);
    }
}
