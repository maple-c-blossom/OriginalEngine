#include "Vector3D.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <cmath>
WarningIgnoreEnd


using namespace MCB;
MCB::Vector3D::Vector3D(const Float3& start,const Float3& end)
{
	Vector3D temp = end - start;
	vec_ = temp.vec_;
}

MCB::Vector3D::Vector3D(const Vector3D& start, const Vector3D& end)
{
	Vector3D temp = end - start;
	vec_ = temp.vec_;
}

MCB::Vector3D::Vector3D(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& end)
{
	Vector3D temp;
	temp.vec_.x_= end .m128_f32[0] - start.m128_f32[0];
	temp.vec_.y_= end .m128_f32[1] - start.m128_f32[1];
	temp.vec_.z_= end .m128_f32[2] - start.m128_f32[2];
	vec_ = temp.vec_;
}

MCB::Vector3D::Vector3D(const DirectX::XMFLOAT3& vec)
{
	vec_.x_ = vec.x;
	vec_.y_ = vec.y;
	vec_.z_ = vec.z;
}


MCB::Vector3D::Vector3D()
{
	vec_.x_ = 0;
	vec_.y_ = 0;
	vec_.z_ = 0;
}
MCB::Vector3D::Vector3D(const Float3& vec)
{
	vec_ = vec;
}
MCB::Vector3D::Vector3D(const DirectX::XMVECTOR vec)
{
	vec_.x_ = vec.m128_f32[0];
	vec_.y_ = vec.m128_f32[1];
	vec_.z_ = vec.m128_f32[2];
}
MCB::Vector3D::Vector3D( float x,  float y,  float z)
{
	vec_.x_ = x;
	vec_.y_ = y;
	vec_.z_ = z;
}
Vector3D MCB::Vector3D::Vector3Substruct(const Float3& start, const Float3& end)
{
	 Vector3D temp(start,end);
	 return temp;
}

Vector3D MCB::Vector3D::Vector3Substruct(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& end)
{
	Vector3D temp(start, end);
	return temp;
}
float MCB::Vector3D::V3Len() const
{
	return sqrtf(((vec_.x_ * vec_.x_) + (vec_.y_ * vec_.y_) + (vec_.z_ * vec_.z_)));
}

void MCB::Vector3D::V3Norm()
{
	float VecLen = V3Len();
	if (VecLen == 0.0f)
	{
		//assert(0);
		return;
	}

	vec_.x_ = vec_.x_ / VecLen;
	vec_.y_ = vec_.y_ / VecLen;
	vec_.z_ = vec_.z_ / VecLen;

}

Vector3D MCB::Vector3D::GetV3Norm()
{
	Vector3D ans = *this;
	ans.V3Norm();
	return ans;
}

float MCB::Vector3D::GetV3Dot(const Vector3D& vector)
{
	return (vec_.x_ * vector.vec_.x_) + (vec_.y_ * vector.vec_.y_) + (vec_.z_ * vector.vec_.z_);
}

Vector3D MCB::Vector3D::GetV3Cross(const Vector3D& vector)
{
	Vector3D temp;
	temp.vec_.x_ = vec_.y_ * vector.vec_.z_ - vec_.z_ * vector.vec_.y_;
	temp.vec_.y_ = vec_.z_ * vector.vec_.x_ - vec_.x_ * vector.vec_.z_;
	temp.vec_.z_ = vec_.x_ * vector.vec_.y_ - vec_.y_ * vector.vec_.x_;
	return temp;
}

Vector3D MCB::Vector3D::GetV3Cross(const Vector3D& avector, const Vector3D& bvector)
{
	Vector3D temp;
	temp.vec_.x_ = avector.vec_.y_ * bvector.vec_.z_ - avector.vec_.z_ * bvector.vec_.y_;
	temp.vec_.y_ = avector.vec_.z_ * bvector.vec_.x_ - avector.vec_.x_ * bvector.vec_.z_;
	temp.vec_.z_ = avector.vec_.x_ * bvector.vec_.y_ - avector.vec_.y_ * bvector.vec_.x_;
	return temp;
}

DirectX::XMVECTOR MCB::Vector3D::ConvertXMVEC()
{
	DirectX::XMVECTOR temp;
	temp.m128_f32[0] = vec_.x_;
	temp.m128_f32[1] = vec_.y_;
	temp.m128_f32[2] = vec_.z_;
	temp.m128_f32[3] = 0;
	return temp;
}


DirectX::XMFLOAT3 MCB::Vector3D::ConvertXMFloat3()
{
	DirectX::XMFLOAT3 temp;
	temp.x = vec_.x_;
	temp.y = vec_.y_;
	temp.z = vec_.z_;
	return temp;
}

Vector3D MCB::Vector3D::GetUpVec(Vector3D RightVec, Vector3D frontVec)
{
	Vector3D ans;
	RightVec.V3Norm();
	frontVec.V3Norm();
	ans = ans.GetV3Cross(frontVec, RightVec);
	ans.V3Norm();

	return ans;
}


Vector3D MCB::Vector3D::GetRightVec( Vector3D frontVec,  Vector3D UpVec)
{
	Vector3D ans;
	frontVec.V3Norm();
	UpVec.V3Norm();
	ans = ans.GetV3Cross(UpVec, frontVec);
	ans.V3Norm();

	return ans;
}

Vector3D MCB::Vector3D::GetV3Normal(Vector3D v0, Vector3D v1, Vector3D v2)
{
	Vector3D ans;
	Vector3D vv1, vv2;
	vv1 = vv1.Vector3Substruct(v0.vec_, v1.vec_);
	vv2 = vv2.Vector3Substruct(v1.vec_, v2.vec_);
	vv1.V3Norm();
	vv2.V3Norm();
	ans = vv1.GetV3Cross(vv2);
	ans.V3Norm();
	return ans;
}



float MCB::Vector3D::GetInnerProduct(Vector3D vector)
{
	Vector3D temp;
	temp.vec_ = vec_;
	temp.V3Norm();
	vector.V3Norm();
	return acos(temp.GetV3Dot(vector));
}

Vector3D& MCB::Vector3D::operator+=(const Vector3D& Vec)
{
	vec_.x_ += Vec.vec_.x_;
	vec_.y_ += Vec.vec_.y_;
	vec_.z_ += Vec.vec_.z_;
	return *this;
}

Vector3D& MCB::Vector3D::operator-=(const Vector3D& Vec)
{
	vec_.x_ -= Vec.vec_.x_;
	vec_.y_ -= Vec.vec_.y_;
	vec_.z_ -= Vec.vec_.z_;
	return *this;
}

Vector3D& MCB::Vector3D::operator*=( float k)
{
	vec_.x_ *= k;
	vec_.y_ *= k;
	vec_.z_ *= k;
	return *this;
}

Vector3D& MCB::Vector3D::operator=(const Float3& a)
{
	vec_.x_ = a.x_;
	vec_.y_ = a.y_;
	vec_.z_ = a.z_;
	return *this;
}

Vector3D& MCB::Vector3D::operator=(const DirectX::XMVECTOR& a)
{
	vec_.x_ = a.m128_f32[0];
	vec_.y_ = a.m128_f32[1];
	vec_.z_ = a.m128_f32[2];
	return *this;
}

Vector3D& MCB::Vector3D::operator=(const DirectX::XMFLOAT3& a)
{
	vec_.x_ = a.x;
	vec_.y_ = a.y;
	vec_.z_ = a.z;
	return *this;
}

Vector3D MCB::operator+(const Vector3D& vecA, const Vector3D& vecB)
{
	Vector3D temp;
	temp = vecA;
	temp += vecB;
	return temp;
}

bool MCB::operator!=(const Vector3D& vecA, const DirectX::XMFLOAT3& vecB)
{
	return (vecA.vec_.x_ != vecB.x || vecA.vec_.y_ != vecB.y || vecA.vec_.z_ != vecB.z);
}

bool MCB::operator!=(const Vector3D& vecA,const Vector3D& vecB)
{
	return ( vecA.vec_.x_ != vecB.vec_.x_ || vecA.vec_.y_ != vecB.vec_.x_ || vecA.vec_.z_ != vecB.vec_.x_ );
}

Vector3D MCB::operator-(const Vector3D& vecA, const Vector3D& vecB)
{
	Vector3D temp;
	temp = vecA;
	temp -= vecB;
	return temp;
}

Vector3D MCB::operator*(const Vector3D& vecA,  float k)
{
	Vector3D temp;
	temp = vecA;
	temp *= k;
	return temp;
}

Vector3D MCB::operator*( float k, const Vector3D& vecA)
{
	Vector3D temp;
	temp = vecA;
	temp *= k;
	return temp;
}

Vector3D MCB::operator/(const Vector3D& vecA,  float k)
{
	Vector3D temp;
	temp.vec_.x_ = vecA.vec_.x_ / k;
	temp.vec_.y_ = vecA.vec_.y_ / k;
	temp.vec_.z_ = vecA.vec_.z_ / k;
	return temp;
}

Vector3D MCB::operator-(const Vector3D& vecA)
{
	Vector3D temp;
	temp.vec_.x_ = vecA.vec_.x_ * -1;
	temp.vec_.y_ = vecA.vec_.y_ * -1;
	temp.vec_.z_ = vecA.vec_.z_ * -1;
	return temp;
}


