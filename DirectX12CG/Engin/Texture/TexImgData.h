#pragma once

#include <DirectXMath.h>
#include<vector>
#include "MCBFloat.h"

namespace MCB
{
	class TexImgData
	{
	public:
        //横方向ピクセル数
        size_t textureWidth_ = 256;
        //縦方向ピクセル数
        size_t textureHeight_ = 256;
        //配列の要素数
        size_t imageDataCount_ = textureWidth_ * textureHeight_;
        //画像イメージデータの配列
       std::vector<DirectX::XMFLOAT4> imageData_;
    
       ~TexImgData();
        void SetImageDataRGBA(const DirectX::XMFLOAT4& RGBA);
        void SetImageDataRGBA(const Float4& RGBA);

        void SetNoTextureFileImageDataRGBA(const Float4& RGBA);
  

	};

}

