#pragma once

#include "Vector3.h"
#include <array>
#include "Quaternion.h"

namespace MCB
{

	class Matrix
	{
	public:
#pragma warning(push)
#pragma warning(disable:4201)
		union
		{
			float matOneArray[16];
			float matTowArray[4][4];
			struct
			{
				float _11_; float _12_; float _13_; float _14_;
				float _21_; float _22_; float _23_; float _24_;
				float _31_; float _32_; float _33_; float _34_;
				float _41_; float _42_; float _43_; float _44_;
			};
		};
#pragma warning(pop)
	public:
		Matrix();
		void MatrixIdentity();

		static Matrix MatrixTranslate(float x, float y, float z);

		static Matrix MatrixTranslate(Vector3 pos);

		static Matrix MatrixScaling(float x, float y, float z);

		static Matrix MatrixScaling(Vector3 scale);

		void ConvertMatrixArray(float** ArrayMat);

		void ConvertMatrixMCBMat(float** ArrayMat);

		void ConvertMatrixMCBMat(std::array<std::array<float, 4>, 4> ArrayMat);

		static std::array<std::array<float, 4>, 4> GetArrayMat(Matrix mat);

		static Matrix MatrixRotaX(float angle);

		static Matrix MatrixRotaY(float angle);

		static Matrix MatrixRotaZ(float angle);

		static Matrix GetQuaternionRotaMat(const Quaternion& rotaQ);

		static Matrix ReturnMatrixIdentity();

		static Matrix MatrixTranspose(Matrix mat);

		static Matrix MatrixInverse(Matrix mat);

		static Vector3 Transform(const Vector3& v, const Matrix& m);

		static Vector3 GetTranslate(const Matrix& m);

		Vector3 GetRotationToEuler();


		Matrix operator*(float s);
		Matrix operator/(float s);
		Matrix operator+(const Matrix& matrix);
		Matrix operator-(const Matrix& matrix);

		Matrix& operator*=(const Matrix& matrix);
		Matrix& operator*=(float s);
		Matrix& operator/=(float s);
		Matrix& operator+=(const Matrix& matrix);
		Matrix& operator-=(const Matrix& matrix);

		float operator[](size_t n);
		Matrix& operator=(float* m);

	};
	Matrix operator*(const Matrix& matrix1, const Matrix& matrix2);

}