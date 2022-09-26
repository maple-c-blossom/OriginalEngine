#pragma once
#include "Vector3D.h"
#include <wrl.h>
#include <Windows.h>
#include "Dx12.h"
#include <d3d12.h>
#include <d3dx12.h>

namespace MCB
{
	class DirLight
	{
	public:
		typedef struct LightConstBuffData
		{
			Vector3D lightV;
			float shininess;
			Float3 lightColor;
			unsigned int active;
		};
		inline void SetLightDir(Vector3D lightdir) 
		{
			lightdir.V3Norm();
			this->lightdir = lightdir;
		}
		inline Vector3D GetLightDir() { return lightdir; }
		inline void SetLightColor(const Float3& lightcolor) { this->lightColor = lightcolor; }
		inline Float3 GetLightColor() { return lightColor; }
		inline void SetLightShininess(const float lightshininess) { this->shininess = lightshininess; }
		inline float GetLightShininess() { return shininess; }
		inline void SetIsActive(bool active) { this->active = active; }
		inline bool IsActive() { return active; }
	private:

		Vector3D lightdir = { 1,0,0 };
		Float3 lightColor = { 1,1,1 };
		float shininess = 3.0f;
		bool active = false;
		//static DirLight* LightCreate();
		//void SetLightDir(Vector3D lightdir);
		//void SetLightColor(Float3 lightColor);
		//void CreateLightBuff();
		//void MapingLightConstBuff();
		//void Init();
		//void Updata();
		//void Draw(UINT rootPramIndex);
	};

}

