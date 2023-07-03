#include "Particle.h"

using namespace MCB;
using namespace std;
MCB::Particle::Particle()
{

}

MCB::Particle::~Particle()
{
    //delete vertex;
    if (constBuffTranceform_ == nullptr) return;
    constBuffTranceform_->Unmap(0, nullptr);
    //vert.material.constBuffMaterialB1->Unmap(0, nullptr);
}

void MCB::Particle::SetColor(const Float4& color)
{
    material_.material_.color = color;
    material_.constMapMaterial_->color = material_.material_.color;
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

    dx12->result_ = dx12->device_->CreateCommittedResource
    (
        &HeapProp,        //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &Resdesc,//���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffTranceform_)
    );

    assert(SUCCEEDED(dx12->result_));

    dx12->result_ = constBuffTranceform_->Map(0, nullptr, (void**)&constMapTranceform_);
    material_.Init();
    tex_ = tex;
    Dx12::GetInstance()->result_ = Dx12::GetInstance()->device_->CreateCommittedResource(
        &HeapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &Resdesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_));
    assert(SUCCEEDED(Dx12::GetInstance()->result_));

    sizeVB_ = static_cast<size_t>(sizeof(Vertex));
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = static_cast<uint32_t>(sizeVB_);
    vbView_.StrideInBytes = sizeof(vertex_);
}

void Particle::Update(View& view, Projection& projection, bool isBillBord)
{
    matWorld_.SetMatScale(scale_.x, scale_.y, scale_.z);
    matWorld_.SetMatRot(rotation_.x, rotation_.y, rotation_.z, false);
    matWorld_.SetMatTrans(position_.x, position_.y, position_.z);
    if (isBillBord)
    {
        if (parent_ == nullptr)
        {
            matWorld_.UpdataBillBordMatrixWorld(view);
        }
        else
        {
            matWorld_.UpdataMatrixWorld();
        }
    }
    else
    {
        matWorld_.UpdataMatrixWorld();
    }

    if (parent_ != nullptr)
    {
        matWorld_.matWorld_ *= parent_->matWorld_.matWorld_;
    }

    //constMapTranceform->mat = matWorld.matWorld * view.mat * projection.mat;
    constMapTranceform_->world = matWorld_.matWorld_ * view.mat_;
    constMapTranceform_->viewproj = projection.mat_;
    constMapTranceform_->cameraPos.x_ = view.eye_.x;
    constMapTranceform_->cameraPos.y_ = view.eye_.y;
    constMapTranceform_->cameraPos.z_ = view.eye_.z;
}

void Particle::Draw()
{
    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();


    ////�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList_->SetGraphicsRootConstantBufferView(2,material_.constBuffMaterialB1_->GetGPUVirtualAddress());

    //lights->Draw(3);

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex_->texture->incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList_->SetGraphicsRootConstantBufferView(0, constBuffTranceform_->GetGPUVirtualAddress());
    //���_�f�[�^
    dx12->commandList_->IASetVertexBuffers(0, 1, &vbView_);
    //�`��R�}���h
    dx12->commandList_->DrawInstanced(vertNum_, 1, 0, 0);
    //�C���f�b�N�X�f�[�^
    //dx12.commandList->IASetIndexBuffer(&model->ibView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    //�`��R�}���h
    //dx12.commandList->DrawInstanced(_countof(vert.vertices), 1, 0, 0);

}

