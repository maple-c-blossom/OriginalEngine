#pragma once
#include "Vector3.h"
#include <stdint.h>

namespace MCB
{
	class Quaternion
	{
	public:
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		float w = 1.f;
		Quaternion(const Vector3& vec, float angle);
		Quaternion();
		Quaternion(float x, float y, float z, float w);

		//与えられたベクトル周りの回転を表すクォータニオンを生成
		void SetRota(Vector3 vec, float angle);
		//共役
		Quaternion GetConjugated(Quaternion q);
		//逆数（数学用語より
		Quaternion GetReciprocal(Quaternion q);
		//長さ
		double GetNorm();
		//q*pの直積
		Quaternion GetDirectProduct(const Quaternion& q, const Quaternion& p);
		//内積
		float Dot(const Quaternion& a, const Quaternion& b);
		//二つのクォータニオンの角度差
		float GetAngle(const Quaternion& a, const Quaternion& b, float& dot, bool& nan);
		float GetAngle(const Quaternion& a, const Quaternion& b);
		//球面線形補間
		Quaternion Slerp(Quaternion start, const Quaternion& end, int32_t time, int32_t maxTime);
		//球面線形補間
		Quaternion Slerp(Quaternion start, Quaternion end, float time);
		//与えられたベクトル軸周りの回転をPositionVecに与える
		Vector3 SetRotationVector(const Vector3& rotationAxisVec, Vector3 PositionVec, float angle);
		//与えられたクォータニオンの回転をPositionVecに与える
		Vector3 SetRotationVector(const Quaternion& rotationQuaternion, Vector3 PositionVec);


		Vector3 SetRotationVector(const Quaternion& rotationQuaternion, const Quaternion& PositionVec);

		Quaternion DirToDir(Vector3 u, Vector3 v);
		//正規化する
		void Normalize();
		//正規化した値を返す
		static Quaternion Normalize(Quaternion q);
		

		void SinCos(float* returnSin, float* returnCos, float theta);
		//オイラー角からクォータニオンを生成
		Quaternion SetToRorateObjectToInternal(const Vector3& eulerAngle);
		//クォータニオンの回転軸を算出
		Vector3 GetRotationAxis(const Quaternion& q);
		//クォータニオンの回転軸を算出(AxisVecを書き換える)
		void GetRotationAxis(const Quaternion& q, Vector3& AxisVec);
		//単位クォータニオン
		Quaternion Identity();

		//クォータニオンの角度
		float GetAngle(const Quaternion& q);

		float SafeAcos(float a);

		bool operator== (const Quaternion& q);
		Quaternion operator-();
		Quaternion operator*(float k);
		Quaternion operator+(Quaternion q);
	};
	Quaternion operator*(float k, Quaternion q);
	Quaternion SetRota(const Vector3& vec, float angle);
}

