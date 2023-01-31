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
		inline float ConvertRadius(float angle)//Util.h“Ç‚Ýž‚ß‚È‚¢‚Ì‚Å‚±‚±‚Å‚Â‚­‚é
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
			unsigned int active;
			float pad5;
		};
		inline void SetSLightDir(Vector3D lightDir){ this->ligntDir = lightDir; }
		inline void SetSLightColor(const Float3& lightcolor) { this->lightColor = lightcolor; }
		inline void SetSLightPos(Float3 pos) { lightPos = pos; }
		inline void SetSLightAtten(Float3 atten) { lightAtten = atten; }
		inline void SetIsActive(bool active) { this->active = active; }
		inline void SetSLightFactorAngle(Float2 factorCos,bool convertFlag = true)
		{
			if (convertFlag)
			{
				lightFactorAngleCos.x = ConvertRadius(factorCos.x);
				lightFactorAngleCos.y = ConvertRadius(factorCos.y);
			}
			else
			{
				lightFactorAngleCos = factorCos;
			}
		}
		inline void SetSLightShininess(float shininess) { this->shininess = shininess; }

		inline float GetSLightShininess() { return shininess; }
		inline Vector3D GetSLightDir() { return ligntDir; }
		inline Float3 GetSLightColor() { return lightColor; }
		inline Float3 GetSLightPos() { return lightPos; }
		inline Float3 GetSLightAtten() { return lightAtten; }
		inline bool IsActive() { return active; }
		inline Float2 GetSLightFactorAngle() { return lightFactorAngleCos; }
	private:
		Vector3D ligntDir = {1,0,0};
		Float3 lightPos = {0,0,0};
		Float3 lightColor = {1,1,1};
		Float3 lightAtten = {1.f,1.f,1.f};
		Float2 lightFactorAngleCos = {0.5f,0.2f};
		float shininess = 3.0f;
		unsigned int active = false;

	};

}

