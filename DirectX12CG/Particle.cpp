#include "Particle.h"

using namespace MCB;
using namespace std;
MCB::Particle::Particle()
{

}

MCB::Particle::~Particle()
{
    //delete vertex;
    if (constBuffTranceform == nullptr) return;
    constBuffTranceform->Unmap(0, nullptr);
    //vert.material.constBuffMaterialB1->Unmap(0, nullptr);
}

void MCB::Particle::SetColor(Float4 color)
{
    material.material.color = color;
    material.constMapMaterial->color = material.material.color;
}

void Particle::Init(TextureCell* tex)
{
    Dx12* dx12 = Dx12::GetInstance();
    D3D12_HEAP_PROPERTIES HeapProp{};
    HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC Resdesc{};
    Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    Resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
    Resdesc.Height = 1;
    Resdesc.DepthOrArraySize = 1;
    Resdesc.MipLevels = 1;
    Resdesc.SampleDesc.Count = 1;
    Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    dx12->result = dx12->device->CreateCommittedResource
    (
        &HeapProp,        //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &Resdesc,//���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffTranceform)
    );

    assert(SUCCEEDED(dx12->result));

    dx12->result = constBuffTranceform->Map(0, nullptr, (void**)&constMapTranceform);
    material.Init();
    this->tex = tex;
    Dx12::GetInstance()->result = Dx12::GetInstance()->device->CreateCommittedResource(
        &HeapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &Resdesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(Dx12::GetInstance()->result));

    sizeVB = static_cast<unsigned int>(sizeof(Vertex));
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeVB;
    vbView.StrideInBytes = sizeof(vertex);
}

void Particle::Update(View& view, Projection& projection, bool isBillBord)
{
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.SetMatRot(rotasion.x, rotasion.y, rotasion.z, false);
    matWorld.SetMatTrans(position.x, position.y, position.z);
    if (isBillBord)
    {
        if (parent == nullptr)
        {
            matWorld.UpdataBillBordMatrixWorld(view);
        }
        else
        {
            matWorld.UpdataMatrixWorld();
        }
    }
    else
    {
        matWorld.UpdataMatrixWorld();
    }

    if (parent != nullptr)
    {
        matWorld.matWorld *= parent->matWorld.matWorld;
    }

    //constMapTranceform->mat = matWorld.matWorld * view.mat * projection.mat;
    constMapTranceform->world = matWorld.matWorld * view.mat;
    constMapTranceform->viewproj = projection.mat;
    constMapTranceform->cameraPos.x = view.eye.x;
    constMapTranceform->cameraPos.y = view.eye.y;
    constMapTranceform->cameraPos.z = view.eye.z;
}

void Particle::Draw()
{
    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();


    ////�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(2,material.constBuffMaterialB1->GetGPUVirtualAddress());

    //lights->Draw(3);

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex->texture->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //�`��R�}���h
    dx12->commandList->DrawInstanced(vertNum, 1, 0, 0);
    //�C���f�b�N�X�f�[�^
    //dx12.commandList->IASetIndexBuffer(&model->ibView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    //�`��R�}���h
    //dx12.commandList->DrawInstanced(_countof(vert.vertices), 1, 0, 0);

}

