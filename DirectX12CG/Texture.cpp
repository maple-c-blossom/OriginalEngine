#include "Texture.h"
#include "MCBFloat.h"
#include <assert.h>

using namespace MCB;
using namespace DirectX;

void MCB::Texture::CreateTexture(Dx12 dx12, const wchar_t* FileName)
{
    HRESULT result = texfile.LoadTexture(FileName, WIC_FLAGS_NONE);

    result = mipMap.GenerateMipMap(&texfile, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(result && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
   texImg.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    TextureBuffer texBuff;
    texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff.SetTexResourceDesc(texfile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff.CommitResouce(dx12, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff.TransferMipmatToTexBuff(texfile, nullptr, result);
    //-----------------------------------
}

void MCB::Texture::CreateTexture(Dx12 dx12, const std::string& directoryPath, const std::string& filename)
{
    HRESULT result = texfile.LoadTexture(directoryPath, filename);

    result = mipMap.GenerateMipMap(&texfile, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(result && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
    texImg.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    TextureBuffer texBuff;
    texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff.SetTexResourceDesc(texfile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff.CommitResouce(dx12, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff.TransferMipmatToTexBuff(texfile, nullptr, result);
    //-----------------------------------
}
