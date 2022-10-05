#include "Sprite.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

DirectX::XMMATRIX Sprite::matProje{};

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
    assert(SUCCEEDED(result) && "SpriteTransferVertexBuffer����vertBuff->Map�G���[");
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
    assert(SUCCEEDED(result) && "SpriteUpdate����constBuff->Map�G���[");
    constMap->color = color;
    constMap->mat = matWorld * Sprite::matProje;

    constBuff->Unmap(0, nullptr);

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


void MCB::Sprite::SpriteDraw(Texture& tex, float positionX, float positionY,
                            float size_x, float size_y)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    Sprite tempsprite = *this;

    Float2 size;
    Float2 anchorPoint;

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

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
    //�`��R�}���h
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

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12->commandList->DrawInstanced(4, 1, 0, 0);
}
