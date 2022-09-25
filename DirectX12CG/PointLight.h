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
		inline void SetPLightPos(Float3 pos) { lightPos = pos; }
		inline void SetPLightColor(Float3 color) { lightColor = color; }
		inline void SetPLightAtten(Float3 atten) { lightAtten = atten; }
		inline void SetIsActive(bool flag) { isActive = flag; }
		inline Float3 GetPLightPos() { return lightPos; }
		inline Float3 GetPLightColor() { return lightColor; }
		inline Float3 GetPLightAtten() { return lightAtten; }
		inline bool IsActive() { return isActive; }

	private:
		Float3 lightPos = {0,0,0};
		Float3 lightColor = {1,1,1};
		Float3 lightAtten = {1.0f,1.0f,1.0f};//Œ¸Š‚Ì‹­‚³(‘å‚«‚¯‚ê‚Î‘å‚«‚¢‚Ú‚ÇŒ¸Š‚·‚é
		bool isActive = false;

	};
}

