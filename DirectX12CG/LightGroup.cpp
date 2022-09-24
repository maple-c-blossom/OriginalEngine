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

LightGroup* LightGroup::LightsCreate()
{
	// 3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	// 初期化
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
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(LightGroupConstBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
	}

	// 定数バッファへデータ転送
	TransferConstBuff();
}

void LightGroup::UpDate()
{
	// 値の更新があった時だけ定数バッファに転送する
	if (isUpdate) {
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
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor;
		for (int i = 0; i < DirLightNum; i++) {
			// ライトが有効なら設定を転送
			if (dirLights[i].IsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightV = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
				constMap->dirLights[i].shininess = dirLights[i].GetLightShininess();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->dirLights[i].active = 0;
			}
		}
		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSet()
{
	dirLights[0].SetIsActive(true);
	dirLights[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[0].SetLightDir({ 0.0f, -1.0f, 0.0f});
	dirLights[0].SetLightShininess(3.0f);

	dirLights[1].SetIsActive(true);
	dirLights[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[1].SetLightDir({ +0.5f, +0.1f, +0.2f });
	dirLights[1].SetLightShininess(3.0f);

	dirLights[2].SetIsActive(true);
	dirLights[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[2].SetLightDir({ -0.5f, +0.1f, -0.2f });
	dirLights[2].SetLightShininess(3.0f);
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

bool MCB::LightGroup::GetDirLightIsActive(int lightNum)
{
	assert(0 <= lightNum && lightNum < DirLightNum);
	return dirLights[lightNum].IsActive();
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
