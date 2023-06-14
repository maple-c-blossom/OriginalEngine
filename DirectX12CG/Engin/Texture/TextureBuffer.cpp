#include "TextureBuffer.h"
#include <d3dx12.h>
#include <memory>
#include <array>
using namespace DirectX;
using namespace std;
HRESULT MCB::TextureBuffer::CommitResouce(const D3D12_HEAP_FLAGS& flags,const D3D12_RESOURCE_STATES& resouceState, const D3D12_CLEAR_VALUE* clearValue)
{
    return Dx12::GetInstance()->device_->CreateCommittedResource(&texHeapProp_, flags, &texresDesc_, resouceState, clearValue, IID_PPV_ARGS(&texbuff_));
}

void MCB::TextureBuffer::TransferMipmatToTexBuff(const TextureFile &texFile, D3D12_BOX* DsrBox, HRESULT &result)
{
    for (size_t i = 0; i < texFile.metadata_.mipLevels; i++)
    {
        //ミップマップレベルを指定してイメージを取得
        const Image* img = texFile.scratchImg_.GetImage(i, 0, 0);
        //テクスチャバッファにデータ転送
        result = texbuff_->WriteToSubresource((uint32_t)i, DsrBox, img->pixels, (uint32_t)img->rowPitch, (uint32_t)img->slicePitch);
        assert(SUCCEEDED(result));
    }
}

void MCB::TextureBuffer::SetTexHeapProp(const D3D12_HEAP_TYPE& heaptype,
    const D3D12_CPU_PAGE_PROPERTY& cpuPagePropety,const D3D12_MEMORY_POOL& memorypool)
{
    texHeapProp_.Type = heaptype;
    texHeapProp_.CPUPageProperty = cpuPagePropety;
    texHeapProp_.MemoryPoolPreference = memorypool;
    
    //texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

}

void MCB::TextureBuffer::SetTexResourceDesc(TextureFile &texFile,const D3D12_RESOURCE_DIMENSION& resouceDimenSion, int32_t SampleDescCount)
{
    texresDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texresDesc_.Format = texFile.metadata_.format;
    texresDesc_.Width = texFile.metadata_.width;
    texresDesc_.Height = (uint32_t)texFile.metadata_.height;
    texresDesc_.DepthOrArraySize = (uint16_t)texFile.metadata_.arraySize;
    texresDesc_.MipLevels = (uint16_t)texFile.metadata_.mipLevels;
    texresDesc_.SampleDesc.Count = SampleDescCount;
}

void MCB::TextureBuffer::SetNoTextureFileTexResourceDesc()
{
    texresDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texresDesc_.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texresDesc_.Width = 256;
    texresDesc_.Height = 256;
    texresDesc_.DepthOrArraySize = 1;
    texresDesc_.MipLevels = 1;
    texresDesc_.SampleDesc.Count = 1;
}

void MCB::TextureBuffer::SetNoTextureFileTexResourceDescForPostEffect()
{
    texresDesc_ = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        (uint32_t)DxWindow::sWINDOW_WIDTH_,
        (uint32_t)DxWindow::sWINDOW_HEIGHT_,
        1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
    );
}


void MCB::TextureBuffer::TransferMipmatToTexBuff(const TexImgData& teximg,HRESULT& result)
{
    //テクスチャバッファにデータ転送
    result = texbuff_->WriteToSubresource(0, nullptr, &teximg.imageData_[0], 
        sizeof(Float4) * (uint32_t)teximg.textureWidth_, sizeof(Float4) * (uint32_t)teximg.imageDataCount_);
    assert(SUCCEEDED(result));
}


void MCB::TextureBuffer::TransferMipmatToTexBuff(HRESULT& result)
{
    const uint32_t pixelCount = DxWindow::sWINDOW_WIDTH_ * DxWindow::sWINDOW_HEIGHT_;
    const uint32_t rowPitch = sizeof(uint32_t) * DxWindow::sWINDOW_WIDTH_;
    const uint32_t depthPitch = rowPitch * DxWindow::sWINDOW_HEIGHT_;
    unique_ptr<std::array<uint32_t, pixelCount>> imgs = make_unique<std::array<uint32_t, pixelCount>>();;
    for (auto& itr : *imgs.get())
    {
        itr = 0x00EEEEff;
    }

    result = texbuff_->WriteToSubresource(0, nullptr, imgs.get(), rowPitch, depthPitch);
    assert(SUCCEEDED(result) && "PostEffectImgError");
}