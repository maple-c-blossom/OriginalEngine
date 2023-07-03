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
			uint32_t active;
		}LightConstBuffData;
		inline void SetLightDir(const Vector3D& lightdir) 
		{
			lightdir_ = lightdir;
		}
		inline Vector3D GetLightDir() { return lightdir_; }
		inline void SetLightColor(const Float3& lightcolor) { lightColor_ = lightcolor; }
		inline Float3 GetLightColor() { return lightColor_; }
		inline void SetLightShininess( float lightshininess) { shininess_ = lightshininess; }
		inline float GetLightShininess() { return shininess_; }
		inline void SetIsActive( bool active) { active_ = active; }
		inline bool IsActive() { return active_; }
	private:

		Vector3D lightdir_ = { 1,0,0 };
		Float3 lightColor_ = { 1,1,1 };
		float shininess_ = 3.0f;
		bool active_ = false;
		//static DirLight* LightCreate();
		//void SetLightDir(Vector3D lightdir);
		//void SetLightColor(Float3 lightColor);
		//void CreateLightBuff();
		//void MapingLightConstBuff();
		//void Init();
		//void Updata();
		//void Draw(uint32_t rootPramIndex);
	};

}

