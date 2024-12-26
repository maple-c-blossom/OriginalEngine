#pragma once
namespace MCB
{

	class Vector3
	{
	public:
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		Vector3(const Vector3& start, const Vector3& end);
		Vector3();
		Vector3(float x, float y, float z);

		Vector3 Vector3Substruct(const Vector3& start, const Vector3& end);
		float V3Len() const;
		void V3Norm();
		Vector3 GetV3Norm();
		float GetV3Dot(const Vector3& vector);
		float GetInnerProduct(Vector3 vector);
		Vector3 GetV3Cross(const Vector3& vector);
		Vector3 GetV3Cross(const Vector3& aVector, const Vector3& bVector);


		Vector3 GetFrontVec(Vector3 RightVec, Vector3 UpVec = { 0,1,0 });
		Vector3 GetUpVec(Vector3 RightVec, Vector3 frontVec = { 0,0,1 });
		Vector3 GetRightVec(Vector3 frontVec, Vector3 UpVec = { 0,1,0 });

		Vector3 GetV3Normal(Vector3 v0, Vector3 v1, Vector3 v2);

		Vector3 Qlamp(const Vector3& vec, const Vector3& min, const Vector3& max);

		Vector3& operator+=(const Vector3& Vec);
		Vector3& operator-=(const Vector3& Vec);
		Vector3& operator*=(float k);

		Vector3 Clamp(const Vector3& vec1, const Vector3& min, const Vector3& max, bool* isClamped = nullptr);

	};
	Vector3 operator+(const Vector3& vecA, const Vector3& vecB);
	bool operator!=(const Vector3& vecA, const Vector3& vecB);
	Vector3 operator-(const Vector3& vecA, const Vector3& vecB);
	Vector3 operator*(const Vector3& vecA, float k);
	Vector3 operator*(float k, const Vector3& vecA);
	Vector3 operator/(const Vector3& vecA, float k);
	Vector3 operator-(const Vector3& vecA);


}

