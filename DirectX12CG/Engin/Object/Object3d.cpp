#include "Object3d.h"
#include "ICamera.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
using namespace MCB;
using namespace std;

LightGroup* Object3d::slights_ = nullptr;
ICamera* Object3d::camera_ = nullptr;


MCB::Object3d::Object3d()
{
    CreateBuff();
}



MCB::Object3d::~Object3d()
{
    if (collider_)
    {
        CollisionManager::GetInstance()->RemoveCollider(collider_);
        collider_ = nullptr;
    }
    //constBuffTranceform_->Unmap(0, nullptr);
    //constBuffSkin_->Unmap(0, nullptr);
}

void Object3d::Init()
{
    name_ = typeid(*this).name();

}

void MCB::Object3d::CreateBuff()
{
    Dx12* dx12 = Dx12::GetInstance();
    normFrontVec_.vec_ = { 0,0,1 };
    nowFrontVec_ = normFrontVec_;

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
        &HeapProp,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &Resdesc,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffTranceform_)
    );

    assert(SUCCEEDED(dx12->result_));

    dx12->result_ = constBuffTranceform_->Map(0, nullptr, (void**)&constMapTranceform_);

    D3D12_RESOURCE_DESC ResdescFbx{};
    ResdescFbx.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    ResdescFbx.Width = (sizeof(ConstBuffSkin) + 0xff) & ~0xff;
    ResdescFbx.Height = 1;
    ResdescFbx.DepthOrArraySize = 1;
    ResdescFbx.MipLevels = 1;
    ResdescFbx.SampleDesc.Count = 1;
    ResdescFbx.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    dx12->result_ = dx12->device_->CreateCommittedResource
    (
        &HeapProp,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &ResdescFbx,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffSkin_)
    );

    assert(SUCCEEDED(dx12->result_));

    dx12->result_ = constBuffSkin_->Map(0, nullptr, (void**)&constMapSkin_);
}

void MCB::Object3d::UniqueUpdate()
{
    isInvisible = false;
}

void Object3d::Update(bool isBillBord)
{
    UpdateMatrix(isBillBord);
    constMapTranceform_->world = matWorld_.matWorld_;
    constMapTranceform_->cameraMat = camera_->GetView()->mat_;
    constMapTranceform_->viewproj = camera_->GetProjection()->mat_;
    constMapTranceform_->cameraPos.x_ = camera_->GetView()->eye_.x;
    constMapTranceform_->cameraPos.y_ = camera_->GetView()->eye_.y;
    constMapTranceform_->cameraPos.z_ = camera_->GetView()->eye_.z;
    constMapTranceform_->shaderNum = shaderNum_;
    constMapTranceform_->color = color_;
    if(collider_)collider_->Update();
}

void MCB::Object3d::UpdateMatrix( bool isBillBord)
{
    matWorld_.SetMatScale(scale_.x, scale_.y, scale_.z);
    matWorld_.SetMatRot(rotation_.x, rotation_.y, rotation_.z, false);
    matWorld_.SetMatTrans(position_.x, position_.y, position_.z);
    if (isBillBord)
    {
        if (parent_ == nullptr)
        {
            matWorld_.UpdataBillBordMatrixWorld(*camera_->GetView());
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
}

void Object3d::Update(Quaternion q, bool isBillBord)
{
    UpdateMatrix(q,isBillBord);

    constMapTranceform_->world = matWorld_.matWorld_;
    constMapTranceform_->cameraMat = camera_->GetView()->mat_;
    constMapTranceform_->viewproj = camera_->GetProjection()->mat_;
    constMapTranceform_->cameraPos.x_ = camera_->GetView()->eye_.x;
    constMapTranceform_->cameraPos.y_ = camera_->GetView()->eye_.y;
    constMapTranceform_->cameraPos.z_ = camera_->GetView()->eye_.z;
    constMapTranceform_->shaderNum = shaderNum_;
    constMapTranceform_->color = color_;
    if (collider_)collider_->Update();
}

void MCB::Object3d::UpdateMatrix( Quaternion q, bool isBillBord)
{
    MCBMatrix matRot;
    matRot.MCBMatrixIdentity();
    matWorld_.SetMatScale(scale_.x, scale_.y, scale_.z);
    matWorld_.matRot_ = matRot.MatrixConvertXMMatrix(q.GetQuaternionRotaMat(q));
    matWorld_.SetMatTrans(position_.x, position_.y, position_.z);
    if (isBillBord)
    {
        if (parent_ == nullptr)
        {
            matWorld_.UpdataBillBordMatrixWorld(*camera_->GetView());
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
}

void Object3d::Draw()
{
    if (model_ == nullptr || isInvisible)return;
    if (model_->material_.constBuffMaterialB1_ == nullptr)return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList_->SetGraphicsRootConstantBufferView(2, model_->material_.constBuffMaterialB1_->GetGPUVirtualAddress());
    slights_->Draw(3);
    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += model_->texture_->texture->incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList_->IASetVertexBuffers(0, 1, &model_->vbView_);
    //インデックスデータ
    dx12->commandList_->IASetIndexBuffer(&model_->ibView_);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList_->SetGraphicsRootConstantBufferView(0, constBuffTranceform_->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList_->DrawIndexedInstanced(static_cast<uint32_t>( model_->indices_.size()), 1, 0, 0, 0);

}

void Object3d::Draw(uint16_t incremant)
{
    if (model_ == nullptr || isInvisible)return;
    if (model_->material_.constBuffMaterialB1_ == nullptr)return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList_->SetGraphicsRootConstantBufferView(2, model_->material_.constBuffMaterialB1_->GetGPUVirtualAddress());

    slights_->Draw(3);

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += incremant * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList_->IASetVertexBuffers(0, 1, &model_->vbView_);
    //インデックスデータ
    dx12->commandList_->IASetIndexBuffer(&model_->ibView_);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList_->SetGraphicsRootConstantBufferView(0, constBuffTranceform_->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList_->DrawIndexedInstanced(static_cast<uint32_t>(model_->indices_.size()), 1, 0, 0, 0);

}

void MCB::Object3d::AnimationUpdate(bool isBillBord)
{
    
    if (animationModel_ == nullptr)return;
    UpdateMatrix(isBillBord);
    constMapTranceform_->world = matWorld_.matWorld_;
    constMapTranceform_->cameraMat = camera_->GetView()->mat_ /** animationModel->nodes.begin()->get()->globalTransform*/;
    constMapTranceform_->viewproj = camera_->GetProjection()->mat_;
    constMapTranceform_->cameraPos.x_ = camera_->GetView()->eye_.x;
    constMapTranceform_->cameraPos.y_ = camera_->GetView()->eye_.y;
    constMapTranceform_->cameraPos.z_ = camera_->GetView()->eye_.z;
    constMapTranceform_->color = color_;
    constMapTranceform_->shaderNum = shaderNum_;
    if (collider_)collider_->Update();
    animeTime_ += animationSpeed_;

  
    
    animationModel_->boneAnimTransform(animeTime_,currentAnimation_);
    for (size_t i = 0; i < animationModel_->bones_.size(); i++)
    {
        constMapSkin_->boneMats[i] = animationModel_->bones_[i].finalMatrix;
    }
}

void MCB::Object3d::AnimationUpdate( Quaternion q, bool isBillBord)
{
    if (animationModel_ == nullptr)return;
    UpdateMatrix(q,isBillBord);

    constMapTranceform_->world = matWorld_.matWorld_;
    constMapTranceform_->cameraMat = camera_->GetView()->mat_ /** animationModel->nodes.begin()->get()->globalTransform*/;
    constMapTranceform_->viewproj = camera_->GetProjection()->mat_;
    constMapTranceform_->cameraPos.x_ = camera_->GetView()->eye_.x;
    constMapTranceform_->cameraPos.y_ = camera_->GetView()->eye_.y;
    constMapTranceform_->cameraPos.z_ = camera_->GetView()->eye_.z;
    constMapTranceform_->shaderNum = shaderNum_;
    constMapTranceform_->color = color_;
    if (collider_)collider_->Update();
    animeTime_ += animationSpeed_;

    if (animeTime_ >= animationModel_->animations_[0]->duration)
    {
        animeTime_ = 0;
    }

    animationModel_->boneAnimTransform(animeTime_, currentAnimation_);
    for (size_t i = 0; i < animationModel_->bones_.size(); i++)
    {
        constMapSkin_->boneMats[i] = animationModel_->bones_[i].finalMatrix;
    }
}

void MCB::Object3d::AnimationDraw()
{
    if (animationModel_ == nullptr || isInvisible)return;
    //定数バッファビュー(CBV)の設定コマンド
    Dx12::GetInstance()->commandList_->SetGraphicsRootConstantBufferView(0, constBuffTranceform_->GetGPUVirtualAddress());
    Dx12::GetInstance()->commandList_->SetGraphicsRootConstantBufferView(4, constBuffSkin_->GetGPUVirtualAddress());
    animationModel_->Draw();
}

void MCB::Object3d::AnimationDraw(uint16_t incremant)
{
    if (animationModel_ == nullptr || isInvisible)return;
    Dx12::GetInstance()->commandList_->SetGraphicsRootConstantBufferView(0, constBuffTranceform_->GetGPUVirtualAddress());
    Dx12::GetInstance()->commandList_->SetGraphicsRootConstantBufferView(4, constBuffSkin_->GetGPUVirtualAddress());
    animationModel_->Draw();
}

void MCB::Object3d::SetCollider(unique_ptr<BaseCollider> collider)
{
    collider->SetObject(this);
    collider->Update();
    collider_ = CollisionManager::GetInstance()->AddCollider(move(collider));
}

void MCB::Object3d::SetLights(LightGroup* lights)
{
    Object3d::slights_ = lights;
}
