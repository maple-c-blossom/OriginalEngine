#pragma once
#include "Vector3D.h"
#include "Pading.h"
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
		void SetPLightPos(const Float3& pos);
		void SetPLightColor(const Float3& color);
		void SetPLightAtten(const Float3& atten);
		void SetPLightShininess(float shininess);
		void SetIsActive(bool flag);
		Float3 GetPLightPos();
		Float3 GetPLightColor();
		Float3 GetPLightAtten();
		float GetPLightShininess();
		bool IsActive();

	private:
		Float3 lightPos_ = {0,0,0};
		Float3 lightColor_ = {1,1,1};
		Float3 lightAtten_ = {1.0f,1.0f,1.0f};//減衰の強さ(大きければ大きいぼど減衰する
		float shininess_ = 3.0f;
		bool isActive_ = false;
		Byte3 pad1;
	};
}

