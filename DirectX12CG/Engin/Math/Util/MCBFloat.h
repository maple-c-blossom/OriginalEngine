#pragma once
#include <DirectXMath.h>


namespace MCB
{
	class Float3
	{
	public:
		float x_;
		float y_;
		float z_;

		Float3(const float& x, const float& y, const float& z);
		Float3();

		Float3& operator+=(const Float3& a);
		Float3& operator-=(const Float3& a);
	};

	class Float2
	{
	public:
		float x_;
		float y_;
	};

	class Float4
	{
	public:
		float x_ = 0;
		float y_ = 0;
		float z_ = 0;
		float w_ = 0;

		Float4();
		Float4(const float& x, const float& y, const float& z, const float& w);
	};

	MCB::Float3 operator-(MCB::Float3& a, MCB::Float3& b);
}

