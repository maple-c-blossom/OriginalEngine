#pragma once
#include "Pading.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <wrl.h>
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
WarningIgnoreEnd
#include "Vector3D.h"
#include "Dx12.h"

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
		void SetLightDir(const Vector3D& lightdir);

		Vector3D GetLightDir();
		void SetLightColor(const Float3& lightcolor);
		Float3 GetLightColor();
		void SetLightShininess(float lightshininess);
		float GetLightShininess();
		void SetIsActive(bool active);
		bool IsActive();
	private:

		Vector3D lightdir_ = { 1,0,0 };
		Float3 lightColor_ = { 1,1,1 };
		float shininess_ = 3.0f;
		bool active_ = false;
		Byte3 pad1;
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

