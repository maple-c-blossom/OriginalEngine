#include "TexImgData.h"

MCB::TexImgData::~TexImgData()
{
    imageData.clear();
}

void MCB::TexImgData::SetImageDataRGBA(DirectX::XMFLOAT4 RGBA)
{
    DirectX::XMFLOAT4 imageDataSeed;
    for (int i = 0; i < imageDataCount; i++)
  {
      imageData.push_back(imageDataSeed);
      imageData[i].x = RGBA.x;//R
      imageData[i].y = RGBA.y;//G
      imageData[i].z = RGBA.z;//B
      imageData[i].w = RGBA.w;//A
  }
}
