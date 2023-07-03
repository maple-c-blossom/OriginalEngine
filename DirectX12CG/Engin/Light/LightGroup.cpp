#include "LightGroup.h"
#include <assert.h>
#include "Dx12.h"
using namespace DirectX;
using namespace MCB;


ID3D12Device* MCB::LightGroup::sdevice_ = nullptr;

void LightGroup::StaticInitialize()
{
	// 再初期化チェック
	assert(!LightGroup::sdevice_);

	// nullptrチェック
	assert(Dx12::GetInstance()->device_.Get());

	LightGroup::sdevice_ = Dx12::GetInstance()->device_.Get();
}


LightGroup* MCB::LightGroup::GetInstance()
{
	static LightGroup instance;
	return &instance;
}


LightGroup* MCB::LightGroup::GetInitInstance()
{
	LightGroup* instance = LightGroup::GetInstance();
	LightGroup::StaticInitialize();
	instance->Initialize();
	return instance;
}
void LightGroup::Initialize()
{
	// nullptrチェック
	assert(sdevice_);

	DefaultLightSet();

	HRESULT result;
	// 定数バッファの生成
	result = sdevice_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(LightGroupConstBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);
	if (FAILED(result)) 
	{
		assert(0);
	}

	// 定数バッファへデータ転送
	TransferConstBuff();
}

void LightGroup::UpDate()
{
	// 値の更新があった時だけ定数バッファに転送する
	if (isUpdate_) 
	{
		TransferConstBuff();
		isUpdate_ = false;
	}
}

void LightGroup::Draw( uint32_t rootParameterIndex)
{
	// 定数バッファビューをセット
	Dx12::GetInstance()->commandList_->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff_->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuff()
{
	HRESULT result;
	// 定数バッファへデータ転送
	LightGroupConstBuff* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) 
	{
		constMap->ambientColor = ambientColor_;
		for (int32_t i = 0; i < sDIR_LIGHT_NUM_; i++) 
		{
			// ライトが有効なら設定を転送
			if (dirLights_[i].IsActive())
			{
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightV = -dirLights_[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights_[i].GetLightColor();
				constMap->dirLights[i].shininess = dirLights_[i].GetLightShininess();
			}
			// ライトが無効ならライト色を0に
			else 
			{
				constMap->dirLights[i].active = 0;
			}
		}

		for (int32_t i = 0; i < sP_LIGHT_NUM_; i++)
		{
			// ライトが有効なら設定を転送
			if (PLights_[i].IsActive()) 
			{
				constMap->PLights[i].active = 1;
				constMap->PLights[i].lightPos = PLights_[i].GetPLightPos();
				constMap->PLights[i].lightColor = PLights_[i].GetPLightColor();
				constMap->PLights[i].lightAtten = PLights_[i].GetPLightAtten();
				constMap->PLights[i].shininess = PLights_[i].GetPLightShininess();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->PLights[i].active = 0;
			}
		}

		for (int32_t i = 0; i < sS_LIGHT_NUM_; i++) 
		{
			// ライトが有効なら設定を転送
			if (SLights_[i].IsActive()) 
			{
				constMap->SLights[i].active = 1;
				constMap->SLights[i].ligntVec = -SLights_[i].GetSLightDir();
				constMap->SLights[i].lightPos = SLights_[i].GetSLightPos();
				constMap->SLights[i].lightColor = SLights_[i].GetSLightColor();
				constMap->SLights[i].lightAtten = SLights_[i].GetSLightAtten();
				constMap->SLights[i].shininess = SLights_[i].GetSLightShininess();
				constMap->SLights[i].lightFactorAngleCos = SLights_[i].GetSLightFactorAngle();
			}
			// ライトが無効ならライト色を0に
			else 
			{
				constMap->SLights[i].active = 0;
			}
		}
		constBuff_->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSet()
{
	dirLights_[0].SetIsActive(true);
	dirLights_[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[0].SetLightDir({ -1.0f, -1.0f, 0.0f});
	dirLights_[0].SetLightShininess(4.0f);

	dirLights_[1].SetIsActive(false);
	dirLights_[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[1].SetLightDir({ +0.5f, +0.1f, +0.2f });
	dirLights_[1].SetLightShininess(3.0f);

	dirLights_[2].SetIsActive(false);
	dirLights_[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[2].SetLightDir({ -0.5f, +0.1f, -0.2f });
	dirLights_[2].SetLightShininess(3.0f);

	PLights_[0].SetIsActive(false);
	PLights_[0].SetPLightPos({ 0, 2, 10 });
	PLights_[0].SetPLightColor({ 0,1,1 });
	PLights_[0].SetPLightAtten({0.01f,0.01f,0.01f});
	
	PLights_[1].SetIsActive(false);
	PLights_[1].SetPLightPos({ 10.0f,2.0f,20.0f });
	PLights_[1].SetPLightColor({ 1,1,1 });
	PLights_[1].SetPLightAtten({ 1,1,1 });

	PLights_[2].SetIsActive(false);
	PLights_[2].SetPLightPos({ 10.0f,2.0f,20.0f });
	PLights_[2].SetPLightColor({ 1,1,1 });
	PLights_[2].SetPLightAtten({ 0.001f,0.001f,0.001f });

	SLights_[0].SetIsActive(false);
	SLights_[0].SetSLightPos({ 0.0f,3.0f,20.0f });
	SLights_[0].SetSLightColor({ 1,0,0 });
	SLights_[0].SetSLightAtten({ 0.0,0.0,0.0 });
	SLights_[0].SetSLightDir({ 0,-1,0 });
	SLights_[0].SetSLightFactorAngle({ 20.0f,30.0f},false);

	SLights_[1].SetIsActive(false);
	SLights_[1].SetSLightPos({ 10.0f,2.0f,20.0f });
	SLights_[1].SetSLightColor({ 1,1,1 });
	SLights_[1].SetSLightAtten({ 0.001f,0.001f,0.001f });
	SLights_[1].SetSLightDir({ 0.0,0.0,0.0 });
	SLights_[1].SetSLightFactorAngle({ 0.001f,0.001f });

	SLights_[2].SetIsActive(false);
	SLights_[2].SetSLightPos({ 10.0f,2.0f,20.0f });
	SLights_[2].SetSLightColor({ 1,1,1 });
	SLights_[2].SetSLightAtten({ 0.001f,0.001f,0.001f });
	SLights_[2].SetSLightDir({ 0.001f,0.001f,0.001f });
	SLights_[2].SetSLightFactorAngle({ 0.001f,0.001f });
	isUpdate_ = true;
}

void LightGroup::SetAmbientColor(const Float3& color)
{
	ambientColor_ = color;
	isUpdate_ = true;
}

void LightGroup::SetDirLightIsActive( int32_t lightindexNum, bool active)
{
	assert(0 <= lightindexNum && lightindexNum < sDIR_LIGHT_NUM_);

	dirLights_[lightindexNum].SetIsActive(active);
	isUpdate_ = true;
}

void LightGroup::SetDirLightForLightDir( int32_t lightindexNum, const Vector3D& lightdir)
{
	assert(0 <= lightindexNum && lightindexNum < sDIR_LIGHT_NUM_);

	dirLights_[lightindexNum].SetLightDir(lightdir);
	isUpdate_ = true;
}

void LightGroup::SetDirLightColor( int32_t lightindexNum, const Float3& lightcolor)
{
	assert(0 <= lightindexNum && lightindexNum < sDIR_LIGHT_NUM_);

	dirLights_[lightindexNum].SetLightColor(lightcolor);
	isUpdate_ = true;
}

void MCB::LightGroup::SetDirLightShininess( int32_t lightindexNum,  float lightShininess)
{
	assert(0 <= lightindexNum && lightindexNum < sDIR_LIGHT_NUM_);

	dirLights_[lightindexNum].SetLightShininess(lightShininess);
	isUpdate_ = true;
}

Float3 MCB::LightGroup::GetAmbientColor()
{
	return ambientColor_;
}

bool MCB::LightGroup::GetDirLightIsActive( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sDIR_LIGHT_NUM_);
	return dirLights_[lightindexNum].IsActive();
}

Vector3D MCB::LightGroup::GetDirLightForLightDir( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sDIR_LIGHT_NUM_);
	return dirLights_[lightindexNum].GetLightDir();
}

Float3 MCB::LightGroup::GetDirLightColor( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sDIR_LIGHT_NUM_);
	return dirLights_[lightindexNum].GetLightColor();
}

float MCB::LightGroup::GetDirLightShininess( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sDIR_LIGHT_NUM_);
	return dirLights_[lightindexNum].GetLightShininess();
}

void MCB::LightGroup::SetPLightIsActive( int32_t lightindexNum, bool active)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	PLights_[lightindexNum].SetIsActive(active);
	isUpdate_ = true;
}

void MCB::LightGroup::SetPLightPos( int32_t lightindexNum, const Float3& lightPos)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	PLights_[lightindexNum].SetPLightPos(lightPos);
	isUpdate_ = true;
}

void MCB::LightGroup::SetPLightColor( int32_t lightindexNum, const Float3& lightColor)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	PLights_[lightindexNum].SetPLightColor(lightColor);
	isUpdate_ = true;
}

void MCB::LightGroup::SetPLightAtten( int32_t lightindexNum, const Float3& lightAtten)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	PLights_[lightindexNum].SetPLightAtten(lightAtten);
	isUpdate_ = true;
}

void MCB::LightGroup::SetPLightShininess( int32_t lightindexNum,  float lightShininess)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	PLights_[lightindexNum].SetPLightShininess(lightShininess);
	isUpdate_ = true;
}

bool MCB::LightGroup::GetPLightIsActive( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	return PLights_[lightindexNum].IsActive();
}

Float3 MCB::LightGroup::GetPLightPos( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	return PLights_[lightindexNum].GetPLightPos();
}

Float3 MCB::LightGroup::GetPLightColor( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	return PLights_[lightindexNum].GetPLightColor();
}

Float3 MCB::LightGroup::GetPLightAtten( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	return PLights_[lightindexNum].GetPLightAtten();
}

float MCB::LightGroup::GetPLightShininess( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	return PLights_[lightindexNum].GetPLightShininess();
}

void MCB::LightGroup::SetSLightIsActive( int32_t lightindexNum, bool active)
{
	assert(0 <= lightindexNum && lightindexNum < sP_LIGHT_NUM_);
	SLights_[lightindexNum].SetIsActive(active);
	isUpdate_ = true;
}

void MCB::LightGroup::SetSLightForLightDir( int32_t lightindexNum, const Vector3D& lightdir)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	SLights_[lightindexNum].SetSLightDir(lightdir);
	isUpdate_ = true;
}

void MCB::LightGroup::SetSLightPos( int32_t lightindexNum, const Float3& lightPos)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	SLights_[lightindexNum].SetSLightPos(lightPos);
	isUpdate_ = true;
}

void MCB::LightGroup::SetSLightColor( int32_t lightindexNum, const Float3& lightColor)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	SLights_[lightindexNum].SetSLightColor(lightColor);
	isUpdate_ = true;
}

void MCB::LightGroup::SetSLightAtten( int32_t lightindexNum, const Float3& lightAtten)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	SLights_[lightindexNum].SetSLightAtten(lightAtten);
	isUpdate_ = true;
}

void MCB::LightGroup::SetSLightShininess( int32_t lightindexNum,  float lightShininess)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	SLights_[lightindexNum].SetSLightShininess(lightShininess);
	isUpdate_ = true;
}

Float3 MCB::LightGroup::GetSLightAtten( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	return SLights_[lightindexNum].GetSLightAtten();
}

float MCB::LightGroup::GetSLightShininess( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	return SLights_[lightindexNum].GetSLightShininess();
}



bool MCB::LightGroup::GetSLightIsActive( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	return SLights_[lightindexNum].IsActive();
}

Vector3D MCB::LightGroup::GetSLightForLightDir( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	return SLights_[lightindexNum].GetSLightDir();
}

Float3 MCB::LightGroup::GetSLightPos( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	return SLights_[lightindexNum].GetSLightPos();
}

Float3 MCB::LightGroup::GetSLightColor( int32_t lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < sS_LIGHT_NUM_);
	return SLights_[lightindexNum].GetSLightColor();
}
