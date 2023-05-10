#include "Object3d.h"
#include "ICamera.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
using namespace MCB;
using namespace std;

LightGroup* Object3d::lights = nullptr;

MCB::Object3d::Object3d()
{
    this->CreateBuff();
}



MCB::Object3d::~Object3d()
{
    if (collider)
    {
        CollisionManager::GetInstance()->RemoveCollider(collider);
        delete collider;
        collider = nullptr;
    }
    constBuffTranceform->Unmap(0, nullptr);
    constBuffSkin->Unmap(0, nullptr);
}

void Object3d::Init()
{
    name = typeid(*this).name();

}

void MCB::Object3d::CreateBuff()
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
        &HeapProp,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &Resdesc,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffTranceform)
    );

    assert(SUCCEEDED(dx12->result));

    dx12->result = constBuffTranceform->Map(0, nullptr, (void**)&constMapTranceform);

    D3D12_RESOURCE_DESC ResdescFbx{};
    ResdescFbx.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    ResdescFbx.Width = (sizeof(ConstBuffSkin) + 0xff) & ~0xff;
    ResdescFbx.Height = 1;
    ResdescFbx.DepthOrArraySize = 1;
    ResdescFbx.MipLevels = 1;
    ResdescFbx.SampleDesc.Count = 1;
    ResdescFbx.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    dx12->result = dx12->device->CreateCommittedResource
    (
        &HeapProp,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &ResdescFbx,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffSkin)
    );

    assert(SUCCEEDED(dx12->result));

    dx12->result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
}

void Object3d::Update(bool isBillBord)
{
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.SetMatRot(rotasion.x, rotasion.y, rotasion.z,false);
    matWorld.SetMatTrans(position.x, position.y, position.z);
    if (isBillBord)
    {
        if (parent == nullptr)
        {
            matWorld.UpdataBillBordMatrixWorld(*camera->GetView());
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
    
    constMapTranceform->world = matWorld.matWorld;
    constMapTranceform->cameraMat = camera->GetView()->mat;
    constMapTranceform->viewproj = camera->GetProjection()->mat;
    constMapTranceform->cameraPos.x = camera->GetView()->eye.x;
    constMapTranceform->cameraPos.y = camera->GetView()->eye.y;
    constMapTranceform->cameraPos.z = camera->GetView()->eye.z;
    constMapTranceform->shaderNum = shaderNum;
    constMapTranceform->color = color;
    if(collider)collider->Update();
}

void MCB::Object3d::UpdateMatrix( bool isBillBord)
{
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.SetMatRot(rotasion.x, rotasion.y, rotasion.z, false);
    matWorld.SetMatTrans(position.x, position.y, position.z);
    if (isBillBord)
    {
        if (parent == nullptr)
        {
            matWorld.UpdataBillBordMatrixWorld(*camera->GetView());
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
}

void Object3d::Update(Quaternion q, bool isBillBord)
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
            matWorld.UpdataBillBordMatrixWorld(*camera->GetView());
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

    constMapTranceform->world = matWorld.matWorld;
    constMapTranceform->cameraMat = camera->GetView()->mat;
    constMapTranceform->viewproj = camera->GetProjection()->mat;
    constMapTranceform->cameraPos.x = camera->GetView()->eye.x;
    constMapTranceform->cameraPos.y = camera->GetView()->eye.y;
    constMapTranceform->cameraPos.z = camera->GetView()->eye.z;
    constMapTranceform->shaderNum = shaderNum;
    constMapTranceform->color = color;
    if (collider)collider->Update();
}

void MCB::Object3d::UpdateMatrix( Quaternion q, bool isBillBord)
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
            matWorld.UpdataBillBordMatrixWorld(*camera->GetView());
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
}

void Object3d::Draw()
{
    if (model == nullptr)return;
    if (model->material.constBuffMaterialB1 == nullptr)return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList->SetGraphicsRootConstantBufferView(2, model->material.constBuffMaterialB1->GetGPUVirtualAddress());
    lights->Draw(3);
    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += model->texture->texture->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList->IASetVertexBuffers(0, 1, &model->vbView);
    //インデックスデータ
    dx12->commandList->IASetIndexBuffer(&model->ibView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList->DrawIndexedInstanced((unsigned int) model->indices.size(), 1, 0, 0, 0);

}

void Object3d::Draw(unsigned short int incremant)
{
    if (model == nullptr)return;
    if (model->material.constBuffMaterialB1 == nullptr)return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList->SetGraphicsRootConstantBufferView(2, model->material.constBuffMaterialB1->GetGPUVirtualAddress());

    lights->Draw(3);

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += incremant * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList->IASetVertexBuffers(0, 1, &model->vbView);
    //インデックスデータ
    dx12->commandList->IASetIndexBuffer(&model->ibView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList->DrawIndexedInstanced((unsigned int)model->indices.size(), 1, 0, 0, 0);

}

void MCB::Object3d::AnimationUpdate(ICamera* camera, bool isBillBord)
{
    
    if (animationModel == nullptr)return;
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.SetMatRot(rotasion.x, rotasion.y, rotasion.z, false);
    matWorld.SetMatTrans(position.x, position.y, position.z);
    if (isBillBord)
    {
        if (parent == nullptr)
        {
            matWorld.UpdataBillBordMatrixWorld(*camera->GetView());
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

    constMapTranceform->world = matWorld.matWorld;
    constMapTranceform->cameraMat = camera->GetView()->mat /** animationModel->nodes.begin()->get()->globalTransform*/;
    constMapTranceform->viewproj = camera->GetProjection()->mat;
    constMapTranceform->cameraPos.x = camera->GetView()->eye.x;
    constMapTranceform->cameraPos.y = camera->GetView()->eye.y;
    constMapTranceform->cameraPos.z = camera->GetView()->eye.z;
    constMapTranceform->color = color;
    constMapTranceform->shaderNum = shaderNum;
    if (collider)collider->Update();
    animeTime += 0.1f;

    if (animeTime >= animationModel->animations[0]->duration)
    {
        animeTime = 0;
    }
    
    animationModel->boneAnimTransform(animeTime);
    for (int i = 0; i < animationModel->bones.size(); i++)
    {
        constMapSkin->boneMats[i] = animationModel->bones[i].finalMatrix;
    }
}

void MCB::Object3d::AnimationUpdate(ICamera* camera, Quaternion q, bool isBillBord)
{
    if (animationModel == nullptr)return;
    MCBMatrix matRot;
    matRot.MCBMatrixIdentity();
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.matRot = matRot.MatrixConvertXMMatrix(q.GetQuaternionRotaMat(q));
    matWorld.SetMatTrans(position.x, position.y, position.z);
    if (isBillBord)
    {
        if (parent == nullptr)
        {
            matWorld.UpdataBillBordMatrixWorld(*camera->GetView());
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

    constMapTranceform->world = matWorld.matWorld;
    constMapTranceform->cameraMat = camera->GetView()->mat /** animationModel->nodes.begin()->get()->globalTransform*/;
    constMapTranceform->viewproj = camera->GetProjection()->mat;
    constMapTranceform->cameraPos.x = camera->GetView()->eye.x;
    constMapTranceform->cameraPos.y = camera->GetView()->eye.y;
    constMapTranceform->cameraPos.z = camera->GetView()->eye.z;
    constMapTranceform->shaderNum = shaderNum;
    constMapTranceform->color = color;
    if (collider)collider->Update();
}

void MCB::Object3d::AnimationDraw()
{
    //定数バッファビュー(CBV)の設定コマンド
    Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(4, constBuffSkin->GetGPUVirtualAddress());
    animationModel->Draw();
}

void MCB::Object3d::AnimationDraw(unsigned short int incremant)
{
    Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    Dx12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(4, constBuffSkin->GetGPUVirtualAddress());
    animationModel->Draw();
}

void MCB::Object3d::SetCollider(BaseCollider* collider)
{
    collider->SetObject(this);
    collider->Update();
    this->collider = collider;
    CollisionManager::GetInstance()->AddCollider(this->collider);
}

void MCB::Object3d::SetLights(LightGroup* lights)
{
    Object3d::lights = lights;
}
