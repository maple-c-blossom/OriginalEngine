#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <DirectXMath.h>
WarningIgnoreEnd

namespace MCB
{
	class Vector3D;
	class Float3
	{
	public:
#pragma warning(push)
#pragma warning(disable:4201)
		union 
		{
			float arrayFloat[ 3 ];

			struct 
			{
				float x_;
				float y_;
				float z_;
			};
		};
#pragma warning(pop)

		Float3( float x,  float y,  float z);
		Float3();

		Float3& operator+=(const Float3& a);
		Float3& operator-=(const Float3& a);
		Float3& operator=(const DirectX::XMVECTOR& a);
		Float3& operator=(const Vector3D& a);
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
		Float4( float x,  float y,  float z,  float w);
	};

	MCB::Float3 operator-(MCB::Float3& a, MCB::Float3& b);
}

