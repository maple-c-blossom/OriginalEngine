#pragma once

#include <DirectXMath.h>


#pragma region using namespace

using namespace std;
using namespace DirectX;

#pragma endregion using namespace

class Projection
{
public:
	XMMATRIX mat; //行列
	float fovAngle; //上下画角
	float aspectRatio; //アスペクト比
	float nearZ;//ファークリップ距離(前)
	float farZ;//ファークリップ距離(後ろ)

	void CreateMatrixProjection(float fovAngle, float aspectRatio, float nearZ, float farZ);
	void UpdataMatrixProjection();
};

