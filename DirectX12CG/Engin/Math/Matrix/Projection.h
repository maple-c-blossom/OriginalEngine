#pragma once

#include <DirectXMath.h>

namespace MCB
{
	class Projection
	{
	public:
		DirectX::XMMATRIX mat; //行列
		float fovAngle; //上下画角
		float aspectRatio; //アスペクト比
		float nearZ;//ファークリップ距離(前)
		float farZ;//ファークリップ距離(後ろ)

		void CreateMatrixProjection(float fovAngle, float aspectRatio, float nearZ, float farZ);
		void UpdataMatrixProjection();
	};
}

