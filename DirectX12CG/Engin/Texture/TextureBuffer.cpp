#include "TextureBuffer.h"
#include <d3dx12.h>
#include <memory>
#include <array>
using namespace DirectX;
using namespace std;
HRESULT MCB::TextureBuffer::CommitResouce(D3D12_HEAP_FLAGS flags, D3D12_RESOURCE_STATES resouceState, const D3D12_CLEAR_VALUE* clearValue)
{
    return Dx12::GetInstance()->device->CreateCommittedResource(&texHeapProp, flags, &texresDesc, resouceState, clearValue, IID_PPV_ARGS(&texbuff));
}

void MCB::TextureBuffer::TransferMipmatToTexBuff(const TextureFile &texFile, D3D12_BOX* DsrBox, HRESULT &result)
{
    for (size_t i = 0; i < texFile.metadata.mipLevels; i++)
    {
        //ミップマップレベルを指定してイメージを取得
        const Image* img = texFile.scratchImg.GetImage(i, 0, 0);
        //テクスチャバッファにデータ転送
        result = texbuff->WriteToSubresource((UINT)i, DsrBox, img->pixels, (UINT)img->rowPitch, (UINT)img->slicePitch);
        assert(SUCCEEDED(result));
    }
}

void MCB::TextureBuffer::SetTexHeapProp(D3D12_HEAP_TYPE heaptype, D3D12_CPU_PAGE_PROPERTY cpuPagePropety, D3D12_MEMORY_POOL memorypool)
{
    texHeapProp.Type = heaptype;
    texHeapProp.CPUPageProperty = cpuPagePropety;
    texHeapProp.MemoryPoolPreference = memorypool;
    
    //texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

}

void MCB::TextureBuffer::SetTexResourceDesc(TextureFile &texFile, D3D12_RESOURCE_DIMENSION resouceDimenSion, int SampleDescCount)
{
    texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texresDesc.Format = texFile.metadata.format;
    texresDesc.Width = texFile.metadata.width;
    texresDesc.Height = (UINT)texFile.metadata.height;
    texresDesc.DepthOrArraySize = (UINT16)texFile.metadata.arraySize;
    texresDesc.MipLevels = (UINT16)texFile.metadata.mipLevels;
    texresDesc.SampleDesc.Count = SampleDescCount;
}

void MCB::TextureBuffer::SetNoTextureFileTexResourceDesc()
{
    texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texresDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texresDesc.Width = 256;
    texresDesc.Height = 256;
    texresDesc.DepthOrArraySize = 1;
    texresDesc.MipLevels = 1;
    texresDesc.SampleDesc.Count = 1;
}

void MCB::TextureBuffer::SetNoTextureFileTexResourceDescForPostEffect()
{
    texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        (UINT)DxWindow::window_width,
        (UINT)DxWindow::window_height,
        1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
    );
}


void MCB::TextureBuffer::TransferMipmatToTexBuff(TexImgData teximg,HRESULT& result)
{
    //テクスチャバッファにデータ転送
    result = texbuff->WriteToSubresource(0, nullptr, &teximg.imageData[0], sizeof(Float4) * (UINT)teximg.textureWidth, sizeof(Float4) * (UINT)teximg.imageDataCount);
    assert(SUCCEEDED(result));
}


void MCB::TextureBuffer::TransferMipmatToTexBuff(HRESULT& result)
{
    const UINT pixelCount = DxWindow::window_width * DxWindow::window_height;
    const UINT rowPitch = sizeof(UINT) * DxWindow::window_width;
    const UINT depthPitch = rowPitch * DxWindow::window_height;
    unique_ptr<std::array<UINT, pixelCount>> imgs = make_unique<std::array<UINT, pixelCount>>();;
    for (auto& itr : *imgs.get())
    {
        itr = 0xff0000ff;
    }

    result = texbuff->WriteToSubresource(0, nullptr, imgs.get(), rowPitch, depthPitch);
    assert(SUCCEEDED(result) && "PostEffectImgError");
}