#pragma once
#include <DirectXMath.h>
#include "Vector3D.h"
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
		void MCBMatrixIdentity();

		MCBMatrix MCBMatrixTransrate( float x,  float y,  float z);

		MCBMatrix MCBMatrixScaling( float x,  float y,  float z);

		void ConvertMatrixArray(float** ArrayMat);

		void ConvertMatrixMCBMat(float** ArrayMat);

		MCBMatrix MCBMatrixRotaX( float angle);

		MCBMatrix MCBMatrixRotaY( float angle);

		MCBMatrix MCBMatrixRotaZ( float angle);

		MCBMatrix MCBMatrixRota( double angle,const DirectX::XMFLOAT3& vector);

		void SetRotaMatrix(const Float3& angle);

		MCBMatrix ReturnMatrixIdentity();

		MCBMatrix MCBMatrixLookAtLH(const Float3& eye, const Float3& target, const Float3& up);

		MCBMatrix MatrixTranspose(const MCBMatrix& mat);

		MCBMatrix MatrixReverse(const MCBMatrix& mat);

		//XMMatrixÇ…ïœä∑Ç∑ÇÈä÷êî
		DirectX::XMMATRIX MatrixConvertXMMatrix(const MCBMatrix& mat);

		MCBMatrix MCBMatrixConvertXMMatrix(const DirectX::XMMATRIX& mat);

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

