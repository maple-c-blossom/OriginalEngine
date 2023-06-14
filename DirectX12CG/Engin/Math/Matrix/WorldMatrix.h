#pragma once

#include <DirectXMath.h>
#include "View.h"

namespace MCB
{
	class WorldMatrix
	{
	public:
		//ワールド行列
		DirectX::XMMATRIX matWorld_ = DirectX::XMMatrixIdentity();
		//スケール行列
		DirectX::XMMATRIX matScale_ = DirectX::XMMatrixIdentity();
		//回転行列
		DirectX::XMMATRIX matRot_ = DirectX::XMMatrixIdentity();
		//平行移動行列
		DirectX::XMMATRIX matTransform_;





		/// <summary>
		/// X回転の設定
		/// </summary>
		/// <param name="angle">角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		void SetMatRotX( float angle,  bool ConvertToRadians = true);
		/// <summary>
		/// Y回転設定
		/// </summary>
		/// <param name="angle">角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		void SetMatRotY( float angle,  bool ConvertToRadians = true);
		/// <summary>
		/// Z回転の設定
		/// </summary>
		/// <param name="angle">角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		void SetMatRotZ( float angle,  bool ConvertToRadians = true);

		/// <summary>
		/// 回転設定
		/// </summary>
		/// <param name="angleX">Xの角度</param>
		/// <param name="angleY">Yの角度</param>
		/// <param name="angleZ">Zの角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		void SetMatRot( float angleX,  float angleY,  float angleZ,  bool ConvertToRadians = true);



		/// <summary>
		/// 引数で与えられた角度に変換された回転行列を返します
		/// </summary>
		/// <param name="matRot">回転元の行列</param>
		/// <param name="angleX">Xの角度</param>
		/// <param name="angleY">Yの角度</param>
		/// <param name="angleZ">Zの角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		/// <returns></returns>
		DirectX::XMMATRIX ReturnMatRot(const DirectX::XMMATRIX& matRot,  float angleX,  float angleY,
			 float angleZ,  bool ConvertToRadians = true);

		void SetMatScale( float scaleX,  float scaleY,  float scaleZ);

		void SetMatTrans( float transX,  float transY,  float transZ);

		void CreateMatrixWorld(const DirectX::XMMATRIX& matScale, const DirectX::XMMATRIX& matRot, 
			const DirectX:: XMMATRIX& matTransform);

		void UpdataBillBordMatrixWorld(const View& view);

		void UpdataMatrixWorld();


	};
}

