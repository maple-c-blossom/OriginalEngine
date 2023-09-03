#include "MCBFloat.h"
#include "Vector3D.h"
using namespace MCB;

MCB::Float3::Float3(float x, float y, float z)
{
	x_ = x;
	y_ = y;
	z_ = z;
}

MCB::Float3::Float3()
{
	x_ = 0;
	y_ = 0;
	z_ = 0;
}

MCB::Float3& Float3::operator+=(const Float3& a)
{
	x_ += a.x_;
	y_ += a.y_;
	z_ += a.z_;
	return *this;
}

MCB::Float3& Float3::operator-=(const Float3& a)
{
	x_ -= a.x_;
	y_ -= a.y_;
	z_ -= a.z_;
	return *this;
}

Float3& MCB::Float3::operator=(const DirectX::XMVECTOR& a)
{
	x_ = a.m128_f32[0];
	y_ = a.m128_f32[1];
	z_ = a.m128_f32[2];
	return *this;
}

Float3& MCB::Float3::operator=(const Vector3D& a)
{
	x_ = a.vec_.x_;
	y_ = a.vec_.y_;
	z_ = a.vec_.z_;
	return *this;
}


MCB::Float3 MCB::operator-(MCB::Float3& a, MCB::Float3& b)
{
	Float3 temp = a;
	temp -= b;
	return temp;
}

MCB::Float4::Float4()
{
	x_ = 0;
	y_ = 0;
	z_ = 0;
	w_ = 0;
}

MCB::Float4::Float4(float x, float y, float z, float w)
{
	x_ = x;
	y_ = y;
	z_ = z;
	w_ = w;
}

