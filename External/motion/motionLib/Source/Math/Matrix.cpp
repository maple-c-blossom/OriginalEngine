#include "Matrix.h"
#include <stdexcept>
#include <string>

#define PI 3.141592654f

using namespace MCB;

MCB::Matrix::Matrix()
{
	_11_ = 1.0f; _12_ = 0.0f; _13_ = 0.0f; _14_ = 0.0f;
	_21_ = 0.0f; _22_ = 1.0f; _23_ = 0.0f; _24_ = 0.0f;
	_31_ = 0.0f; _32_ = 0.0f; _33_ = 1.0f; _34_ = 0.0f;
	_41_ = 0.0f; _42_ = 0.0f; _43_ = 0.0f; _44_ = 1.0f;
}

void MCB::Matrix::MatrixIdentity()
{
	_11_ = 1.0f; _12_ = 0.0f; _13_ = 0.0f; _14_ = 0.0f;
	_21_ = 0.0f; _22_ = 1.0f; _23_ = 0.0f; _24_ = 0.0f;
	_31_ = 0.0f; _32_ = 0.0f; _33_ = 1.0f; _34_ = 0.0f;
	_41_ = 0.0f; _42_ = 0.0f; _43_ = 0.0f; _44_ = 1.0f;
}

MCB::Matrix MCB::Matrix::MatrixTranslate(float x, float y, float z)
{
	Matrix temp;
	temp.MatrixIdentity();

	temp._41_ = x;
	temp._42_ = y;
	temp._43_ = z;
	temp._44_ = 1;

	return temp;
}

Matrix MCB::Matrix::MatrixTranslate(Vector3 pos)
{
	Matrix temp;
	temp.MatrixIdentity();

	temp._41_ = pos.x;
	temp._42_ = pos.y;
	temp._43_ = pos.z;
	temp._44_ = 1;

	return temp;
}

MCB::Matrix MCB::Matrix::MatrixScaling(float x, float y, float z)
{

	Matrix temp;
	temp.MatrixIdentity();

	temp._11_ = x;
	temp._22_ = y;
	temp._33_ = z;
	temp._44_ = 1;

	return temp;

}

Matrix MCB::Matrix::MatrixScaling(Vector3 scale)
{
	Matrix temp;
	temp.MatrixIdentity();

	temp._11_ = scale.x;
	temp._22_ = scale.y;
	temp._33_ = scale.z;
	temp._44_ = 1;

	return temp;
}

void MCB::Matrix::ConvertMatrixArray(float** ArrayMat)
{
	ArrayMat[0][0] = _11_, ArrayMat[0][1] = _12_, ArrayMat[0][2] = _13_, ArrayMat[0][3] = _14_;
	ArrayMat[1][0] = _21_, ArrayMat[1][1] = _22_, ArrayMat[1][2] = _23_, ArrayMat[1][3] = _24_;
	ArrayMat[2][0] = _31_, ArrayMat[2][1] = _32_, ArrayMat[2][2] = _33_, ArrayMat[2][3] = _34_;
	ArrayMat[3][0] = _41_, ArrayMat[3][1] = _42_, ArrayMat[3][2] = _43_, ArrayMat[3][3] = _44_;
}

void MCB::Matrix::ConvertMatrixMCBMat(float** ArrayMat)
{
	_11_ = ArrayMat[0][0], _12_ = ArrayMat[0][1], _13_ = ArrayMat[0][2], _14_ = ArrayMat[0][3];
	_21_ = ArrayMat[1][0], _22_ = ArrayMat[1][1], _23_ = ArrayMat[1][2], _24_ = ArrayMat[1][3];
	_31_ = ArrayMat[2][0], _32_ = ArrayMat[2][1], _33_ = ArrayMat[2][2], _34_ = ArrayMat[2][3];
	_41_ = ArrayMat[3][0], _42_ = ArrayMat[3][1], _43_ = ArrayMat[3][2], _44_ = ArrayMat[3][3];
}

void MCB::Matrix::ConvertMatrixMCBMat(std::array<std::array<float, 4>, 4> ArrayMat)
{
	_11_ = ArrayMat[0][0], _12_ = ArrayMat[0][1], _13_ = ArrayMat[0][2], _14_ = ArrayMat[0][3];
	_21_ = ArrayMat[1][0], _22_ = ArrayMat[1][1], _23_ = ArrayMat[1][2], _24_ = ArrayMat[1][3];
	_31_ = ArrayMat[2][0], _32_ = ArrayMat[2][1], _33_ = ArrayMat[2][2], _34_ = ArrayMat[2][3];
	_41_ = ArrayMat[3][0], _42_ = ArrayMat[3][1], _43_ = ArrayMat[3][2], _44_ = ArrayMat[3][3];
}

std::array<std::array<float, 4>, 4> MCB::Matrix::GetArrayMat(Matrix mat)
{
	std::array<std::array<float, 4>, 4> ans;
	ans[0][0] = mat._11_, ans[1][0] = mat._21_, ans[2][0] = mat._31_, ans[3][0] = mat._41_;
	ans[0][1] = mat._12_, ans[1][1] = mat._22_, ans[2][1] = mat._32_, ans[3][1] = mat._42_;
	ans[0][2] = mat._13_, ans[1][2] = mat._23_, ans[2][2] = mat._33_, ans[3][2] = mat._43_;
	ans[0][3] = mat._14_, ans[1][3] = mat._24_, ans[2][3] = mat._34_, ans[3][3] = mat._44_;
	return ans;
}

MCB::Matrix MCB::Matrix::MatrixRotaX(float angle)
{
	Matrix matrix;

	matrix.MatrixIdentity();

	matrix._22_ = cosf(angle);
	matrix._23_ = sinf(angle);

	matrix._32_ = -sinf(angle);
	matrix._33_ = cosf(angle);

	return matrix;
}

MCB::Matrix MCB::Matrix::MatrixRotaY(float angle)
{
	Matrix matrix;

	matrix.MatrixIdentity();
	matrix._11_ = (float)cosf(angle);
	matrix._31_ = (float)-sin(angle);

	matrix._13_ = (float)sinf(angle);
	matrix._33_ = (float)cosf(angle);

	return matrix;
}

MCB::Matrix MCB::Matrix::MatrixRotaZ(float angle)
{
	Matrix matrix;
	matrix.MatrixIdentity();

	matrix._11_ = (float)cos(angle);
	matrix._12_ = (float)sin(angle);

	matrix._21_ = (float)-sin(angle);
	matrix._22_ = (float)cos(angle);

	return matrix;
}


Matrix MCB::Matrix::GetQuaternionRotaMat(const Quaternion& rotaQ)
{
	Matrix mat;
	mat.MatrixIdentity();
	mat._11_ = 1 - (2 * (rotaQ.y * rotaQ.y)) - (2 * (rotaQ.z * rotaQ.z));
	mat._12_ = (2 * rotaQ.x * rotaQ.y) + (2 * rotaQ.w * rotaQ.z);
	mat._13_ = (2 * rotaQ.x * rotaQ.z) - (2 * rotaQ.w *rotaQ. y);
	mat._14_ = 0;

	mat._21_ = (2 * rotaQ.x * rotaQ.y) - (2 * rotaQ.w * rotaQ.z);
	mat._22_ = 1 - (2 * (rotaQ.x * rotaQ.x)) - (2 * (rotaQ.z * rotaQ.z));
	mat._23_ = (2 * rotaQ.y * rotaQ.z) + (2 * rotaQ.w * rotaQ.x);
	mat._24_ = 0;

	mat._31_ = (2 * rotaQ.x * rotaQ.z) + (2 * rotaQ.w * rotaQ.y);
	mat._32_ = (2 * rotaQ.y * rotaQ.z) - (2 * rotaQ.w * rotaQ.x);
	mat._33_ = 1 - (2 * (rotaQ.x * rotaQ.x)) - (2 * (rotaQ.y * rotaQ.y));
	mat._34_ = 0;

	mat._41_ = 0;
	mat._42_ = 0;
	mat._43_ = 0;
	mat._44_ = 1;


	return mat;
}


MCB::Matrix MCB::Matrix::ReturnMatrixIdentity()
{
	Matrix ans;
	ans._11_ = 1.0f; ans._12_ = 0.0f; ans._13_ = 0.0f; ans._14_ = 0.0f;
	ans._21_ = 0.0f; ans._22_ = 1.0f; ans._23_ = 0.0f; ans._24_ = 0.0f;
	ans._31_ = 0.0f; ans._32_ = 0.0f; ans._33_ = 1.0f; ans._34_ = 0.0f;
	ans._41_ = 0.0f; ans._42_ = 0.0f; ans._43_ = 0.0f; ans._44_ = 1.0f;
	return ans;
}

Matrix MCB::Matrix::MatrixTranspose(Matrix mat)
{
	std::array<std::array<float, 4>, 4> ans;
	std::array<std::array<float, 4>, 4> arrayMat = mat.GetArrayMat(mat);
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			ans[i][j] = arrayMat[j][i];
		}
	}
	Matrix ret;
	ret.ConvertMatrixMCBMat(ans);
	return ret;
}

Matrix MCB::Matrix::MatrixInverse(Matrix mat)
{

	Matrix result;
	std::array<std::array<float, 4>, 4> tempResult;
	std::array<std::array<float, 8>, 4> temp = {};
	std::array<std::array<float, 4>, 4> num;
	num = mat.GetArrayMat(mat);
	float a;

	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			temp[i][j] = num[i][j];

			if (i == j)temp[i][4 + j] = 1;
		}
	}

	for (uint32_t k = 0; k < 4; k++) {
		a = 1 / temp[k][k];

		for (uint32_t j = 0; j < 8; j++) {
			temp[k][j] *= a;
		}

		for (uint32_t i = 0; i < 4; i++) {
			if (i == k) {
				continue;
			}

			a = -temp[i][k];

			for (uint32_t j = 0; j < 8; j++) {
				temp[i][j] += temp[k][j] * a;
			}
		}
	}

	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			tempResult[i][j] = temp[i][4 + j];
		}
	}
	result.ConvertMatrixMCBMat(tempResult);
	return result;
}

//MCB::Matrix MCB::Matrix::operator*(Matrix matrix)
//{
//	Matrix ans;
//
//#pragma region １行目
//	ans._11 = (_11 * matrix._11) + (_12 * matrix._21) + (_13 * matrix._31) + (_14 * matrix._41);
//	ans._12 = (_11 * matrix._12) + (_12 * matrix._22) + (_13 * matrix._32) + (_14 * matrix._42);
//	ans._13 = (_11 * matrix._13) + (_12 * matrix._23) + (_13 * matrix._33) + (_14 * matrix._43);
//	ans._14 = (_11 * matrix._14) + (_12 * matrix._24) + (_13 * matrix._34) + (_14 * matrix._44);
//#pragma endregion 1行目
//
//#pragma region 2行目
//	ans._21 = (_21 * matrix._11) + (_22 * matrix._21) + (_23 * matrix._31) + (_24 * matrix._41);
//	ans._22 = (_21 * matrix._12) + (_22 * matrix._22) + (_23 * matrix._32) + (_24 * matrix._42);
//	ans._23 = (_21 * matrix._13) + (_22 * matrix._23) + (_23 * matrix._33) + (_24 * matrix._43);
//	ans._24 = (_21 * matrix._14) + (_22 * matrix._24) + (_23 * matrix._34) + (_24 * matrix._44);
//#pragma endregion 2行目
//
//#pragma region 3行目
//	ans._31 = (_31 * matrix._11) + (_32 * matrix._21) + (_33 * matrix._31) + (_34 * matrix._41);
//	ans._32 = (_31 * matrix._12) + (_32 * matrix._22) + (_33 * matrix._32) + (_34 * matrix._42);
//	ans._33 = (_31 * matrix._13) + (_32 * matrix._23) + (_33 * matrix._33) + (_34 * matrix._43);
//	ans._34 = (_31 * matrix._14) + (_32 * matrix._24) + (_33 * matrix._34) + (_34 * matrix._44);
//#pragma endregion 3行目
//
//#pragma region 4行目
//	ans._41 = (_41 * matrix._11) + (_42 * matrix._21) + (_43 * matrix._31) + (_44 * matrix._41);
//	ans._42 = (_41 * matrix._12) + (_42 * matrix._22) + (_43 * matrix._32) + (_44 * matrix._42);
//	ans._43 = (_41 * matrix._13) + (_42 * matrix._23) + (_43 * matrix._33) + (_44 * matrix._43);
//	ans._44 = (_41 * matrix._14) + (_42 * matrix._24) + (_43 * matrix._34) + (_44 * matrix._44);
//#pragma endregion 4行目
//
//	return ans;
//}

Vector3 MCB::Matrix::Transform(const Vector3& v, const Matrix& m)
{
	float w = v.x * m._14_ + v.y * m._24_ + v.z * m._34_ + m._44_;

	Vector3 result
	{
		(v.x * m._11_ + v.y * m._21_ + v.z * m._31_ + m._41_) / w,
		(v.x * m._12_ + v.y * m._22_ + v.z * m._32_ + m._42_) / w,
		(v.x * m._13_ + v.y * m._23_ + v.z * m._33_ + m._43_) / w
	};
	return result;
}

Vector3 MCB::Matrix::GetTranslate(const Matrix& m)
{
	Vector3 pos;
	pos.x = m._41_;
	pos.y = m._42_;
	pos.z = m._43_;
	return pos;
}

Vector3 MCB::Matrix::GetRotationToEuler()
{
	Vector3 rot;
	if (_13_ == 1.0f)
	{
		rot.x = static_cast<float>(atan2(_23_, _33_));
		rot.y = PI * 0.5f;
		rot.z = 0;
	}
	else if (13 == -1.0f)
	{
		rot.x = static_cast<float>(atan2(_23_, _33_));
		rot.y = -PI * 0.5f;
		rot.z = 0;
	}
	else
	{
		rot.x = static_cast<float>(atan2(_23_, _33_));
		rot.y = -static_cast<float>(asin(_13_));
		rot.z = static_cast<float>(atan2(_12_, _11_));///-90~90
	}

	return rot;
}

MCB::Matrix MCB::Matrix::operator*(float s)
{
	Matrix ans;
	ans._11_ = _11_ * s;
	ans._12_ = _12_ * s;
	ans._13_ = _13_ * s;
	ans._14_ = _14_ * s;

	ans._21_ = _21_ * s;
	ans._22_ = _22_ * s;
	ans._23_ = _23_ * s;
	ans._24_ = _24_ * s;

	ans._31_ = _31_ * s;
	ans._32_ = _32_ * s;
	ans._33_ = _33_ * s;
	ans._34_ = _34_ * s;

	ans._41_ = _41_ * s;
	ans._42_ = _42_ * s;
	ans._43_ = _43_ * s;
	ans._44_ = _44_ * s;

	return ans;
}

MCB::Matrix MCB::Matrix::operator/(float s)
{
	Matrix ans;
	ans._11_ = _11_ / s;
	ans._12_ = _12_ / s;
	ans._13_ = _13_ / s;
	ans._14_ = _14_ / s;

	ans._21_ = _21_ / s;
	ans._22_ = _22_ / s;
	ans._23_ = _23_ / s;
	ans._24_ = _24_ / s;

	ans._31_ = _31_ / s;
	ans._32_ = _32_ / s;
	ans._33_ = _33_ / s;
	ans._34_ = _34_ / s;

	ans._41_ = _41_ / s;
	ans._42_ = _42_ / s;
	ans._43_ = _43_ / s;
	ans._44_ = _44_ / s;

	return ans;
}

MCB::Matrix MCB::Matrix::operator+(const Matrix& matrix)
{
	Matrix ans;
	ans._11_ = _11_ + matrix._11_;
	ans._12_ = _12_ + matrix._12_;
	ans._13_ = _13_ + matrix._13_;
	ans._14_ = _14_ + matrix._14_;

	ans._21_ = _21_ + matrix._21_;
	ans._22_ = _22_ + matrix._22_;
	ans._23_ = _23_ + matrix._23_;
	ans._24_ = _24_ + matrix._24_;

	ans._31_ = _31_ + matrix._31_;
	ans._32_ = _32_ + matrix._32_;
	ans._33_ = _33_ + matrix._33_;
	ans._34_ = _34_ + matrix._34_;

	ans._41_ = _41_ + matrix._41_;
	ans._42_ = _42_ + matrix._42_;
	ans._43_ = _43_ + matrix._43_;
	ans._44_ = _44_ + matrix._44_;


	return ans;
}

MCB::Matrix MCB::Matrix::operator-(const Matrix& matrix)
{
	Matrix ans;
	ans._11_ = _11_ - matrix._11_;
	ans._12_ = _12_ - matrix._12_;
	ans._13_ = _13_ - matrix._13_;
	ans._14_ = _14_ - matrix._14_;

	ans._21_ = _21_ - matrix._21_;
	ans._22_ = _22_ - matrix._22_;
	ans._23_ = _23_ - matrix._23_;
	ans._24_ = _24_ - matrix._24_;

	ans._31_ = _31_ - matrix._31_;
	ans._32_ = _32_ - matrix._32_;
	ans._33_ = _33_ - matrix._33_;
	ans._34_ = _34_ - matrix._34_;

	ans._41_ = _41_ - matrix._41_;
	ans._42_ = _42_ - matrix._42_;
	ans._43_ = _43_ - matrix._43_;
	ans._44_ = _44_ - matrix._44_;


	return ans;
}

MCB::Matrix& MCB::Matrix::operator*=(const Matrix& matrix)
{
	*this = *this * matrix;
	return(*this);
}

MCB::Matrix& MCB::Matrix::operator*=(float s)
{
	*this = *this * s;
	return(*this);
}

MCB::Matrix& MCB::Matrix::operator/=(float s)
{
	*this = *this / s;
	return(*this);
}

MCB::Matrix& MCB::Matrix::operator+=(const Matrix& matrix)
{
	*this = *this + matrix;
	return(*this);
}

MCB::Matrix& MCB::Matrix::operator-=(const Matrix& matrix)
{
	*this = *this - matrix;
	return *this;
}

float MCB::Matrix::operator[](size_t n)
{
	switch (n)
	{
	case 0:
		return _11_;
		break;

	case 1:
		return _12_;
		break;

	case 2:
		return _13_;
		break;

	case 3:
		return _14_;
		break;

	case 4:
		return _21_;
		break;

	case 5:
		return _22_;
		break;

	case 6:
		return _23_;
		break;

	case 7:
		return _24_;
		break;

	case 8:
		return _31_;
		break;

	case 9:
		return _32_;
		break;

	case 10:
		return _33_;
		break;

	case 11:
		return _34_;
		break;

	case 12:
		return _41_;
		break;

	case 13:
		return _42_;
		break;

	case 14:
		return _43_;
		break;

	case 15:
		return _44_;
		break;
	default:
		throw std::out_of_range(std::string("無効な値が入力されました。0～15までの範囲の整数を入力してください。"));
		break;
	}


}

Matrix& MCB::Matrix::operator=(float* m)
{
	for (int i = 0; i < 16; i++)
	{
		matOneArray[i] = m[i];
	}
	return *this;
}

Matrix MCB::operator*(const Matrix& matrix1, const Matrix& matrix2)
{
	Matrix ans;

#pragma region １行目
	ans._11_ = (matrix1._11_ * matrix2._11_) + (matrix1._12_ * matrix2._21_) + (matrix1._13_ * matrix2._31_) + (matrix1._14_ * matrix2._41_);
	ans._12_ = (matrix1._11_ * matrix2._12_) + (matrix1._12_ * matrix2._22_) + (matrix1._13_ * matrix2._32_) + (matrix1._14_ * matrix2._42_);
	ans._13_ = (matrix1._11_ * matrix2._13_) + (matrix1._12_ * matrix2._23_) + (matrix1._13_ * matrix2._33_) + (matrix1._14_ * matrix2._43_);
	ans._14_ = (matrix1._11_ * matrix2._14_) + (matrix1._12_ * matrix2._24_) + (matrix1._13_ * matrix2._34_) + (matrix1._14_ * matrix2._44_);
#pragma endregion 1行目

#pragma region 2行目
	ans._21_ = (matrix1._21_ * matrix2._11_) + (matrix1._22_ * matrix2._21_) + (matrix1._23_ * matrix2._31_) + (matrix1._24_ * matrix2._41_);
	ans._22_ = (matrix1._21_ * matrix2._12_) + (matrix1._22_ * matrix2._22_) + (matrix1._23_ * matrix2._32_) + (matrix1._24_ * matrix2._42_);
	ans._23_ = (matrix1._21_ * matrix2._13_) + (matrix1._22_ * matrix2._23_) + (matrix1._23_ * matrix2._33_) + (matrix1._24_ * matrix2._43_);
	ans._24_ = (matrix1._21_ * matrix2._14_) + (matrix1._22_ * matrix2._24_) + (matrix1._23_ * matrix2._34_) + (matrix1._24_ * matrix2._44_);
#pragma endregion 2行目

#pragma region 3行目
	ans._31_ = (matrix1._31_ * matrix2._11_) + (matrix1._32_ * matrix2._21_) + (matrix1._33_ * matrix2._31_) + (matrix1._34_ * matrix2._41_);
	ans._32_ = (matrix1._31_ * matrix2._12_) + (matrix1._32_ * matrix2._22_) + (matrix1._33_ * matrix2._32_) + (matrix1._34_ * matrix2._42_);
	ans._33_ = (matrix1._31_ * matrix2._13_) + (matrix1._32_ * matrix2._23_) + (matrix1._33_ * matrix2._33_) + (matrix1._34_ * matrix2._43_);
	ans._34_ = (matrix1._31_ * matrix2._14_) + (matrix1._32_ * matrix2._24_) + (matrix1._33_ * matrix2._34_) + (matrix1._34_ * matrix2._44_);
#pragma endregion 3行目

#pragma region 4行目
	ans._41_ = (matrix1._41_ * matrix2._11_) + (matrix1._42_ * matrix2._21_) + (matrix1._43_ * matrix2._31_) + (matrix1._44_ * matrix2._41_);
	ans._42_ = (matrix1._41_ * matrix2._12_) + (matrix1._42_ * matrix2._22_) + (matrix1._43_ * matrix2._32_) + (matrix1._44_ * matrix2._42_);
	ans._43_ = (matrix1._41_ * matrix2._13_) + (matrix1._42_ * matrix2._23_) + (matrix1._43_ * matrix2._33_) + (matrix1._44_ * matrix2._43_);
	ans._44_ = (matrix1._41_ * matrix2._14_) + (matrix1._42_ * matrix2._24_) + (matrix1._43_ * matrix2._34_) + (matrix1._44_ * matrix2._44_);
#pragma endregion 4行目

	return ans;
}
