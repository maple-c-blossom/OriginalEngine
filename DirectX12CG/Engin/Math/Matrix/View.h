#pragma once
#include "Pading.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <DirectXMath.h>
WarningIgnoreEnd

namespace MCB
{
    class View
    {
        public:
        DirectX::XMMATRIX mat_ = DirectX::XMMatrixIdentity();//行列
        DirectX::XMFLOAT3 eye_{ 0,0,0 }; //視点座標(カメラ自身の座標)
        //注視点座標(カメラが見ている座標、XMMatrixLookAtLHはこの座標から
        //視点座標へのベクトルを求めることで向きを決定している)
        DirectX::XMFLOAT3 target_{ 0,0,1 };
        DirectX::XMFLOAT3 up_{ 0,1,0 };//上方向ベクトル(カメラの上側がどこかを決定する座標。)
		Byte6 pad1;
		Byte6 pad2;
        DirectX::XMMATRIX billMat_ = DirectX::XMMatrixIdentity();


        /// <summary>
        /// 引数の値をクラスのそれぞれの変数に代入した上でビュー変換行列を作成します。
        /// </summary>
        /// <param name="eye">視点座標</param>
        /// <param name="target">注視点座標</param>
        /// <param name="up">上方向ベクトル</param>
        void CreateMatrixView(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

        /// <summary>
        /// すでにある変数を利用して行列を作成します
        /// </summary>
        void UpDateMatrixView( bool billbordY = false);
    };

}

