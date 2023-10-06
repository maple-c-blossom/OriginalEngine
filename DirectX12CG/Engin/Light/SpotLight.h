#pragma once
#include "Dx12.h"
#include "Pading.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
WarningIgnoreEnd
#include "Vector3D.h"
static const float PI = 3.14159265358979323846264338327950288f;
namespace MCB
{
	class SpotLight
	{
	private:
		 float ConvertRadius(float angle);//Util.h読み込めないのでここでつくる
	public:
		typedef struct SpotLConstBuffData
		{
			Vector3D ligntVec;
			float shininess;
			Float3 lightPos;
			float pad2;
			Float3 lightColor;
			float pad3;
			Float3 lightAtten;
			float pad4;
			Float2 lightFactorAngleCos;
			uint32_t active;
			float pad5;
		}SpotLConstBuffData;
		void SetSLightDir(const Vector3D& lightDir);
		void SetSLightColor(const Float3& lightcolor);
		void SetSLightPos(const Float3& pos);
		void SetSLightAtten(const Float3& atten);
		void SetIsActive(bool active);
		void SetSLightFactorAngle(const Float2& factorCos,bool convertFlag = true);
		void SetSLightShininess(float shininess);

		float GetSLightShininess();
		Vector3D GetSLightDir();
		Float3 GetSLightColor();
		Float3 GetSLightPos();
		Float3 GetSLightAtten();
		bool IsActive();
		Float2 GetSLightFactorAngle();
	private:
		Vector3D ligntDir_ = {1,0,0};
		Float3 lightPos_ = {0,0,0};
		Float3 lightColor_ = {1,1,1};
		Float3 lightAtten_ = {1.f,1.f,1.f};
		Float2 lightFactorAngleCos_ = {0.5f,0.2f};
		float shininess_ = 3.0f;
		Byte4 pad1;
		bool active_ = false;
		Byte3 pad2;
	};

}

