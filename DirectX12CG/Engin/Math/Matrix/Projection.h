#pragma once

#include <DirectXMath.h>

namespace MCB
{
	class Projection
	{
	public:
		DirectX::XMMATRIX mat_; //�s��
		float fovAngle_; //�㉺��p
		float aspectRatio_; //�A�X�y�N�g��
		float nearZ_;//�t�@�[�N���b�v����(�O)
		float farZ_;//�t�@�[�N���b�v����(���)

		void CreateMatrixProjection( float fovAngle,  float aspectRatio,  float nearZ,  float farZ);
		void UpdataMatrixProjection();
	};
}

