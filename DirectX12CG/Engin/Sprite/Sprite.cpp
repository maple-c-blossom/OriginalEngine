#include "Sprite.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

DirectX::XMMATRIX Sprite::matProje{};

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

    float left = (0.0f - anchorPoint.x) * size.x;
    float right = (1.0f - anchorPoint.x) * size.x;
    float top = (0.0f - anchorPoint.y) * size.y;
    float bottom = (1.0f - anchorPoint.y) * size.y;


    enum { LB, LT, RB, RT };

    if (isFlipX)
    {
        left = -left;
        right = -right;
    }

    if (isFlipY)
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
        D3D12_RESOURCE_DESC resDesc = tex->texBuff.texbuff->GetDesc();

        float tex_left = texLeftTop.x / resDesc.Width;
        float tex_right = (texLeftTop.x + cuttingSize.x) / resDesc.Width;
        float tex_top = texLeftTop.y / resDesc.Height;
        float tex_bottom = (texLeftTop.y + cuttingSize.y) / resDesc.Height;

        vertices[LB].uv = { tex_left,tex_bottom };
        vertices[LT].uv = { tex_left,tex_top };
        vertices[RB].uv = { tex_right,tex_bottom };
        vertices[RT].uv = { tex_right,tex_top };

    }

    

    SpriteVertex* vertexMap = nullptr;
    result = vertBuff->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "SpriteTransferVertexBuffer時のvertBuff->Mapエラー");
    memcpy(vertexMap, vertices, sizeof(vertices));
    vertBuff->Unmap(0, nullptr);

}

void MCB::Sprite::SpriteUpdate()
{
    HRESULT result = S_FALSE;
    matWorld = DirectX::XMMatrixIdentity();
    matWorld *= DirectX::XMMatrixRotationZ(ConvertRadius(rotation));
    matWorld *= XMMatrixTranslation(position.x, position.y, position.z);

    SpriteConstBufferDataTransform* constMap = nullptr;
    result = constBuff->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "SpriteUpdate時のconstBuff->Mapエラー");
    constMap->color = color;
    constMap->mat = matWorld * Sprite::matProje;

    constBuff->Unmap(0, nullptr);

}

void MCB::Sprite::InitMatProje()
{
    DxWindow* dxWindow = DxWindow::GetInstance();
    Sprite::matProje = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f, (float)dxWindow->window_width, (float)dxWindow->window_height, 0.0f, 0.0f, 1.0f);

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

    this->HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;


    this->Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    this->Resdesc.Width = (sizeof(vertices));
    this->Resdesc.Height = 1;
    this->Resdesc.DepthOrArraySize = 1;
    this->Resdesc.MipLevels = 1;
    this->Resdesc.SampleDesc.Count = 1;
    this->Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    result = dx12->device.Get()->CreateCommittedResource(
        &this->HeapProp,
        D3D12_HEAP_FLAG_NONE,
        &this->Resdesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&this->vertBuff));

    assert(SUCCEEDED(result) && "Sprite生成時の頂点バッファCommittedResourceエラー");

    float left = (0.0f - this->anchorPoint.x) * this->size.x;
    float right = (1.0f - this->anchorPoint.x) * this->size.x;
    float top = (0.0f - this->anchorPoint.y) * this->size.y;
    float bottom = (1.0f - this->anchorPoint.y) * this->size.y;


    enum { LB, LT, RB, RT };

    vertices[LB].pos = { left,bottom,0.0f };
    vertices[LT].pos = { left,top,0.0f };
    vertices[RB].pos = { right,bottom,0.0f };
    vertices[RT].pos = { right,top,0.0f };


    SpriteVertex* vertexMap = nullptr;
    result = this->vertBuff->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "Sprite生成時のvertBuff->Mapエラー");
    memcpy(vertexMap, vertices, sizeof(vertices));
    this->vertBuff->Unmap(0, nullptr);

    this->vbView.BufferLocation = this->vertBuff->GetGPUVirtualAddress();
    this->vbView.SizeInBytes = sizeof(vertices);
    this->vbView.StrideInBytes = sizeof(vertices[0]);

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


    result = dx12->device.Get()->CreateCommittedResource(
        &constHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &constResdesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&this->constBuff)
    );

    assert(SUCCEEDED(result) && "Sprite生成時の定数バッファのCommittedResourceエラー");
    SpriteConstBufferDataTransform* constMap = nullptr;
    result = this->constBuff->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "Sprite生成時のconstBuff->Mapエラー");
    Float4 tempcolor; tempcolor.x = 1; tempcolor.y = 1;tempcolor.z = 1;tempcolor.w = 1;
    constMap->color = tempcolor;
    constMap->mat = Sprite::matProje;
    this->constBuff->Unmap(0,nullptr);
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

    size.x = size_x;
    size.y = size_y;

    tempsprite.position.x = positionX;
    tempsprite.position.y = positionY;
    tempsprite.position.z = 0;

    tempsprite.SpriteUpdate();

    if (size.x == 0 || size.y == 0)
    {
        D3D12_RESOURCE_DESC resdesc = tex.texBuff.texbuff->GetDesc();

        if (size.x == 0)
        {
            tempsprite.size.x = (float)resdesc.Width;
        }

        if (size.y == 0)
        {
            tempsprite.size.y = (float)resdesc.Height;
        }
    }
    
    if(size.x != 0 || size.y != 0)
    {
        if (size.x != 0)
        {
            tempsprite.size.x = size.x;
        }

        if (size.y != 0)
        {
            tempsprite.size.y = size.y;
        }
    }



    if (tempsprite.size.x != this->size.x || tempsprite.size.y != this->size.y)
    {
        tempsprite.SpriteTransferVertexBuffer();
        this->size = tempsprite.size;
    }

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList->SetGraphicsRootConstantBufferView(0, tempsprite.constBuff->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList->DrawInstanced(4,1,0,0);

}

void MCB::Sprite::SpriteFlipDraw(Texture& tex, float positionX, float positionY, bool isflipX, bool isflipY)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    Sprite tempSprite = *this;

    tempSprite.position.x = positionX;
    tempSprite.position.y = positionY;
    tempSprite.position.z = 0;

    tempSprite.SpriteUpdate();

    tempSprite.isFlipX = isflipX;
    tempSprite.isFlipY = isflipY;

    D3D12_RESOURCE_DESC resdesc = tex.texBuff.texbuff->GetDesc();

    tempSprite.size.x = (float)resdesc.Width;
    tempSprite.size.y = (float)resdesc.Height;

    tempSprite.SpriteTransferVertexBuffer();

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList->DrawInstanced(4, 1, 0, 0);

}

void MCB::Sprite::SpriteCuttingDraw( Texture& tex, float positionX, float positionY, Float2 cuttingsize, Float2 CuttingLeftTop)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();
    Sprite tempSprite = *this;

    tempSprite.position.x = positionX;
    tempSprite.position.y = positionY;
    tempSprite.position.z = 0;
    tempSprite.texLeftTop = CuttingLeftTop;
    tempSprite.cuttingSize = cuttingsize;

    tempSprite.SpriteUpdate();
    tempSprite.SpriteTransferVertexBuffer(&tex);

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList->DrawInstanced(4, 1, 0, 0);
}


