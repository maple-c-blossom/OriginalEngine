#pragma once
#include <cstdint>
namespace MCB
{


	class Vector2D
	{


	public:
		float x_ = 0;
		float y_ = 0;

		Vector2D();
		Vector2D(float x, float y);


		float GetCrossProduct(const Vector2D& a, const Vector2D& b);
		float GetCrossProduct(const Vector2D& orher);
		float GetDot(const Vector2D& vec);

		Vector2D VecGet(const Vector2D& startVec, const Vector2D& endVec);
		Vector2D VecGet(const Vector2D& startVec);
		bool HitCrossProduct(Vector2D* points,  int32_t ArraySize, const Vector2D& point);
		bool HitCrossProduct(Vector2D* points,  int32_t ArraySize);
		float GetLenge() const;
		void VecNorm();




		float GetX() const;
		float GetY() const;

		void SetX( float x);
		void SetY( float y);
		void SetVec(const Vector2D& orher);

		Vector2D operator +();

		Vector2D operator -();

		Vector2D& operator +=(const Vector2D& other);

		Vector2D& operator -=(const Vector2D& other);

		Vector2D& operator *=( float speed);

		Vector2D& operator /=( float Lenge);

	};


	Vector2D operator +(const Vector2D& a, const Vector2D& b);

	Vector2D operator -(const Vector2D& a, const Vector2D& b);

	Vector2D operator *(const Vector2D& a, const float& speed);

	Vector2D operator *(const float& speed, const Vector2D& a);

	Vector2D operator /(const Vector2D& a, const float& Lenge);

}