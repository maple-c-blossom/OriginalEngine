#include "Object3d.h"

using namespace MCB;
using namespace std;

LightGroup* Object3d::lights = nullptr;

MCB::Object3d::Object3d()
{
    NORM_FRONT_VEC.vec = { 0,0,1 };
    nowFrontVec = NORM_FRONT_VEC;
    this->Init();
}



MCB::Object3d::~Object3d()
{
    constBuffTranceform->Unmap(0, nullptr);
}

void Object3d::Init()
{
    Dx12* dx12 = Dx12::GetInstance();
    NORM_FRONT_VEC.vec = { 0,0,1 };
    nowFrontVec = NORM_FRONT_VEC;

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
}

void Object3d::Update(View& view, Projection& projection,bool isBillBord)
{
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.SetMatRot(rotasion.x, rotasion.y, rotasion.z,false);
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
    
    constMapTranceform->world = matWorld.matWorld * view.mat;
    constMapTranceform->viewproj = projection.mat;
    constMapTranceform->cameraPos.x = view.eye.x;
    constMapTranceform->cameraPos.y = view.eye.y;
    constMapTranceform->cameraPos.z = view.eye.z;
}

void Object3d::Update(View& view, Projection& projection,Quaternion q, bool isBillBord)
{
    MCBMatrix matRot;
    matRot.MCBMatrixIdentity();
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.matRot = matRot.MatrixConvertXMMatrix(q.GetQuaternionRotaMat(q));
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

    constMapTranceform->world = matWorld.matWorld * view.mat;
    constMapTranceform->viewproj = projection.mat;
    constMapTranceform->cameraPos.x = view.eye.x;
    constMapTranceform->cameraPos.y = view.eye.y;
    constMapTranceform->cameraPos.z = view.eye.z;
}

void Object3d::Draw()
{
    if (model == nullptr)return;
    if (model->material.constBuffMaterialB1 == nullptr)return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(2, model->material.constBuffMaterialB1->GetGPUVirtualAddress());
    lights->Draw(3);
    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += model->texture->texture->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &model->vbView);
    //�C���f�b�N�X�f�[�^
    dx12->commandList->IASetIndexBuffer(&model->ibView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12->commandList->DrawIndexedInstanced((unsigned int) model->indices.size(), 1, 0, 0, 0);

}

void Object3d::Draw(unsigned short int incremant)
{
    if (model == nullptr)return;
    if (model->material.constBuffMaterialB1 == nullptr)return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(2, model->material.constBuffMaterialB1->GetGPUVirtualAddress());

    lights->Draw(3);

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += incremant * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &model->vbView);
    //�C���f�b�N�X�f�[�^
    dx12->commandList->IASetIndexBuffer(&model->ibView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12->commandList->DrawIndexedInstanced((unsigned int)model->indices.size(), 1, 0, 0, 0);

}

void MCB::Object3d::FbxUpdate(View& view, Projection& projection, bool isBillBord)
{
    if (fbxModel == nullptr)return;
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

    constMapTranceform->world = matWorld.matWorld * view.mat * fbxModel->nodes.begin()->get()->globalTransform;
    constMapTranceform->viewproj = projection.mat;
    constMapTranceform->cameraPos.x = view.eye.x;
    constMapTranceform->cameraPos.y = view.eye.y;
    constMapTranceform->cameraPos.z = view.eye.z;
}

void MCB::Object3d::FbxUpdate(View& view, Projection& projection, Quaternion q, bool isBillBord)
{
    if (fbxModel == nullptr)return;
    MCBMatrix matRot;
    matRot.MCBMatrixIdentity();
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.matRot = matRot.MatrixConvertXMMatrix(q.GetQuaternionRotaMat(q));
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

    constMapTranceform->world = matWorld.matWorld * view.mat * fbxModel->nodes.begin()->get()->globalTransform;
    constMapTranceform->viewproj = projection.mat;
    constMapTranceform->cameraPos.x = view.eye.x;
    constMapTranceform->cameraPos.y = view.eye.y;
    constMapTranceform->cameraPos.z = view.eye.z;
}

void MCB::Object3d::FbxDraw()
{
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    fbxModel->Draw();
}

void MCB::Object3d::FbxDraw(unsigned short int incremant)
{
    Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    fbxModel->Draw();
}

void MCB::Object3d::SetLights(LightGroup* lights)
{
    Object3d::lights = lights;
}
