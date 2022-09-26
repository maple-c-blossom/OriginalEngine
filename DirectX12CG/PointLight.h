#pragma once
#include "Vector3D.h"

namespace MCB
{
	class PointLight
	{
	public:
		typedef struct PointLConstBuffData
		{
			Float3 lightPos;
			float shininess;
			Float3 lightColor;
			float pad2;
			Float3 lightAtten;
			unsigned int active;
		};
		inline void SetPLightPos(Float3 pos) { lightPos = pos; }
		inline void SetPLightColor(Float3 color) { lightColor = color; }
		inline void SetPLightAtten(Float3 atten) { lightAtten = atten; }
		inline void SetPLightShininess(float shininess) { this->shininess = shininess; }
		inline void SetIsActive(bool flag) { isActive = flag; }
		inline Float3 GetPLightPos() { return lightPos; }
		inline Float3 GetPLightColor() { return lightColor; }
		inline Float3 GetPLightAtten() { return lightAtten; }
		inline float GetPLightShininess() { return shininess; }
		inline bool IsActive() { return isActive; }

	private:
		Float3 lightPos = {0,0,0};
		Float3 lightColor = {1,1,1};
		Float3 lightAtten = {1.0f,1.0f,1.0f};//Œ¸Š‚Ì‹­‚³(‘å‚«‚¯‚ê‚Î‘å‚«‚¢‚Ú‚ÇŒ¸Š‚·‚é
		float shininess = 3.0f;
		bool isActive = false;

	};
}

