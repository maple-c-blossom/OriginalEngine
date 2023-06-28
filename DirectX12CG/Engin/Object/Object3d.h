#pragma once
#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "WorldMatrix.h"
#include "Dx12.h"
#include "Vector3D.h"
#include "Descriptor.h"
#include <vector>
#include "Model.h"
#include "Quaternion.h"
#include "LightGroup.h"
#include "FBXLoader.h"
#include "CollisionInfomation.h"
#include <memory>
namespace MCB
{
    class ICamera;
    class BaseCollider;

    class Object3d
    {
    protected:
        std::string name_;
        BaseCollider* collider_ = nullptr;
        //�萔�o�b�t�@�p�\����(�s��)------------------------
        typedef struct ConstBufferDataTransform
        {
            //DirectX::XMMATRIX mat;
            DirectX::XMMATRIX viewproj;
            DirectX::XMMATRIX cameraMat;
            DirectX::XMMATRIX world;
            Float4 color = {1.f,1.f,1.f,1.f};
            Float3 cameraPos;
            float shaderNum;

        }ConstBufferDataTransform;
        //---------------------------------
        //�s��p�萔�o�b�t�@
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffSkin_ = nullptr;
        //�s��p�萔�o�b�t�@�}�b�v
        ConstBufferDataTransform* constMapTranceform_ = nullptr;
        ConstBuffSkin* constMapSkin_ = nullptr;
    public:
        static LightGroup* slights_;
        //�A�t�B���ϊ����
        DirectX::XMFLOAT3 scale_ = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotation_ = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position_ = { 0.0f, 0.0f, 0.0f };
        Float4 color_ = { 1.f,1.f,1.f,1.f };
        float shaderNum_ = 1.f;
        //���[���h�s��
        WorldMatrix matWorld_ = {};
        Vector3D normFrontVec_ = {};
        Vector3D nowFrontVec_ = {0,0,1};
        float frontAngle_ = 0;
        float animeTime_;
        float animationSpeed_ = 0.1f;
        Model* model_ = nullptr;
        AnimationModel* animationModel_ = nullptr;
        bool hited_ = false;
        Object3d();

        virtual ~Object3d();
        ICamera* camera_;
        //�e�I�u�W�F�N�g�ւ̃|�C���^
        Object3d* parent_ = nullptr;
        bool trackingFlag_ = false;
        virtual void Init();
        virtual void CreateBuff();
        virtual void UniqueUpdate();
        virtual void Update(  bool isBillBord = false);
        virtual void UpdateMatrix(  bool isBillBord = false);

        virtual void Update(  Quaternion q,  bool isBillBord = false);
        virtual void UpdateMatrix(  Quaternion q,  bool isBillBord = false);

        virtual void Draw();

        virtual void Draw( uint16_t incremant);

        virtual void AnimationUpdate( bool isBillBord = false);

        virtual void AnimationUpdate( Quaternion q,  bool isBillBord = false);

        virtual void AnimationDraw();

       virtual void AnimationDraw( uint16_t incremant);
       const DirectX::XMMATRIX GetMatWorld() { return matWorld_.matWorld_; };
       void SetCollider(std::unique_ptr<BaseCollider> collider);
       BaseCollider* GetCollider() { return collider_; };
       virtual void OnCollision(const CollisionInfomation& info) { color_ = { 1,0,0,1 }; hited_ = true; }
       virtual void OffCollision(const CollisionInfomation* info = nullptr) { color_ = { 1,1,1,1 };  hited_ = false;}
       static void SetLights(LightGroup* light);
       ConstBufferDataTransform* GetConstMapTrans() { return constMapTranceform_; };
       ID3D12Resource* GetConstBuffTrans() { return constBuffTranceform_.Get(); };
        //void CreateModel(const char* fileName);
    };

}

