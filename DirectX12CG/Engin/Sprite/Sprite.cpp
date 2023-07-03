#include "Sprite.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

DirectX::XMMATRIX Sprite::smatProje_{};

MCB::Sprite::Sprite()
{
    CreateSprite();
}

void MCB::Sprite::SpriteTransferVertexBuffer(Texture* tex)
{
    HRESULT result = S_FALSE;

    SpriteVertex vertices[] =
    {
        {{},{0.0f,1.0f}},
        {{},{0.0f,0.0f}},
        {{},{1.0f,1.0f}},
        {{},{1.0f,0.0f}},
    };

    float left = (0.0f - anchorPoint_.x_) * size_.x_;
    float right = (1.0f - anchorPoint_.x_) * size_.x_;
    float top = (0.0f - anchorPoint_.y_) * size_.y_;
    float bottom = (1.0f - anchorPoint_.y_) * size_.y_;


    enum { LB, LT, RB, RT };

    if (isFlipX_)
    {
        left = -left;
        right = -right;
    }

    if (isFlipY_)
    {
        top = -top;
        bottom = -bottom;
    }

    vertices[LB].pos = { left,bottom,0.0f };
    vertices[LT].pos = { left,top,0.0f };
    vertices[RB].pos = { right,bottom,0.0f };
    vertices[RT].pos = { right,top,0.0f };

    if (tex != nullptr)
    {
        D3D12_RESOURCE_DESC resDesc = tex->texBuff_.texbuff_->GetDesc();

        float tex_left = texLeftTop_.x_ / resDesc.Width;
        float tex_right = (texLeftTop_.x_ + cuttingSize_.x_) / resDesc.Width;
        float tex_top = texLeftTop_.y_ / resDesc.Height;
        float tex_bottom = (texLeftTop_.y_ + cuttingSize_.y_) / resDesc.Height;

        vertices[LB].uv = { tex_left,tex_bottom };
        vertices[LT].uv = { tex_left,tex_top };
        vertices[RB].uv = { tex_right,tex_bottom };
        vertices[RT].uv = { tex_right,tex_top };

    }

    

    SpriteVertex* vertexMap = nullptr;
    result = vertBuff_->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "SpriteTransferVertexBuffer時のvertBuff->Mapエラー");
    memcpy(vertexMap, vertices, sizeof(vertices));
    vertBuff_->Unmap(0, nullptr);

}

void MCB::Sprite::SpriteUpdate()
{
    HRESULT result = S_FALSE;
    matWorld_ = DirectX::XMMatrixIdentity();
    matWorld_ *= DirectX::XMMatrixRotationZ(ConvertRadius(rotation_));
    matWorld_ *= XMMatrixTranslation(position_.x_, position_.y_, position_.z_);

    SpriteConstBufferDataTransform* constMap = nullptr;
    result = constBuff_->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "SpriteUpdate時のconstBuff->Mapエラー");
    constMap->color = color_;
    constMap->mat = matWorld_ * Sprite::smatProje_;

    constBuff_->Unmap(0, nullptr);

}

void MCB::Sprite::InitMatProje()
{
    DxWindow* dxWindow = DxWindow::GetInstance();
    Sprite::smatProje_ = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f, (float)dxWindow->sWINDOW_WIDTH_, (float)dxWindow->sWINDOW_HEIGHT_, 0.0f, 0.0f, 1.0f);

}

void MCB::Sprite::CreateSprite()
{
    Dx12* dx12 = Dx12::GetInstance();

    HRESULT result = S_FALSE;


    SpriteVertex vertices[] =
    {
        {{0.0f,100.0f,0.0f},{0.0f,1.0f}},
        {{0.0f ,0.0f ,0.0f},{0.0f,0.0f}},
        {{100.0f,100.0f,0.0f},{1.0f,1.0f}},
        {{100.0f,0.0f, 0.0f},{1.0f,0.0f}}
    };

    HeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;


    Resdesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    Resdesc_.Width = (sizeof(vertices));
    Resdesc_.Height = 1;
    Resdesc_.DepthOrArraySize = 1;
    Resdesc_.MipLevels = 1;
    Resdesc_.SampleDesc.Count = 1;
    Resdesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    result = dx12->device_.Get()->CreateCommittedResource(
        &HeapProp_,
        D3D12_HEAP_FLAG_NONE,
        &Resdesc_,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff_));

    assert(SUCCEEDED(result) && "Sprite生成時の頂点バッファCommittedResourceエラー");

    float left = (0.0f - anchorPoint_.x_) * size_.x_;
    float right = (1.0f - anchorPoint_.x_) * size_.x_;
    float top = (0.0f - anchorPoint_.y_) * size_.y_;
    float bottom = (1.0f - anchorPoint_.y_) * size_.y_;


    enum { LB, LT, RB, RT };

    vertices[LB].pos = { left,bottom,0.0f };
    vertices[LT].pos = { left,top,0.0f };
    vertices[RB].pos = { right,bottom,0.0f };
    vertices[RT].pos = { right,top,0.0f };


    SpriteVertex* vertexMap = nullptr;
    result = vertBuff_->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "Sprite生成時のvertBuff->Mapエラー");
    memcpy(vertexMap, vertices, sizeof(vertices));
    vertBuff_->Unmap(0, nullptr);

    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = sizeof(vertices);
    vbView_.StrideInBytes = sizeof(vertices[0]);

    D3D12_HEAP_PROPERTIES constHeapProp{};
    constHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC constResdesc{};
    constResdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    constResdesc.Width = (sizeof(SpriteConstBufferDataTransform) + 0xff) & ~0xff;
    constResdesc.Height = 1;
    constResdesc.DepthOrArraySize = 1;
    constResdesc.MipLevels = 1;
    constResdesc.SampleDesc.Count = 1;
    constResdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


    result = dx12->device_.Get()->CreateCommittedResource(
        &constHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &constResdesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff_)
    );

    assert(SUCCEEDED(result) && "Sprite生成時の定数バッファのCommittedResourceエラー");
    SpriteConstBufferDataTransform* constMap = nullptr;
    result = constBuff_->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "Sprite生成時のconstBuff->Mapエラー");
    Float4 tempcolor; tempcolor.x_ = 1; tempcolor.y_ = 1;tempcolor.z_ = 1;tempcolor.w_ = 1;
    constMap->color = tempcolor;
    constMap->mat = Sprite::smatProje_;
    constBuff_->Unmap(0,nullptr);
}

//void MCB::Sprite::SpriteCommonBeginDraw(const PipelineRootSignature& pipeline)
//{
//    Dx12* dx12 = Dx12::GetInstance();
//    dx12->commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
//    dx12->commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());
//
//
//    //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
//    dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//    //SRVヒープの設定コマンド
//    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { ShaderResource::GetInstance()->srvHeap};
//    dx12->commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
//}


void MCB::Sprite::SpriteDraw(Texture& tex, float positionX, float positionY,
                            float size_x, float size_y)
{
    if (&tex == nullptr) return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    Sprite tempsprite = *this;

    Float2 size;

    size.x_ = size_x;
    size.y_ = size_y;

    tempsprite.position_.x_ = positionX;
    tempsprite.position_.y_ = positionY;
    tempsprite.position_.z_ = 0;

    tempsprite.SpriteUpdate();

    if (size.x_ == 0 || size.y_ == 0)
    {
        D3D12_RESOURCE_DESC resdesc = tex.texBuff_.texbuff_->GetDesc();

        if (size.x_ == 0)
        {
            tempsprite.size_.x_ = (float)resdesc.Width;
        }

        if (size.y_ == 0)
        {
            tempsprite.size_.y_ = (float)resdesc.Height;
        }
    }
    
    if(size.x_ != 0 || size.y_ != 0)
    {
        if (size.x_ != 0)
        {
            tempsprite.size_.x_ = size.x_;
        }

        if (size.y_ != 0)
        {
            tempsprite.size_.y_ = size.y_;
        }
    }



    if (tempsprite.size_.x_ != size_.x_ || tempsprite.size_.y_ != size_.y_)
    {
        tempsprite.SpriteTransferVertexBuffer();
        size_ = tempsprite.size_;
    }

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList_->IASetVertexBuffers(0, 1, &vbView_);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList_->SetGraphicsRootConstantBufferView(0, tempsprite.constBuff_->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList_->DrawInstanced(4,1,0,0);

}

void MCB::Sprite::SpriteFlipDraw(Texture& tex, float positionX, float positionY, bool isflipX, bool isflipY)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    Sprite tempSprite = *this;

    tempSprite.position_.x_ = positionX;
    tempSprite.position_.y_ = positionY;
    tempSprite.position_.z_ = 0;

    tempSprite.SpriteUpdate();

    tempSprite.isFlipX_ = isflipX;
    tempSprite.isFlipY_ = isflipY;

    D3D12_RESOURCE_DESC resdesc = tex.texBuff_.texbuff_->GetDesc();

    tempSprite.size_.x_ = (float)resdesc.Width;
    tempSprite.size_.y_ = (float)resdesc.Height;

    tempSprite.SpriteTransferVertexBuffer();

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList_->IASetVertexBuffers(0, 1, &vbView_);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList_->DrawInstanced(4, 1, 0, 0);

}

void MCB::Sprite::SpriteCuttingDraw( Texture& tex, float positionX, float positionY,
    const Float2& cuttingsize,const Float2& CuttingLeftTop)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();
    Sprite tempSprite = *this;

    tempSprite.position_.x_ = positionX;
    tempSprite.position_.y_ = positionY;
    tempSprite.position_.z_ = 0;
    tempSprite.texLeftTop_ = CuttingLeftTop;
    tempSprite.cuttingSize_ = cuttingsize;

    tempSprite.SpriteUpdate();
    tempSprite.SpriteTransferVertexBuffer(&tex);

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList_->IASetVertexBuffers(0, 1, &vbView_);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList_->DrawInstanced(4, 1, 0, 0);
}


