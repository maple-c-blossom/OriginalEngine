#pragma once
#include "MCBFloat.h"
namespace MCB
{

	class Vector3D
	{
	public:
		Float3 vec_;
		Vector3D(const Float3& start, const Float3& end);
		Vector3D(const Vector3D& start, const Vector3D& end);
		Vector3D();
		Vector3D(const Float3& vec);
		Vector3D( float x,  float y, float z);

		Vector3D V3Get(const Float3& start, const Float3& end);
		float V3Len() const;
		void V3Norm();
		float GetV3Dot(const Vector3D& vector);
		float GetInnerProduct( Vector3D vector);
		Vector3D GetV3Cross(const Vector3D& vector);
		Vector3D GetV3Cross(const Vector3D& aVector, const Vector3D& bVector);
		DirectX::XMVECTOR ConvertXMVEC();


		Vector3D GetFrontVec( Vector3D RightVec,  Vector3D UpVec = { 0,1,0 });
		Vector3D GetUpVec(Vector3D RightVec, Vector3D frontVec = { 0,0,1 });
		Vector3D GetRightVec( Vector3D frontVec,  Vector3D UpVec = { 0,1,0 });

		Vector3D& operator+=(const Vector3D& Vec);
		Vector3D& operator-=(const Vector3D& Vec);
		Vector3D& operator*=( float k);
		Vector3D& operator= (const Float3& a);
		Vector3D& operator= (const DirectX::XMVECTOR& a);
	};
	Vector3D operator+(const Vector3D& vecA, const Vector3D& vecB);
	Vector3D operator-(const Vector3D& vecA, const Vector3D& vecB);
	Vector3D operator*(const Vector3D& vecA,  float k);
	Vector3D operator*( float k, const Vector3D& vecA);
	Vector3D operator/(const Vector3D& vecA,  float k);
	Vector3D operator-(const Vector3D& vecA);

}

