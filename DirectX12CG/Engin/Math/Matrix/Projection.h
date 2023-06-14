#pragma once

#include <DirectXMath.h>

namespace MCB
{
	class Projection
	{
	public:
		DirectX::XMMATRIX mat_; //行列
		float fovAngle_; //上下画角
		float aspectRatio_; //アスペクト比
		float nearZ_;//ファークリップ距離(前)
		float farZ_;//ファークリップ距離(後ろ)

		void CreateMatrixProjection( float fovAngle,  float aspectRatio,  float nearZ,  float farZ);
		void UpdataMatrixProjection();
	};
}

