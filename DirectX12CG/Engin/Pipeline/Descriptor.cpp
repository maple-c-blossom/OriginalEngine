#include "Descriptor.h"

using namespace MCB;

unsigned short int MCB::ShaderResource::AllincrementNum = 0;

void MCB::ShaderResource::Init()
{
    SetHeapDesc(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
    Dx12::GetInstance()->result = SetDescriptorHeap();
    SetDescriptorRange(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0);
}

void MCB::ShaderResource::SetHeapDesc(D3D12_DESCRIPTOR_HEAP_FLAGS flags)
{
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = flags; //シェーダーから見えるように
    srvHeapDesc.NumDescriptors = MaxSRVCount;//定数バッファの数
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

void MCB::ShaderResource::SetDescriptorRange(int NumDescriptors, D3D12_DESCRIPTOR_RANGE_TYPE type, int BaseShaderRegister)
{
    descriptorRange.NumDescriptors = NumDescriptors;
    descriptorRange.RangeType = type;
    descriptorRange.BaseShaderRegister = BaseShaderRegister;
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
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

void MCB::ShaderResource::SetSrvHeap(unsigned short int incrementNum)
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
    static ShaderResource* instance = new ShaderResource;
    return instance;
}

void MCB::ShaderResource::DeleteInstace()
{
    delete ShaderResource::GetInstance();
}

ShaderResource* MCB::ShaderResource::GetInitInstance()
{
    ShaderResource* instance = ShaderResource::GetInstance();
    instance->Init();
    return instance;
}
