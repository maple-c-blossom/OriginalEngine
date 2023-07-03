#include "MCBFloat.h"
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

