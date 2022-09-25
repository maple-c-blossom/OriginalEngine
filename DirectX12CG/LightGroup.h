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
		static const int PLightNum = 3;

		struct LightGroupConstBuff
		{
			Float3 ambientColor;
			float pad1;
			DirLight::LightConstBuffData dirLights[DirLightNum];
			PointLight::PointLConstBuff PLights[PLightNum];
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
		void SetDirLightIsActive(int lightindexNum, bool active);
		void SetDirLightForLightDir(int lightindexNum, const Vector3D& lightdir);
		void SetDirLightColor(int lightindexNum, const Float3& lightColor);
		void SetDirLightShininess(int lightindexNum, const float& lightShininess);

		Float3 GetAmbientColor();
		bool GetDirLightIsActive(int lightindexNum);
		Vector3D GetDirLightForLightDir(int lightindexNum);
		Float3 GetDirLightColor(int lightindexNum);
		float GetDirLightShininess(int lightindexNum);


		void SetPLightIsActive(int lightindexNum, bool active);
		void SetPLightPos(int lightindexNum, const Float3& lightPos);
		void SetPLightColor(int lightindexNum, const Float3& lightColor);
		void SetPLightAtten(int lightindexNum, const Float3& lightAtten);

		bool GetPLightIsActive(int lightindexNum);
		Float3 GetPLightPos(int lightindexNum);
		Float3 GetPLightColor(int lightindexNum);
		Float3 GetPLightAtten(int lightindexNum);
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		Float3 ambientColor = { 1,1,1 };
		DirLight dirLights[DirLightNum];
		PointLight PLights[PLightNum];
		bool isUpdate = false;
	};

}

