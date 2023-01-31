#pragma once
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <wrl.h>
namespace MCB
{
	class LightGroup
	{
	public:
		static const int DirLightNum = 3;
		static const int PLightNum = 3;
		static const int SLightNum = 3;

		struct LightGroupConstBuff
		{
			Float3 ambientColor;
			float pad1;
			DirLight::LightConstBuffData dirLights[DirLightNum];
			PointLight::PointLConstBuffData PLights[PLightNum];
			SpotLight::SpotLConstBuffData SLights[SLightNum];
		};
	public:
		static void StaticInitialize();

	private: 

		static ID3D12Device* device;
		LightGroup() {};
		LightGroup(const LightGroup& lghtGroup) {};
		LightGroup& operator=(const LightGroup& lightGroup) {};
		~LightGroup() {};
	public: 

		static LightGroup* GetInstance();
		static void DeleteInstace();
		static LightGroup* GetInitInstance();
		void Initialize();
		void UpDate();
		void Draw( UINT rootParameterIndex);
		void TransferConstBuff();
		void DefaultLightSet();

		void SetAmbientColor(const Float3& color);
		Float3 GetAmbientColor();
#pragma region DirL
		void SetDirLightIsActive(int lightindexNum, bool active);
		void SetDirLightForLightDir(int lightindexNum, const Vector3D& lightdir);
		void SetDirLightColor(int lightindexNum, const Float3& lightColor);
		void SetDirLightShininess(int lightindexNum, const float& lightShininess);

		bool GetDirLightIsActive(int lightindexNum);
		Vector3D GetDirLightForLightDir(int lightindexNum);
		Float3 GetDirLightColor(int lightindexNum);
		float GetDirLightShininess(int lightindexNum);
#pragma endregion DirL

#pragma region PointL
		void SetPLightIsActive(int lightindexNum, bool active);
		void SetPLightPos(int lightindexNum, const Float3& lightPos);
		void SetPLightColor(int lightindexNum, const Float3& lightColor);
		void SetPLightAtten(int lightindexNum, const Float3& lightAtten);
		void SetPLightShininess(int lightindexNum, const float& lightShininess);

		bool GetPLightIsActive(int lightindexNum);
		Float3 GetPLightPos(int lightindexNum);
		Float3 GetPLightColor(int lightindexNum);
		Float3 GetPLightAtten(int lightindexNum);
		float GetPLightShininess(int lightindexNum);
#pragma endregion PointL

#pragma region SpotL

		void SetSLightIsActive(int lightindexNum, bool active);
		void SetSLightForLightDir(int lightindexNum, const Vector3D& lightdir);
		void SetSLightPos(int lightindexNum, const Float3& lightPos);
		void SetSLightColor(int lightindexNum, const Float3& lightColor);
		void SetSLightAtten(int lightindexNum, const Float3& lightAtten);
		void SetSLightShininess(int lightindexNum, const float& lightShininess);

		bool GetSLightIsActive(int lightindexNum);
		Vector3D GetSLightForLightDir(int lightindexNum);
		Float3 GetSLightPos(int lightindexNum);
		Float3 GetSLightColor(int lightindexNum);
		Float3 GetSLightAtten(int lightindexNum);
		float GetSLightShininess(int lightindexNum);

#pragma endregion SpotL

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		Float3 ambientColor = { 1,1,1 };
		DirLight dirLights[DirLightNum];
		PointLight PLights[PLightNum];
		SpotLight SLights[SLightNum];
		bool isUpdate = false;
	};

}

