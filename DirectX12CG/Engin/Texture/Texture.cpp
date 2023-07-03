#include "Texture.h"
#include "MCBFloat.h"
#include <assert.h>

using namespace MCB;
using namespace DirectX;



MCB::Texture::~Texture()
{

}

void MCB::Texture::CreateTexture(const wchar_t* FileName, uint16_t incrementNum)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* srv = ShaderResource::GetInstance();

    HRESULT result = texfile_.LoadTexture(FileName, WIC_FLAGS_NONE);

    assert(SUCCEEDED(result) && "テクスチャ読み込みエラー");
 /*   if (!SUCCEEDED(result))
    {
        CreateNoTextureFileIsTexture();
        return;
    }*/

    result = mipMap_.GenerateMipMap(&texfile_, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(SUCCEEDED(result) && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
    texImg_.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    texBuff_.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff_.SetTexResourceDesc(texfile_, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff_.CommitResouce( D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff_.TransferMipmatToTexBuff(texfile_, nullptr, result);
    //-----------------------------------

    incrementNum_ = incrementNum;

    srvptr_ = srv;

    SetSrvHeap();
}



void MCB::Texture::CreateTexture(const std::string& directoryPath, const std::string& filename, uint16_t incrementNum)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* srv = ShaderResource::GetInstance();

    HRESULT result = texfile_.LoadTexture(directoryPath, filename);

    assert(SUCCEEDED(result) && "テクスチャ読み込みエラー");

    result = mipMap_.GenerateMipMap(&texfile_, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(SUCCEEDED(result) && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
    texImg_.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    texBuff_.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff_.SetTexResourceDesc(texfile_, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff_.CommitResouce(D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff_.TransferMipmatToTexBuff(texfile_, nullptr, result);
    //-----------------------------------

    incrementNum_ = incrementNum;

    srvptr_ = srv;

    SetSrvHeap();

}

void MCB::Texture::SetSrvHeap()
{
    if (srvptr_ == nullptr)
    {
        assert("SRVデスクリプタ作った後にsrvptrにポインタを渡してください。srvptrはnullptrでした。");
    }

    srvptr_->SetSrvHeap(incrementNum_);

    srvptr_->SetSrvDesc(texBuff_, D3D12_SRV_DIMENSION_TEXTURE2D);

    srvptr_->SetShaderResourceView(texBuff_);
}

void MCB::Texture::CreateTexture(const wchar_t* FileName)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* srv = ShaderResource::GetInstance();

    HRESULT result = texfile_.LoadTexture(FileName, WIC_FLAGS_NONE);

    assert(SUCCEEDED(result) && "テクスチャ読み込みエラー");

    result = mipMap_.GenerateMipMap(&texfile_, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(SUCCEEDED(result) && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
    texImg_.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    texBuff_.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff_.SetTexResourceDesc(texfile_, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff_.CommitResouce(D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff_.TransferMipmatToTexBuff(texfile_, nullptr, result);
    //-----------------------------------
    incrementNum_ = ShaderResource::sAllincrementNum_;

    ShaderResource::sAllincrementNum_++;
    srvptr_ = srv;

    SetSrvHeap();
}

void MCB::Texture::CreateTexture(const std::string& directoryPath, const std::string& filename)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* srv = ShaderResource::GetInstance();

    HRESULT result = texfile_.LoadTexture(directoryPath, filename);

    assert(SUCCEEDED(result) && "テクスチャ読み込みエラー");

    result = mipMap_.GenerateMipMap(&texfile_, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(SUCCEEDED(result) && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
    texImg_.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    texBuff_.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff_.SetTexResourceDesc(texfile_, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff_.CommitResouce( D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff_.TransferMipmatToTexBuff(texfile_, nullptr, result);
    //-----------------------------------

    incrementNum_ = ShaderResource::sAllincrementNum_;

    ShaderResource::sAllincrementNum_++;
    srvptr_ = srv;

    SetSrvHeap();
}

void MCB::Texture::CreateNoTextureFileIsTexture(bool postEffect)
{
    if (postEffect)
    {
        Dx12* dx12 = Dx12::GetInstance();
        ShaderResource* srv = ShaderResource::GetInstance();

        //画像イメージデータの作成----------------------
        texImg_.SetImageDataRGBA(Float4(1.0f, 1.0f, 1.0f, 1.0f));
        //------------------------------------

         //テクスチャバッファ設定---------------------------------------
        texBuff_.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
        texBuff_.SetNoTextureFileTexResourceDescForPostEffect();
        //--------------------------------------

        D3D12_CLEAR_VALUE val;
        val.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        val.Color[0] = 0.25f; val.Color[1] = 0.5f; val.Color[2] = 0.1f; val.Color[3] = 1.0f;
        //テクスチャバッファの生成----------------------
        HRESULT result = texBuff_.CommitResouce(D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &val);
        texBuff_.TransferMipmatToTexBuff(texImg_, result);
        //-----------------------------------
        incrementNum_ = ShaderResource::sAllincrementNum_;

        ShaderResource::sAllincrementNum_++;
        srvptr_ = srv;

        SetSrvHeap();
    }
    else
    {
        Dx12* dx12 = Dx12::GetInstance();
        ShaderResource* srv = ShaderResource::GetInstance();

        //画像イメージデータの作成----------------------
        texImg_.SetImageDataRGBA(Float4(1.0f, 1.0f, 1.0f, 1.0f));
        //------------------------------------

         //テクスチャバッファ設定---------------------------------------
        texBuff_.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
        texBuff_.SetNoTextureFileTexResourceDesc();
        //--------------------------------------


        //テクスチャバッファの生成----------------------
        HRESULT result = texBuff_.CommitResouce(D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
        texBuff_.TransferMipmatToTexBuff(texImg_, result);
        //-----------------------------------
        incrementNum_ = ShaderResource::sAllincrementNum_;

        ShaderResource::sAllincrementNum_++;
        srvptr_ = srv;

        SetSrvHeap();
    }
}


void MCB::Texture::CreateNoTextureFileIsTexture(uint16_t incrementNum, bool postEffect)
{
    if (postEffect)
    {
        Dx12* dx12 = Dx12::GetInstance();
        ShaderResource* srv = ShaderResource::GetInstance();


         //テクスチャバッファ設定---------------------------------------
        texBuff_.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
        texBuff_.SetNoTextureFileTexResourceDescForPostEffect();
        //--------------------------------------

        D3D12_CLEAR_VALUE val;
        val.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        val.Color[0] = 0.25f; val.Color[1] = 0.5f; val.Color[2] = 0.1f; val.Color[3] = 1.0f;
        //テクスチャバッファの生成----------------------
        HRESULT result = texBuff_.CommitResouce(D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &val);
        texBuff_.TransferMipmatToTexBuff(result);
        //-----------------------------------
        incrementNum_ = ShaderResource::sAllincrementNum_;

        incrementNum_ = incrementNum;
        srvptr_ = srv;

        SetSrvHeap();
    }
    else
    {
        Dx12* dx12 = Dx12::GetInstance();
        ShaderResource* srv = ShaderResource::GetInstance();

        //画像イメージデータの作成----------------------
        texImg_.SetImageDataRGBA(Float4(1.0f, 1.0f, 1.0f, 1.0f));
        //------------------------------------

         //テクスチャバッファ設定---------------------------------------
        texBuff_.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
        texBuff_.SetNoTextureFileTexResourceDesc();
        //--------------------------------------


        //テクスチャバッファの生成----------------------
        HRESULT result = texBuff_.CommitResouce(D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
        texBuff_.TransferMipmatToTexBuff(texImg_, result);
        //-----------------------------------
        incrementNum_ = ShaderResource::sAllincrementNum_;

        incrementNum_ = incrementNum;
        srvptr_ = srv;

        SetSrvHeap();
    }

}
