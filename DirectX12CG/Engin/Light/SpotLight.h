#pragma once
#include <wrl.h>
#include <Windows.h>
#include "Dx12.h"
#include <d3d12.h>
#include <d3dx12.h>
#include "Vector3D.h"
#define PI 3.14159265358979323846264338327950288f
namespace MCB
{
	class SpotLight
	{
	private:
		inline float ConvertRadius(float angle)//Util.hì«Ç›çûÇﬂÇ»Ç¢ÇÃÇ≈Ç±Ç±Ç≈Ç¬Ç≠ÇÈ
		{
			return angle * PI / 180.0f;
		};
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
		inline void SetSLightDir(const Vector3D& lightDir){ ligntDir_ = lightDir; }
		inline void SetSLightColor(const Float3& lightcolor) { lightColor_ = lightcolor; }
		inline void SetSLightPos(const Float3& pos) { lightPos_ = pos; }
		inline void SetSLightAtten(const Float3& atten) { lightAtten_ = atten; }
		inline void SetIsActive( bool active) { active_ = active; }
		inline void SetSLightFactorAngle(const Float2& factorCos, bool convertFlag = true)
		{
			if (convertFlag)
			{
				lightFactorAngleCos_.x_ = ConvertRadius(factorCos.x_);
				lightFactorAngleCos_.y_ = ConvertRadius(factorCos.y_);
			}
			else
			{
				lightFactorAngleCos_ = factorCos;
			}
		}
		inline void SetSLightShininess( float shininess) { shininess_ = shininess; }

		inline float GetSLightShininess() { return shininess_; }
		inline Vector3D GetSLightDir() { return ligntDir_; }
		inline Float3 GetSLightColor() { return lightColor_; }
		inline Float3 GetSLightPos() { return lightPos_; }
		inline Float3 GetSLightAtten() { return lightAtten_; }
		inline bool IsActive() { return active_; }
		inline Float2 GetSLightFactorAngle() { return lightFactorAngleCos_; }
	private:
		Vector3D ligntDir_ = {1,0,0};
		Float3 lightPos_ = {0,0,0};
		Float3 lightColor_ = {1,1,1};
		Float3 lightAtten_ = {1.f,1.f,1.f};
		Float2 lightFactorAngleCos_ = {0.5f,0.2f};
		float shininess_ = 3.0f;
		size_t active_ = false;

	};

}

