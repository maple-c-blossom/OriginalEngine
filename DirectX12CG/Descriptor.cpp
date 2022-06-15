#include "Descriptor.h"

void MCB::Descriptor::SetHeapDesc(D3D12_DESCRIPTOR_HEAP_FLAGS flags)
{
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = flags; //シェーダーから見えるように
    srvHeapDesc.NumDescriptors = MaxSRVCount;//定数バッファの数
}

HRESULT MCB::Descriptor::SetDescriptorHeap(Dx12 &dx12)
{
    return dx12.device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
}

void MCB::Descriptor::SetShaderResourceView(Dx12& dx12, TextureBuffer& texBuffer)
{
    //ヒープの二番目にシェーダーリソースビュー作成
    dx12.device->CreateShaderResourceView(texBuffer.texbuff.Get(), &srvDesc, srvHandle);
}

void MCB::Descriptor::SetDescriptorRange(int NumDescriptors, D3D12_DESCRIPTOR_RANGE_TYPE type, int BaseShaderRegister)
{
    descriptorRange.NumDescriptors = NumDescriptors;
    descriptorRange.RangeType = type;
    descriptorRange.BaseShaderRegister = BaseShaderRegister;
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void MCB::Descriptor::SetSrvDesc(TextureBuffer &texBuffer, D3D12_SRV_DIMENSION srvDimension)
{
    srvDesc.Format = texBuffer.texresDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = srvDimension;
    srvDesc.Texture2D.MipLevels = texBuffer.texresDesc.MipLevels;
}

void MCB::Descriptor::SetSrvHeap()
{
    srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
    
}

void MCB::Descriptor::SetSrvHeap(unsigned short int incrementNum, Dx12 dx12)
{
    srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
    srvHandle.ptr += incrementNum * dx12.device.Get()->GetDescriptorHandleIncrementSize(srvHeapDesc.Type);
}
