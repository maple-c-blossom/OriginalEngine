#pragma once
#include "Vector3D.h"
namespace MCB
{
	class Quaternion
	{
		Float4 quaternion = { 0,0,0,0 };
		
		/// <summary>
		/// vec軸でのangle分の回転を表すクォータニオンを生成
		/// </summary>
		/// <param name="vec">回転軸のベクトル</param>
		/// <param name="angle">回転角度(ラジアン)</param>
		void SetRotaQuaternion(Vector3D vec,float angle);

		/// <summary>
		/// 引数で渡されたクォータニオンの共役を返す
		/// </summary>
		/// <param name="q">共役がほしいクォータニオン</param>
		/// <returns></returns>
		Quaternion GetQuaternionConjugated(Quaternion q);

		/// <summary>
		/// クォータニオンの逆数を返す
		/// </summary>
		/// <param name="q">逆数がほしいクォータニオン</param>
		/// <returns></returns>
		Quaternion GetQuaternionReciprocal(Quaternion q);

		/// <summary>
		/// 二つのクォータニオンの直積を返す
		/// ※引数の順番が変わると結果も変わるので注意すること!!
		/// </summary>
		/// <param name="q">左辺におきたいクォータニオン</param>
		/// <param name="p">右辺におきたいクォータニオン</param>
		/// <returns></returns>
		Quaternion GetCartesianProduct(Quaternion q, Quaternion p);

		//cosθからsinθへの変換
		// cos = q1とq2の内積
		//sin = sqrtf(1 - (cos*cos));
		
		//θ = acos(q1とq2の内積)
		//qSLerp = (q1*sin((1-t)θ) + q2sin(tθ) ) / sinθ

	};
}

