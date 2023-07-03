#include "ObjectMaterial.h"

using namespace MCB;
using namespace DirectX;

MCB::ObjectMaterial::~ObjectMaterial()
{
    //constBuffMaterialB1->Unmap(0, nullptr);
}

void ObjectMaterial::Init()
{
    Dx12* dx12 = Dx12::GetInstance();
    HeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;

 
    Resdesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    Resdesc_.Width = (sizeof(ConstBufferDataMaterialB1) + 0xff) & ~0xff;
    Resdesc_.Height = 1;
    Resdesc_.DepthOrArraySize = 1;
    Resdesc_.MipLevels = 1;
    Resdesc_.SampleDesc.Count = 1;
    Resdesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    dx12->result_ = dx12->device_->CreateCommittedResource
    (
        &HeapProp_,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &Resdesc_,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffMaterialB1_)
    );
    assert(SUCCEEDED(dx12->result_));

    dx12->result_ = constBuffMaterialB1_->Map(0, nullptr, (void**)&constMapMaterial_);

    assert(SUCCEEDED(dx12->result_));

    constMapMaterial_->ambient = material_.ambient;
    constMapMaterial_->diffuse = material_.diffuse;
    constMapMaterial_->specular = material_.specular;
    constMapMaterial_->alpha = material_.alpha;

    //constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);
}

void MCB::ObjectMaterial::Update()
{
    constMapMaterial_->ambient = material_.ambient;
    constMapMaterial_->diffuse = material_.diffuse;
    constMapMaterial_->specular = material_.specular;
    constMapMaterial_->alpha = material_.alpha;
}


void MCB::ObjectMaterial::SetIndex(const D3D12_RESOURCE_DIMENSION& dimension, uint32_t sizeIB,
    int32_t height, int32_t DepthOrArraySize, int32_t MipLevels, int32_t SampleDescCount,
    const D3D12_TEXTURE_LAYOUT& layput)
{
    Resdesc_.Dimension = dimension;
    Resdesc_.Width = sizeIB;
    Resdesc_.Height = height;
    Resdesc_.DepthOrArraySize = DepthOrArraySize;
    Resdesc_.MipLevels = MipLevels;
    Resdesc_.SampleDesc.Count = SampleDescCount;
    Resdesc_.Layout = layput;
}

void MCB::ObjectMaterial::SetVertexBuffer(const D3D12_HEAP_TYPE& heaptype,const D3D12_RESOURCE_DIMENSION& dimension,
    uint32_t sizeVB, int32_t height, int32_t DepthOrArraySize, 
    int32_t MipLevels, int32_t SampleDescCount,const D3D12_TEXTURE_LAYOUT& layput)
{
    HeapProp_.Type = heaptype; // GPUへの転送用

    Resdesc_.Dimension = dimension;
    Resdesc_.Width = sizeVB; // 頂点データ全体のサイズ
    Resdesc_.Height = height;
    Resdesc_.DepthOrArraySize = DepthOrArraySize;
    Resdesc_.MipLevels = MipLevels;
    Resdesc_.SampleDesc.Count = SampleDescCount;
    Resdesc_.Layout = layput;

}
