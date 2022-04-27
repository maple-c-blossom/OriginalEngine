#include "TexImgData.h"

void MCB::TexImgData::SetImageDataRGBA(DirectX::XMFLOAT4 RGBA)
{
    for (int i = 0; i < imageDataCount; i++)
  {
      imageData[i].x = RGBA.x;//R
      imageData[i].y = RGBA.y;//G
      imageData[i].z = RGBA.z;//B
      imageData[i].w = RGBA.w;//A
  }
}
