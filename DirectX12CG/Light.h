#pragma once
#include "Vector3D.h"
#include <wrl.h>
#include <Windows.h>
#include <wrl.h>
#include "Dx12.h"
#include <d3d12.h>
#include <d3dx12.h>

namespace MCB
{
	class DirLight
	{
	public:
		struct LightConstBuffData
		{
			Vector3D lightV;
			float pad1;
			Float3 lightColor;
		};
		Microsoft::WRL::ComPtr<ID3D12Resource> lightConstBuff;
		inline void SetLightDir(Vector3D lightdir) 
		{
			lightdir.V3Norm();
			this->lightdir = lightdir;
		}
		inline const Vector3D& GetLightDir() { return lightdir; }
		inline void SetLightColor(const Float3& lightcolor) { this->lightColor = lightcolor; }
		inline const Float3& GetLightColor() { return lightColor; }
		inline void SetActive(bool active) { this->active = active; }
		inline bool IsActive() { return active; }
	private:

		Vector3D lightdir = { 1,0,0 };
		Float3 lightColor = { 1,1,1 };
		bool active = true;
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

