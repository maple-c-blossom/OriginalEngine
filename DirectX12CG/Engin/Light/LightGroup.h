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

		static ID3D12Device* sdevice_;
		LightGroup() {};
		LightGroup(const LightGroup& lghtGroup) {};
		LightGroup& operator=(const LightGroup& lightGroup) {};
		~LightGroup() {};
	public: 

		static LightGroup* GetInstance();

		static LightGroup* GetInitInstance();
		void Initialize();
		void UpDate();
		void Draw(  uint32_t rootParameterIndex);
		void TransferConstBuff();
		void DefaultLightSet();

		void SetAmbientColor(const Float3& color);
		Float3 GetAmbientColor();
#pragma region DirL
		void SetDirLightIsActive( int32_t lightindexNum,  bool active);
		void SetDirLightForLightDir( int32_t lightindexNum, const Vector3D& lightdir);
		void SetDirLightColor( int32_t lightindexNum, const Float3& lightColor);
		void SetDirLightShininess( int32_t lightindexNum,  float lightShininess);

		bool GetDirLightIsActive( int32_t lightindexNum);
		Vector3D GetDirLightForLightDir( int32_t lightindexNum);
		Float3 GetDirLightColor( int32_t lightindexNum);
		float GetDirLightShininess( int32_t lightindexNum);
#pragma endregion DirL

#pragma region PointL
		void SetPLightIsActive( int32_t lightindexNum, bool active);
		void SetPLightPos( int32_t lightindexNum, const Float3& lightPos);
		void SetPLightColor( int32_t lightindexNum, const Float3& lightColor);
		void SetPLightAtten( int32_t lightindexNum, const Float3& lightAtten);
		void SetPLightShininess( int32_t lightindexNum,  float lightShininess);

		bool GetPLightIsActive( int32_t lightindexNum);
		Float3 GetPLightPos( int32_t lightindexNum);
		Float3 GetPLightColor( int32_t lightindexNum);
		Float3 GetPLightAtten( int32_t lightindexNum);
		float GetPLightShininess( int32_t lightindexNum);
#pragma endregion PointL

#pragma region SpotL

		void SetSLightIsActive( int32_t lightindexNum, bool active);
		void SetSLightForLightDir( int32_t lightindexNum, const Vector3D& lightdir);
		void SetSLightPos( int32_t lightindexNum, const Float3& lightPos);
		void SetSLightColor( int32_t lightindexNum, const Float3& lightColor);
		void SetSLightAtten( int32_t lightindexNum, const Float3& lightAtten);
		void SetSLightShininess( int32_t lightindexNum,  float lightShininess);

		bool GetSLightIsActive( int32_t lightindexNum);
		Vector3D GetSLightForLightDir( int32_t lightindexNum);
		Float3 GetSLightPos( int32_t lightindexNum);
		Float3 GetSLightColor( int32_t lightindexNum);
		Float3 GetSLightAtten( int32_t lightindexNum);
		float GetSLightShininess( int32_t lightindexNum);

#pragma endregion SpotL

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
		Float3 ambientColor_ = { 1,1,1 };
		DirLight dirLights_[sDIR_LIGHT_NUM_];
		PointLight PLights_[sP_LIGHT_NUM_];
		SpotLight SLights_[sS_LIGHT_NUM_];
		bool isUpdate_ = false;
	};

}

