#include "Light.h"
#include <memory>
using namespace MCB;

//
//DirLight* MCB::DirLight::LightCreate()
//{
//    DirLight* instance = new DirLight;
//    instance->Init();
//    return instance;
//}
//
//void MCB::DirLight::SetLightDir(Vector3D lightdir)
//{
//    lightdir.V3Norm();
//    lightdir = lightdir;
//    dirty = true;
//}
//
//void MCB::DirLight::SetLightColor(Float3 lightColor)
//{
//    lightColor = lightColor;
//    dirty = true;
//}
//
//void MCB::DirLight::CreateLightBuff()
//{
//
//	 Dx12::GetInstance()->result = Dx12::GetInstance()->device->CreateCommittedResource(
//        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
//        D3D12_HEAP_FLAG_NONE,
//        &CD3DX12_RESOURCE_DESC::Buffer((sizeof(LightConstBuffData) + 0xff)& ~0xff), // リソース設定
//        D3D12_RESOURCE_STATE_GENERIC_READ,
//        nullptr,
//        IID_PPV_ARGS(&lightConstBuff));
//    assert(SUCCEEDED(Dx12::GetInstance()->result));
//}
//
//void MCB::DirLight::MapingLightConstBuff()
//{
//    HRESULT result;
//    LightConstBuffData* constMap = nullptr;
//    result = lightConstBuff->Map(0, nullptr, (void**)&constMap);
//    if (SUCCEEDED(result))
//    {
//        constMap->lightV = -lightdir;
//        constMap->lightColor = lightColor;
//        lightConstBuff->Unmap(0,nullptr);
//    }
//}
//
//void MCB::DirLight::Init()
//{
//    CreateLightBuff();
//    MapingLightConstBuff();
//}
//
//void MCB::DirLight::Updata()
//{
//    if (dirty)
//    {
//        MapingLightConstBuff();
//        dirty = false;
//    }
//}
//
//void MCB::DirLight::Draw(uint32_t rootPramIndex)
//{
//    Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(rootPramIndex, lightConstBuff->GetGPUVirtualAddress());
//}
