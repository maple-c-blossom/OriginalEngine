#pragma once
#include "Vector3D.h"
#include "IgnoreWarning.h"
#include "Pading.h"
WarningIgnoreBegin
#include <DirectXMath.h>
#include <array>
WarningIgnoreEnd
namespace MCB
{

	class MCBMatrix
	{
	public:
		float _11_; float _12_; float _13_; float _14_;
		float _21_; float _22_; float _23_; float _24_;
		float _31_; float _32_; float _33_; float _34_;
		float _41_; float _42_; float _43_; float _44_;
		
	public:
		MCBMatrix();
		MCBMatrix(DirectX::XMMATRIX mat);
		void MCBMatrixIdentity();

		static MCBMatrix MCBMatrixTransrate( float x,  float y,  float z);

		static MCBMatrix MCBMatrixTransrate(Vector3D pos);

		static MCBMatrix MCBMatrixScaling( float x,  float y,  float z);

		void ConvertMatrixArray(float** ArrayMat);

		void ConvertMatrixMCBMat(float** ArrayMat);

		void ConvertMatrixMCBMat(std::array<std::array<float, 4>, 4> ArrayMat);

		static std::array<std::array<float, 4>, 4> GetArrayMat(MCBMatrix mat);

		static MCBMatrix MCBMatrixRotaX( float angle);

		static MCBMatrix MCBMatrixRotaY( float angle);

		static MCBMatrix MCBMatrixRotaZ( float angle);

		static MCBMatrix MCBMatrixRota( double angle,const DirectX::XMFLOAT3& vector);

		void SetRotaMatrix(const Float3& angle);

		static MCBMatrix ReturnMatrixIdentity();

		MCBMatrix MCBMatrixLookAtLH(const Float3& eye, const Float3& target, const Float3& up);

		static MCBMatrix MatrixTranspose( MCBMatrix mat);

		static MCBMatrix MatrixInverse( MCBMatrix mat);

		//XMMatrixに変換する関数
		static DirectX::XMMATRIX MatrixConvertXMMatrix(const MCBMatrix& mat);

		static MCBMatrix MCBMatrixConvertXMMatrix(const DirectX::XMMATRIX& mat);

		static Vector3D Transform(const Vector3D& v, const MCBMatrix& m);

		static Vector3D GetTranslate(const MCBMatrix& m);



		//MCBMatrix operator*(MCBMatrix matrix);
		MCBMatrix operator*( float s);
		MCBMatrix operator/( float s);
		MCBMatrix operator+(const MCBMatrix& matrix);
		MCBMatrix operator-(const MCBMatrix& matrix);

		MCBMatrix& operator*=(const MCBMatrix& matrix);
		MCBMatrix& operator*=( float s);
		MCBMatrix& operator/=( float s);
		MCBMatrix& operator+=(const MCBMatrix& matrix);
		MCBMatrix& operator-=(const MCBMatrix& matrix);

		float operator[]( size_t n);

	};
		MCBMatrix operator*(const MCBMatrix& matrix1, const MCBMatrix& matrix2);

}

