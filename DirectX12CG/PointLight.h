#pragma once
#include "Vector3D.h"

namespace MCB
{
	class PointLight
	{
	public:
		struct PointLConstBuff
		{
			Float3 lightPos;
			float pad1;
			Float3 lightColor;
			float pad2;
			Float3 lightAtten;
			unsigned int active;
		};

		Float3 lightPos = {0,0,0};
		Float3 lightColor = {1,1,1};
		Float3 lightAtten = {1.0f,1.0f,1.0f};


	};
}

