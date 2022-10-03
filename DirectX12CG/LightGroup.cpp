#include "LightGroup.h"
#include <assert.h>
#include "Dx12.h"
using namespace DirectX;
using namespace MCB;


ID3D12Device* MCB::LightGroup::device = nullptr;

void LightGroup::StaticInitialize()
{
	// 再初期化チェック
	assert(!LightGroup::device);

	// nullptrチェック
	assert(Dx12::GetInstance()->device.Get());

	LightGroup::device = Dx12::GetInstance()->device.Get();
}


LightGroup* MCB::LightGroup::GetInstance()
{
	static LightGroup* instance = new LightGroup;
	return instance;
}

void MCB::LightGroup::DeleteInstace()
{
	delete LightGroup::GetInstance();
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
	assert(device);

	DefaultLightSet();

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(LightGroupConstBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
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
	if (isUpdate) 
	{
		TransferConstBuff();
		isUpdate = false;
	}
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	// 定数バッファビューをセット
	Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuff()
{
	HRESULT result;
	// 定数バッファへデータ転送
	LightGroupConstBuff* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) 
	{
		constMap->ambientColor = ambientColor;
		for (int i = 0; i < DirLightNum; i++) 
		{
			// ライトが有効なら設定を転送
			if (dirLights[i].IsActive())
			{
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightV = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
				constMap->dirLights[i].shininess = dirLights[i].GetLightShininess();
			}
			// ライトが無効ならライト色を0に
			else 
			{
				constMap->dirLights[i].active = 0;
			}
		}

		for (int i = 0; i < PLightNum; i++) 
		{
			// ライトが有効なら設定を転送
			if (PLights[i].IsActive()) 
			{
				constMap->PLights[i].active = 1;
				constMap->PLights[i].lightPos = PLights[i].GetPLightPos();
				constMap->PLights[i].lightColor = PLights[i].GetPLightColor();
				constMap->PLights[i].lightAtten = PLights[i].GetPLightAtten();
				constMap->PLights[i].shininess = PLights[i].GetPLightShininess();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->PLights[i].active = 0;
			}
		}

		for (int i = 0; i < SLightNum; i++) 
		{
			// ライトが有効なら設定を転送
			if (SLights[i].IsActive()) 
			{
				constMap->SLights[i].active = 1;
				constMap->SLights[i].ligntVec = -SLights[i].GetSLightDir();
				constMap->SLights[i].lightPos = SLights[i].GetSLightPos();
				constMap->SLights[i].lightColor = SLights[i].GetSLightColor();
				constMap->SLights[i].lightAtten = SLights[i].GetSLightAtten();
				constMap->SLights[i].shininess = SLights[i].GetSLightShininess();
				constMap->SLights[i].lightFactorAngleCos = SLights[i].GetSLightFactorAngle();
			}
			// ライトが無効ならライト色を0に
			else 
			{
				constMap->SLights[i].active = 0;
			}
		}
		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSet()
{
	dirLights[0].SetIsActive(false);
	dirLights[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[0].SetLightDir({ 0.0f, -1.0f, 0.0f});
	dirLights[0].SetLightShininess(3.0f);

	dirLights[1].SetIsActive(false);
	dirLights[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[1].SetLightDir({ +0.5f, +0.1f, +0.2f });
	dirLights[1].SetLightShininess(3.0f);

	dirLights[2].SetIsActive(false);
	dirLights[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[2].SetLightDir({ -0.5f, +0.1f, -0.2f });
	dirLights[2].SetLightShininess(3.0f);

	PLights[0].SetIsActive(true);
	PLights[0].SetPLightPos({ 0, 2, 10 });
	PLights[0].SetPLightColor({ 0,1,1 });
	PLights[0].SetPLightAtten({0.01,0.01,0.01});
	
	PLights[1].SetIsActive(false);
	PLights[1].SetPLightPos({ 10.0f,2.0f,20.0f });
	PLights[1].SetPLightColor({ 1,1,1 });
	PLights[1].SetPLightAtten({ 1,1,1 });

	PLights[2].SetIsActive(false);
	PLights[2].SetPLightPos({ 10.0f,2.0f,20.0f });
	PLights[2].SetPLightColor({ 1,1,1 });
	PLights[2].SetPLightAtten({ 0.001,0.001,0.001 });

	SLights[0].SetIsActive(true);
	SLights[0].SetSLightPos({ 0.0f,3.0f,20.0f });
	SLights[0].SetSLightColor({ 1,1,1 });
	SLights[0].SetSLightAtten({ 0.0,0.0,0.0 });
	SLights[0].SetSLightDir({ 0,-1,0 });
	SLights[0].SetSLightFactorAngle({ 20.0f,30.0f},false);

	SLights[1].SetIsActive(false);
	SLights[1].SetSLightPos({ 10.0f,2.0f,20.0f });
	SLights[1].SetSLightColor({ 1,1,1 });
	SLights[1].SetSLightAtten({ 0.001,0.001,0.001 });
	SLights[1].SetSLightDir({ 0.0,0.0,0.0 });
	SLights[1].SetSLightFactorAngle({ 0.001,0.001 });

	SLights[2].SetIsActive(false);
	SLights[2].SetSLightPos({ 10.0f,2.0f,20.0f });
	SLights[2].SetSLightColor({ 1,1,1 });
	SLights[2].SetSLightAtten({ 0.001,0.001,0.001 });
	SLights[2].SetSLightDir({ 0.001,0.001,0.001 });
	SLights[2].SetSLightFactorAngle({ 0.001,0.001 });
	isUpdate = true;
}

void LightGroup::SetAmbientColor(const Float3& color)
{
	ambientColor = color;
	isUpdate = true;
}

void LightGroup::SetDirLightIsActive(int lightindexNum, bool active)
{
	assert(0 <= lightindexNum && lightindexNum < DirLightNum);

	dirLights[lightindexNum].SetIsActive(active);
	isUpdate = true;
}

void LightGroup::SetDirLightForLightDir(int lightindexNum, const Vector3D& lightdir)
{
	assert(0 <= lightindexNum && lightindexNum < DirLightNum);

	dirLights[lightindexNum].SetLightDir(lightdir);
	isUpdate = true;
}

void LightGroup::SetDirLightColor(int lightindexNum, const Float3& lightcolor)
{
	assert(0 <= lightindexNum && lightindexNum < DirLightNum);

	dirLights[lightindexNum].SetLightColor(lightcolor);
	isUpdate = true;
}

void MCB::LightGroup::SetDirLightShininess(int lightindexNum, const float& lightShininess)
{
	assert(0 <= lightindexNum && lightindexNum < DirLightNum);

	dirLights[lightindexNum].SetLightShininess(lightShininess);
	isUpdate = true;
}

Float3 MCB::LightGroup::GetAmbientColor()
{
	return ambientColor;
}

bool MCB::LightGroup::GetDirLightIsActive(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < DirLightNum);
	return dirLights[lightindexNum].IsActive();
}

Vector3D MCB::LightGroup::GetDirLightForLightDir(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < DirLightNum);
	return dirLights[lightindexNum].GetLightDir();
}

Float3 MCB::LightGroup::GetDirLightColor(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < DirLightNum);
	return dirLights[lightindexNum].GetLightColor();
}

float MCB::LightGroup::GetDirLightShininess(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < DirLightNum);
	return dirLights[lightindexNum].GetLightShininess();
}

void MCB::LightGroup::SetPLightIsActive(int lightindexNum, bool active)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	PLights[lightindexNum].SetIsActive(active);
	isUpdate = true;
}

void MCB::LightGroup::SetPLightPos(int lightindexNum, const Float3& lightPos)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	PLights[lightindexNum].SetPLightPos(lightPos);
	isUpdate = true;
}

void MCB::LightGroup::SetPLightColor(int lightindexNum, const Float3& lightColor)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	PLights[lightindexNum].SetPLightColor(lightColor);
	isUpdate = true;
}

void MCB::LightGroup::SetPLightAtten(int lightindexNum, const Float3& lightAtten)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	PLights[lightindexNum].SetPLightAtten(lightAtten);
	isUpdate = true;
}

void MCB::LightGroup::SetPLightShininess(int lightindexNum, const float& lightShininess)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	PLights[lightindexNum].SetPLightShininess(lightShininess);
	isUpdate = true;
}

bool MCB::LightGroup::GetPLightIsActive(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	return PLights[lightindexNum].IsActive();
}

Float3 MCB::LightGroup::GetPLightPos(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	return PLights[lightindexNum].GetPLightPos();
}

Float3 MCB::LightGroup::GetPLightColor(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	return PLights[lightindexNum].GetPLightColor();
}

Float3 MCB::LightGroup::GetPLightAtten(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	return PLights[lightindexNum].GetPLightAtten();
}

float MCB::LightGroup::GetPLightShininess(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	return PLights[lightindexNum].GetPLightShininess();
}

void MCB::LightGroup::SetSLightIsActive(int lightindexNum, bool active)
{
	assert(0 <= lightindexNum && lightindexNum < PLightNum);
	SLights[lightindexNum].SetIsActive(active);
	isUpdate = true;
}

void MCB::LightGroup::SetSLightForLightDir(int lightindexNum, const Vector3D& lightdir)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	SLights[lightindexNum].SetSLightDir(lightdir);
	isUpdate = true;
}

void MCB::LightGroup::SetSLightPos(int lightindexNum, const Float3& lightPos)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	SLights[lightindexNum].SetSLightPos(lightPos);
	isUpdate = true;
}

void MCB::LightGroup::SetSLightColor(int lightindexNum, const Float3& lightColor)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	SLights[lightindexNum].SetSLightColor(lightColor);
	isUpdate = true;
}

void MCB::LightGroup::SetSLightAtten(int lightindexNum, const Float3& lightAtten)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	SLights[lightindexNum].SetSLightAtten(lightAtten);
	isUpdate = true;
}

void MCB::LightGroup::SetSLightShininess(int lightindexNum, const float& lightShininess)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	SLights[lightindexNum].SetSLightShininess(lightShininess);
	isUpdate = true;
}

Float3 MCB::LightGroup::GetSLightAtten(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	return SLights[lightindexNum].GetSLightAtten();
}

float MCB::LightGroup::GetSLightShininess(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	return SLights[lightindexNum].GetSLightShininess();
}



bool MCB::LightGroup::GetSLightIsActive(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	return SLights[lightindexNum].IsActive();
}

Vector3D MCB::LightGroup::GetSLightForLightDir(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	return SLights[lightindexNum].GetSLightDir();
}

Float3 MCB::LightGroup::GetSLightPos(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	return SLights[lightindexNum].GetSLightPos();
}

Float3 MCB::LightGroup::GetSLightColor(int lightindexNum)
{
	assert(0 <= lightindexNum && lightindexNum < SLightNum);
	return SLights[lightindexNum].GetSLightColor();
}
