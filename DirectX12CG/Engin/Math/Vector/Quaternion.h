#pragma once
#include "Vector3D.h"
#include "MCBMatrix.h"
namespace MCB
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;
		Quaternion(const Vector3D& vec, const float& angle);
		Quaternion();
		Quaternion(const float& x, const float& y,const float& z,const float& w);

		//与えられたベクトル周りの回転を表すクォータニオンを生成
		void SetRota(const Vector3D& vec,const float& angle);
		//共役
		Quaternion GetConjugated (const Quaternion& q);
		//逆数（数学用語より）
		Quaternion GetReciprocal(const Quaternion& q);
		//長さ
		double GetNorm();
		//q*pの直積
		Quaternion GetDirectProduct(const Quaternion& q, const Quaternion& p);
		//内積
		float Dot(const Quaternion& a, const Quaternion& b);
		//二つのクォータニオンの角度差
		float GetAngle(const Quaternion& a, const Quaternion& b, float& dot,bool& nan);
		float GetAngle(const Quaternion& a, const Quaternion& b);
		//球面線形補間
		Quaternion Slerp(const Quaternion& start, const Quaternion& end, const int32_t& time, const int32_t& maxTime);
		//球面線形補間
		Quaternion Slerp(const Quaternion& start, const Quaternion& end, const float& time);
		//与えられたベクトル軸周りの回転をPositionVecに与える
		Vector3D SetRotationVector(const Vector3D& rotationAxisVec, const Vector3D& PositionVec, const float& angle);
		//与えられたクォータニオンの回転をPositionVecに与える
		Vector3D SetRotationVector(const Quaternion& rotationQuaternion, const Vector3D& PositionVec);
		//何のために作ったの作ったんでしょうかね
		Vector3D SetRotationVector(const Quaternion& rotationQuaternion, const Quaternion& PositionVec);

		Quaternion DirToDir(const Vector3D& u, const Vector3D& v);
		//正規化する
		void Normalize();
		//正規化した値を返す
		static Quaternion Normalize(const Quaternion& q);
		//回転行列に変換
		MCBMatrix GetQuaternionRotaMat(const Quaternion& rotaQ);
		
		void SinCos(float* returnSin, float* returnCos, float theta);
		//多分オイラー角からクォータニオンを生成
		Quaternion SetToRorateObjectToInternal(const Float3& eulerAngle);
		//クォータニオンの回転軸を算出
		Vector3D GetRotationAxis(const Quaternion& q);
		//クォータニオンの回転軸を算出(AxisVecを書き換える)
		void GetRotationAxis(const Quaternion& q,Vector3D& AxisVec);
		//単位クォータニオン
		Quaternion Identity();

		//クォータニオンの角度
		float GetAngle(const Quaternion& q);
		
		float SafeAcos(const float& a);

		bool operator== (const Quaternion& q);
		Quaternion operator-();
	};
	Quaternion SetRota(const Vector3D& vec, const float& angle);
}

