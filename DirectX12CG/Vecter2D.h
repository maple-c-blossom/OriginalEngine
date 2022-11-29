#pragma once

namespace MCB
{


	class Vector2D
	{


	public:
		float x = 0;
		float y = 0;

		Vector2D();
		Vector2D(float x, float y);


		float GetCrossProduct(Vector2D a, Vector2D b);
		float GetCrossProduct(Vector2D orher);
		float GetDot(Vector2D vec);

		Vector2D VecGet(Vector2D startVec, Vector2D endVec);
		Vector2D VecGet(Vector2D startVec);
		bool HitCrossProduct(Vector2D* points, const int ArraySize, const Vector2D point);
		bool HitCrossProduct(Vector2D* points, const int ArraySize);
		float GetLenge() const;
		void VecNorm();




		float GetX() const;
		float GetY() const;

		void SetX(float x);
		void SetY(float y);
		void SetVec(Vector2D orher);

		Vector2D operator +();

		Vector2D operator -();

		Vector2D& operator +=(const Vector2D& other);

		Vector2D& operator -=(const Vector2D& other);

		Vector2D& operator *=(float speed);

		Vector2D& operator /=(float Lenge);

	};


	Vector2D operator +(const Vector2D& a, const Vector2D& b);

	Vector2D operator -(const Vector2D& a, const Vector2D& b);

	Vector2D operator *(Vector2D a, float speed);

	Vector2D operator *(float speed, Vector2D a);

	Vector2D operator /(Vector2D a, float Lenge);

}