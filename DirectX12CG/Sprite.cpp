#include "Sprite.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

DirectX::XMMATRIX Sprite::matProje{};

void MCB::Sprite::SpriteTransferVertexBuffer(const Sprite& sprite,Texture* tex)
{
    HRESULT result = S_FALSE;

    SpriteVertex vertices[] =
    {
        {{},{0.0f,1.0f}},
        {{},{0.0f,0.0f}},
        {{},{1.0f,1.0f}},
        {{},{1.0f,0.0f}},
    };

    float left = (0.0f - sprite.anchorPoint.x) * sprite.size.x;
    float right = (1.0f - sprite.anchorPoint.x) * sprite.size.x;
    float top = (0.0f - sprite.anchorPoint.y) * sprite.size.y;
    float bottom = (1.0f - sprite.anchorPoint.y) * sprite.size.y;


    enum { LB, LT, RB, RT };

    if (sprite.isFlipX)
    {
        left = -left;
        right = -right;
    }

    if (sprite.isFlipY)
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

        float tex_left = sprite.texLeftTop.x / resDesc.Width;
        float tex_right = (sprite.texLeftTop.x + sprite.cuttingSize.x) / resDesc.Width;
        float tex_top = sprite.texLeftTop.y / resDesc.Height;
        float tex_bottom = (sprite.texLeftTop.y + sprite.cuttingSize.y) / resDesc.Height;

        vertices[LB].uv = { tex_left,tex_bottom };
        vertices[LT].uv = { tex_left,tex_top };
        vertices[RB].uv = { tex_right,tex_bottom };
        vertices[RT].uv = { tex_right,tex_top };

    }

    

    SpriteVertex* vertexMap = nullptr;
    result = sprite.vertBuff->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "SpriteTransferVertexBuffer����vertBuff->Map�G���[");
    memcpy(vertexMap, vertices, sizeof(vertices));
    sprite.vertBuff->Unmap(0, nullptr);

}

void MCB::Sprite::SpriteUpdate(Sprite& sprite)
{
    HRESULT result = S_FALSE;
    sprite.matWorld = DirectX::XMMatrixIdentity();
    sprite.matWorld *= DirectX::XMMatrixRotationZ(ConvertRadius(sprite.rotation));
    sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);

    SpriteConstBufferDataTransform* constMap = nullptr;
    result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "SpriteUpdate����constBuff->Map�G���[");
    constMap->color = sprite.color;
    constMap->mat = sprite.matWorld * Sprite::matProje;

    sprite.constBuff->Unmap(0, nullptr);

}

void MCB::Sprite::InitMatProje()
{
    DxWindow* dxWindow = DxWindow::GetInstance();
    Sprite::matProje = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f, dxWindow->window_width, dxWindow->window_height, 0.0f, 0.0f, 1.0f);

}

MCB::Sprite MCB::Sprite::CreateSprite()
{
    Dx12* dx12 = Dx12::GetInstance();

    HRESULT result = S_FALSE;

    Sprite tempSprite = {};


    SpriteVertex vertices[] =
    {
        {{0.0f,100.0f,0.0f},{0.0f,1.0f}},
        {{0.0f ,0.0f ,0.0f},{0.0f,0.0f}},
        {{100.0f,100.0f,0.0f},{1.0f,1.0f}},
        {{100.0f,0.0f, 0.0f},{1.0f,0.0f}}
    };

    tempSprite.HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;


    tempSprite.Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    tempSprite.Resdesc.Width = (sizeof(vertices));
    tempSprite.Resdesc.Height = 1;
    tempSprite.Resdesc.DepthOrArraySize = 1;
    tempSprite.Resdesc.MipLevels = 1;
    tempSprite.Resdesc.SampleDesc.Count = 1;
    tempSprite.Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    result = dx12->device.Get()->CreateCommittedResource(
        &tempSprite.HeapProp,
        D3D12_HEAP_FLAG_NONE,
        &tempSprite.Resdesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&tempSprite.vertBuff));

    assert(SUCCEEDED(result) && "Sprite�������̒��_�o�b�t�@CommittedResource�G���[");

    float left = (0.0f - tempSprite.anchorPoint.x) * tempSprite.size.x;
    float right = (1.0f - tempSprite.anchorPoint.x) * tempSprite.size.x;
    float top = (0.0f - tempSprite.anchorPoint.y) * tempSprite.size.y;
    float bottom = (1.0f - tempSprite.anchorPoint.y) * tempSprite.size.y;


    enum { LB, LT, RB, RT };

    vertices[LB].pos = { left,bottom,0.0f };
    vertices[LT].pos = { left,top,0.0f };
    vertices[RB].pos = { right,bottom,0.0f };
    vertices[RT].pos = { right,top,0.0f };


    SpriteVertex* vertexMap = nullptr;
    result = tempSprite.vertBuff->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "Sprite��������vertBuff->Map�G���[");
    memcpy(vertexMap, vertices, sizeof(vertices));
    tempSprite.vertBuff->Unmap(0, nullptr);

    tempSprite.vbView.BufferLocation = tempSprite.vertBuff->GetGPUVirtualAddress();
    tempSprite.vbView.SizeInBytes = sizeof(vertices);
    tempSprite.vbView.StrideInBytes = sizeof(vertices[0]);

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
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&tempSprite.constBuff)
    );

    assert(SUCCEEDED(result) && "Sprite�������̒萔�o�b�t�@��CommittedResource�G���[");
    SpriteConstBufferDataTransform* constMap = nullptr;
    result = tempSprite.constBuff->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "Sprite��������constBuff->Map�G���[");
    Float4 tempcolor; tempcolor.x = 1; tempcolor.y = 1;tempcolor.z = 1;tempcolor.w = 1;
    constMap->color = tempcolor;
    constMap->mat = Sprite::matProje;
    tempSprite.constBuff->Unmap(0,nullptr);


    return tempSprite;
}

//void MCB::Sprite::SpriteCommonBeginDraw(const PipelineRootSignature& pipeline)
//{
//    Dx12* dx12 = Dx12::GetInstance();
//    dx12->commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
//    dx12->commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());
//
//
//    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
//    dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//    //SRV�q�[�v�̐ݒ�R�}���h
//    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { ShaderResource::GetInstance()->srvHeap};
//    dx12->commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
//}


void MCB::Sprite::SpriteDraw(Sprite& sprite,Texture& tex, float positionX, float positionY,
                            float size_x, float size_y)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    Sprite tempsprite = sprite;

    Float2 size;
    Float2 anchorPoint;

    size.x = size_x;
    size.y = size_y;

    tempsprite.position.x = positionX;
    tempsprite.position.y = positionY;
    tempsprite.position.z = 0;

    tempsprite.SpriteUpdate(tempsprite);

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



    if (tempsprite.size.x != sprite.size.x || tempsprite.size.y != sprite.size.y)
    {
        tempsprite.SpriteTransferVertexBuffer(tempsprite);
        sprite.size = tempsprite.size;
    }

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, tempsprite.constBuff->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12->commandList->DrawInstanced(4,1,0,0);

}

void MCB::Sprite::SpriteFlipDraw(Sprite& sprite, Texture& tex, float positionX, float positionY, bool isflipX, bool isflipY)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    Sprite tempSprite = sprite;

    tempSprite.position.x = positionX;
    tempSprite.position.y = positionY;
    tempSprite.position.z = 0;

    tempSprite.SpriteUpdate(tempSprite);

    tempSprite.isFlipX = isflipX;
    tempSprite.isFlipY = isflipY;

    D3D12_RESOURCE_DESC resdesc = tex.texBuff.texbuff->GetDesc();

    tempSprite.size.x = (float)resdesc.Width;
    tempSprite.size.y = (float)resdesc.Height;

    tempSprite.SpriteTransferVertexBuffer(tempSprite);

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12->commandList->DrawInstanced(4, 1, 0, 0);

}

void MCB::Sprite::SpriteCuttingDraw(Sprite& sprite, Texture& tex, float positionX, float positionY, Float2 cuttingsize, Float2 CuttingLeftTop)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();
    Sprite tempSprite = sprite;

    tempSprite.position.x = positionX;
    tempSprite.position.y = positionY;
    tempSprite.position.z = 0;
    tempSprite.texLeftTop = CuttingLeftTop;
    tempSprite.cuttingSize = cuttingsize;

    tempSprite.SpriteUpdate(tempSprite);
    tempSprite.SpriteTransferVertexBuffer(tempSprite,&tex);

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12->commandList->DrawInstanced(4, 1, 0, 0);
}
