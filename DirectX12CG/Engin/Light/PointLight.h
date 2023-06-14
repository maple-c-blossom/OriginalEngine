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
		inline void SetPLightPos(const Float3& pos) { lightPos_ = pos; }
		inline void SetPLightColor(const Float3& color) { lightColor_ = color; }
		inline void SetPLightAtten(const Float3& atten) { lightAtten_ = atten; }
		inline void SetPLightShininess( float shininess) { shininess_ = shininess; }
		inline void SetIsActive( bool flag) { isActive_ = flag; }
		inline Float3 GetPLightPos() { return lightPos_; }
		inline Float3 GetPLightColor() { return lightColor_; }
		inline Float3 GetPLightAtten() { return lightAtten_; }
		inline float GetPLightShininess() { return shininess_; }
		inline bool IsActive() { return isActive_; }

	private:
		Float3 lightPos_ = {0,0,0};
		Float3 lightColor_ = {1,1,1};
		Float3 lightAtten_ = {1.0f,1.0f,1.0f};//Œ¸Š‚Ì‹­‚³(‘å‚«‚¯‚ê‚Î‘å‚«‚¢‚Ú‚ÇŒ¸Š‚·‚é
		float shininess_ = 3.0f;
		bool isActive_ = false;

	};
}

