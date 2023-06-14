#include "Descriptor.h"
#include <algorithm>
using namespace MCB;

uint16_t MCB::ShaderResource::sAllincrementNum_ = 0;

void MCB::ShaderResource::Init()
{
    SetHeapDesc(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
    Dx12::GetInstance()->result_ = SetDescriptorHeap();
    SetDescriptorRange(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0);
    SetDescriptorRange(2, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,1);
}

void MCB::ShaderResource::SetHeapDesc(const D3D12_DESCRIPTOR_HEAP_FLAGS& flags)
{
    srvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc_.Flags = flags; //シェーダーから見えるように
    srvHeapDesc_.NumDescriptors = (uint32_t)MAX_SRV_COUNT_;//定数バッファの数
}

HRESULT MCB::ShaderResource::SetDescriptorHeap()
{
    return Dx12::GetInstance()->device_->CreateDescriptorHeap(&srvHeapDesc_, IID_PPV_ARGS(&srvHeap_));
}

void MCB::ShaderResource::SetShaderResourceView(TextureBuffer& texBuffer)
{
    //ヒープの二番目にシェーダーリソースビュー作成
    Dx12::GetInstance()->device_->CreateShaderResourceView(texBuffer.texbuff_.Get(), &srvDesc_, srvHandle_);
}

void MCB::ShaderResource::SetDescriptorRange( int32_t NumDescriptors, const D3D12_DESCRIPTOR_RANGE_TYPE& type, 
     int32_t BaseShaderRegister,  size_t index)
{
    size_t i = min(static_cast<size_t>(descriptorRange_.size()) - 1, index);
    descriptorRange_[i].NumDescriptors = NumDescriptors;
    descriptorRange_[i].RangeType = type;
    descriptorRange_[i].BaseShaderRegister = BaseShaderRegister;
    descriptorRange_[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void MCB::ShaderResource::SetSrvDesc(TextureBuffer &texBuffer, const D3D12_SRV_DIMENSION& srvDimension)
{
    srvDesc_.Format = texBuffer.texresDesc_.Format;
    srvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc_.ViewDimension = srvDimension;
    srvDesc_.Texture2D.MipLevels = texBuffer.texresDesc_.MipLevels;
}

void MCB::ShaderResource::SetSrvHeap()
{
    srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();
    
}

void MCB::ShaderResource::SetSrvHeap( uint16_t incrementNum)
{
    srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();
    srvHandle_.ptr += incrementNum * Dx12::GetInstance()->device_.Get()->GetDescriptorHandleIncrementSize(srvHeapDesc_.Type);
}

void MCB::ShaderResource::InitAllincrementNum()
{
    ShaderResource::sAllincrementNum_ = 0;
}


ShaderResource* MCB::ShaderResource::GetInstance()
{
    static ShaderResource instance;
    return &instance;
}



ShaderResource* MCB::ShaderResource::GetInitInstance()
{
    ShaderResource* instance = ShaderResource::GetInstance();
    instance->Init();
    return instance;
}
