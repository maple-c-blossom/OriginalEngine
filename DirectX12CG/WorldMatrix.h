#pragma once

#include <DirectXMath.h>


#pragma region using namespace

using namespace std;
using namespace DirectX;

#pragma endregion using namespace

class WorldMatrix
{
public:
	//ワールド行列
	XMMATRIX matWorld = XMMatrixIdentity();
	//スケール行列
	XMMATRIX matScale = XMMatrixIdentity();
	//回転行列
	XMMATRIX matRot = XMMatrixIdentity();
	//平行移動行列
	XMMATRIX matTransform;

	/// <summary>
	/// X回転の設定
	/// </summary>
	/// <param name="angle">角度</param>
	/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
	void SetMatRotX(float angle,bool ConvertToRadians = true);
	/// <summary>
	/// Y回転設定
	/// </summary>
	/// <param name="angle">角度</param>
	/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
	void SetMatRotY(float angle, bool ConvertToRadians = true);
	/// <summary>
	/// Z回転の設定
	/// </summary>
	/// <param name="angle">角度</param>
	/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
	void SetMatRotZ(float angle, bool ConvertToRadians = true);

	/// <summary>
	/// 回転設定
	/// </summary>
	/// <param name="angleX">Xの角度</param>
	/// <param name="angleY">Yの角度</param>
	/// <param name="angleZ">Zの角度</param>
	/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
	void SetMatRot(float angleX, float angleY, float angleZ, bool ConvertToRadians = true);

	/// <summary>
	/// 引数で与えられた角度に変換された回転行列を返します
	/// </summary>
	/// <param name="matRot">回転元の行列</param>
	/// <param name="angleX">Xの角度</param>
	/// <param name="angleY">Yの角度</param>
	/// <param name="angleZ">Zの角度</param>
	/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
	/// <returns></returns>
	XMMATRIX ReturnMatRot(XMMATRIX matRot,float angleX, float angleY, float angleZ, bool ConvertToRadians = true);

	void SetMatScale(float scaleX, float scaleY, float scaleZ);

	void SetMatTrans(float transX, float transY, float transZ);

	void CreateMatrixWorld(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTransform);
	void UpdataMatrixWorld();

};

