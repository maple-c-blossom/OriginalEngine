#include "LightGroup.h"
#include <assert.h>
#include "Dx12.h"
using namespace DirectX;
using namespace MCB;


ID3D12Device* MCB::LightGroup::device = nullptr;

void LightGroup::StaticInitialize()
{
	// �ď������`�F�b�N
	assert(!LightGroup::device);

	// nullptr�`�F�b�N
	assert(Dx12::GetInstance()->device.Get());

	LightGroup::device = Dx12::GetInstance()->device.Get();
}

LightGroup* LightGroup::LightsCreate()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	LightGroup* instance = new LightGroup();

	// ������
	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	DefaultLightSet();

	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(LightGroupConstBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
	}

	// �萔�o�b�t�@�փf�[�^�]��
	TransferConstBuff();
}

void LightGroup::UpDate()
{
	// �l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (isUpdate) {
		TransferConstBuff();
		isUpdate = false;
	}
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	// �萔�o�b�t�@�r���[���Z�b�g
	Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuff()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	LightGroupConstBuff* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor;
		for (int i = 0; i < DirLightNum; i++) {
			// ���C�g���L���Ȃ�ݒ��]��
			if (dirLights[i].IsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightV = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
				constMap->dirLights[i].shininess = dirLights[i].GetLightShininess();
			}
			// ���C�g�������Ȃ烉�C�g�F��0��
			else {
				constMap->dirLights[i].active = 0;
			}
		}

		for (int i = 0; i < PLightNum; i++) {
			// ���C�g���L���Ȃ�ݒ��]��
			if (PLights[i].IsActive()) {
				constMap->PLights[i].active = 1;
				constMap->PLights[i].lightPos = PLights[i].GetPLightPos();
				constMap->PLights[i].lightColor = PLights[i].GetPLightColor();
				constMap->PLights[i].lightAtten = PLights[i].GetPLightAtten();
			}
			// ���C�g�������Ȃ烉�C�g�F��0��
			else {
				constMap->dirLights[i].active = 0;
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
	PLights[0].SetPLightPos({ 10.0f,2.0f,20.0f });
	PLights[0].SetPLightColor({ 1,1,1 });
	PLights[0].SetPLightAtten({0.001,0.001,0.001});
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