#include "Descriptor.h"
#include <algorithm>
using namespace MCB;

uint16_t MCB::ShaderResource::AllincrementNum = 0;

void MCB::ShaderResource::Init()
{
    SetHeapDesc(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
    Dx12::GetInstance()->result = SetDescriptorHeap();
    SetDescriptorRange(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0);
    SetDescriptorRange(2, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,1);
}

void MCB::ShaderResource::SetHeapDesc(D3D12_DESCRIPTOR_HEAP_FLAGS flags)
{
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = flags; //シェーダーから見えるように
    srvHeapDesc.NumDescriptors = (uint32_t)MaxSRVCount;//定数バッファの数
}

HRESULT MCB::ShaderResource::SetDescriptorHeap()
{
    return Dx12::GetInstance()->device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
}

void MCB::ShaderResource::SetShaderResourceView(TextureBuffer& texBuffer)
{
    //ヒープの二番目にシェーダーリソースビュー作成
    Dx12::GetInstance()->device->CreateShaderResourceView(texBuffer.texbuff.Get(), &srvDesc, srvHandle);
}

void MCB::ShaderResource::SetDescriptorRange(const int32_t& NumDescriptors, const D3D12_DESCRIPTOR_RANGE_TYPE& type, const int32_t& BaseShaderRegister, const size_t& index)
{
    size_t i = min(static_cast<size_t>(descriptorRange.size()) - 1, index);
    descriptorRange[i].NumDescriptors = NumDescriptors;
    descriptorRange[i].RangeType = type;
    descriptorRange[i].BaseShaderRegister = BaseShaderRegister;
    descriptorRange[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void MCB::ShaderResource::SetSrvDesc(TextureBuffer &texBuffer, D3D12_SRV_DIMENSION srvDimension)
{
    srvDesc.Format = texBuffer.texresDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = srvDimension;
    srvDesc.Texture2D.MipLevels = texBuffer.texresDesc.MipLevels;
}

void MCB::ShaderResource::SetSrvHeap()
{
    srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
    
}

void MCB::ShaderResource::SetSrvHeap(uint16_t incrementNum)
{
    srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
    srvHandle.ptr += incrementNum * Dx12::GetInstance()->device.Get()->GetDescriptorHandleIncrementSize(srvHeapDesc.Type);
}

void MCB::ShaderResource::InitAllincrementNum()
{
    ShaderResource::AllincrementNum = 0;
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
