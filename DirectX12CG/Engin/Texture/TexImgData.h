#pragma once

#include <DirectXMath.h>
#include<vector>
#include "MCBFloat.h"

namespace MCB
{
	class TexImgData
	{
	public:
        //�������s�N�Z����
        size_t textureWidth_ = 256;
        //�c�����s�N�Z����
        size_t textureHeight_ = 256;
        //�z��̗v�f��
        size_t imageDataCount_ = textureWidth_ * textureHeight_;
        //�摜�C���[�W�f�[�^�̔z��
       std::vector<DirectX::XMFLOAT4> imageData_;
    
       ~TexImgData();
        void SetImageDataRGBA(const DirectX::XMFLOAT4& RGBA);
        void SetImageDataRGBA(const Float4& RGBA);

        void SetNoTextureFileImageDataRGBA(const Float4& RGBA);
  

	};

}

