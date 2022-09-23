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

	private:

		Vector3D lightdir = { 1,0,0 };
		Float3 lightColor = { 1,1,1 };
		bool dirty = false;

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

