#pragma once
#include "Light.h"
#include "PointLight.h"
#include <wrl.h>
namespace MCB
{
	class LightGroup
	{
	public:
		static const int DirLightNum = 3;

		struct LightGroupConstBuff
		{
			Float3 ambientColor;
			float pad1;
			DirLight::LightConstBuffData dirLights[DirLightNum];
		};
	public:
		static void StaticInitialize();
		static LightGroup* LightsCreate();

	private: 

		static ID3D12Device* device;

	public: 
		void Initialize();
		void UpDate();
		void Draw( UINT rootParameterIndex);
		void TransferConstBuff();
		void DefaultLightSet();
		void SetAmbientColor(const Float3& color);
		void SetDirLightIsActive(int lightNum, bool active);
		void SetDirLightForLightDir(int lightindexNum, const Vector3D& lightdir);
		void SetDirLightColor(int lightindexNum, const Float3& lightcolor);
		void SetDirLightShininess(int lightindexNum, const float& lightShininess);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		Float3 ambientColor = { 1,1,1 };
		DirLight dirLights[DirLightNum];
		bool isUpdate = false;
	};

}

