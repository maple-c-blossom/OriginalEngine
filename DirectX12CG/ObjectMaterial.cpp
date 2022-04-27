#include "ObjectMaterial.h"

using namespace MCB;
using namespace DirectX;

void ObjectMaterial::Init(Dx12 dx12)
{
    
    HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

 
    Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    Resdesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
    Resdesc.Height = 1;
    Resdesc.DepthOrArraySize = 1;
    Resdesc.MipLevels = 1;
    Resdesc.SampleDesc.Count = 1;
    Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    dx12.result = dx12.device->CreateCommittedResource
    (
        &HeapProp,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &Resdesc,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffMaterial)
    );
    assert(SUCCEEDED(dx12.result));

    dx12.result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);

    assert(SUCCEEDED(dx12.result));

    constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);
}

void MCB::ObjectMaterial::SetIndex(D3D12_RESOURCE_DIMENSION dimension, UINT sizeIB, int height, int DepthOrArraySize, int MipLevels, int SampleDescCount, D3D12_TEXTURE_LAYOUT layput)
{
    Resdesc.Dimension = dimension;
    Resdesc.Width = sizeIB;
    Resdesc.Height = height;
    Resdesc.DepthOrArraySize = DepthOrArraySize;
    Resdesc.MipLevels = MipLevels;
    Resdesc.SampleDesc.Count = SampleDescCount;
    Resdesc.Layout = layput;
}
