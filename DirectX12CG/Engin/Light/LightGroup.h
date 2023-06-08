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
		static const int32_t sDIR_LIGHT_NUM_ = 3;
		static const int32_t sP_LIGHT_NUM_ = 3;
		static const int32_t sS_LIGHT_NUM_ = 3;

		struct LightGroupConstBuff
		{
			Float3 ambientColor;
			float pad1;
			DirLight::LightConstBuffData dirLights[sDIR_LIGHT_NUM_];
			PointLight::PointLConstBuffData PLights[sP_LIGHT_NUM_];
			SpotLight::SpotLConstBuffData SLights[sS_LIGHT_NUM_];
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

		static LightGroup* GetInitInstance();
		void Initialize();
		void UpDate();
		void Draw( const uint32_t& rootParameterIndex);
		void TransferConstBuff();
		void DefaultLightSet();

		void SetAmbientColor(const Float3& color);
		Float3 GetAmbientColor();
#pragma region DirL
		void SetDirLightIsActive(const int32_t& lightindexNum, bool active);
		void SetDirLightForLightDir(const int32_t& lightindexNum, const Vector3D& lightdir);
		void SetDirLightColor(const int32_t& lightindexNum, const Float3& lightColor);
		void SetDirLightShininess(const int32_t& lightindexNum, const float& lightShininess);

		bool GetDirLightIsActive(const int32_t& lightindexNum);
		Vector3D GetDirLightForLightDir(const int32_t& lightindexNum);
		Float3 GetDirLightColor(const int32_t& lightindexNum);
		float GetDirLightShininess(const int32_t& lightindexNum);
#pragma endregion DirL

#pragma region PointL
		void SetPLightIsActive(const int32_t& lightindexNum, bool active);
		void SetPLightPos(const int32_t& lightindexNum, const Float3& lightPos);
		void SetPLightColor(const int32_t& lightindexNum, const Float3& lightColor);
		void SetPLightAtten(const int32_t& lightindexNum, const Float3& lightAtten);
		void SetPLightShininess(const int32_t& lightindexNum, const float& lightShininess);

		bool GetPLightIsActive(const int32_t& lightindexNum);
		Float3 GetPLightPos(const int32_t& lightindexNum);
		Float3 GetPLightColor(const int32_t& lightindexNum);
		Float3 GetPLightAtten(const int32_t& lightindexNum);
		float GetPLightShininess(const int32_t& lightindexNum);
#pragma endregion PointL

#pragma region SpotL

		void SetSLightIsActive(const int32_t& lightindexNum, bool active);
		void SetSLightForLightDir(const int32_t& lightindexNum, const Vector3D& lightdir);
		void SetSLightPos(const int32_t& lightindexNum, const Float3& lightPos);
		void SetSLightColor(const int32_t& lightindexNum, const Float3& lightColor);
		void SetSLightAtten(const int32_t& lightindexNum, const Float3& lightAtten);
		void SetSLightShininess(const int32_t& lightindexNum, const float& lightShininess);

		bool GetSLightIsActive(const int32_t& lightindexNum);
		Vector3D GetSLightForLightDir(const int32_t& lightindexNum);
		Float3 GetSLightPos(const int32_t& lightindexNum);
		Float3 GetSLightColor(const int32_t& lightindexNum);
		Float3 GetSLightAtten(const int32_t& lightindexNum);
		float GetSLightShininess(const int32_t& lightindexNum);

#pragma endregion SpotL

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		Float3 ambientColor = { 1,1,1 };
		DirLight dirLights[sDIR_LIGHT_NUM_];
		PointLight PLights[sP_LIGHT_NUM_];
		SpotLight SLights[sS_LIGHT_NUM_];
		bool isUpdate = false;
	};

}

