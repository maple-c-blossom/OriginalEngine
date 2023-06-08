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
			uint32_t active;
		}PointLConstBuffData;
		inline void SetPLightPos(const Float3& pos) { lightPos = pos; }
		inline void SetPLightColor(const Float3& color) { lightColor = color; }
		inline void SetPLightAtten(const Float3& atten) { lightAtten = atten; }
		inline void SetPLightShininess(const float& shininess) { this->shininess = shininess; }
		inline void SetIsActive(const bool& flag) { isActive = flag; }
		inline Float3 GetPLightPos() { return lightPos; }
		inline Float3 GetPLightColor() { return lightColor; }
		inline Float3 GetPLightAtten() { return lightAtten; }
		inline float GetPLightShininess() { return shininess; }
		inline bool IsActive() { return isActive; }

	private:
		Float3 lightPos = {0,0,0};
		Float3 lightColor = {1,1,1};
		Float3 lightAtten = {1.0f,1.0f,1.0f};//減衰の強さ(大きければ大きいぼど減衰する
		float shininess = 3.0f;
		bool isActive = false;

	};
}

