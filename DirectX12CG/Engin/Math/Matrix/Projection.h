#pragma once

#include <DirectXMath.h>

namespace MCB
{
	class Projection
	{
	public:
		DirectX::XMMATRIX mat; //�s��
		float fovAngle; //�㉺��p
		float aspectRatio; //�A�X�y�N�g��
		float nearZ;//�t�@�[�N���b�v����(�O)
		float farZ;//�t�@�[�N���b�v����(���)

		void CreateMatrixProjection(float fovAngle, float aspectRatio, float nearZ, float farZ);
		void UpdataMatrixProjection();
	};
}

