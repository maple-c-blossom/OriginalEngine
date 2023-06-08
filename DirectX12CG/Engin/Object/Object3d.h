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
        std::string name;
        std::shared_ptr<BaseCollider> collider_;
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
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffSkin = nullptr;
        //�s��p�萔�o�b�t�@�}�b�v
        ConstBufferDataTransform* constMapTranceform = nullptr;
        ConstBuffSkin* constMapSkin = nullptr;
    public:
        static LightGroup* lights;
        //�A�t�B���ϊ����
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotasion = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
        Float4 color = { 1.f,1.f,1.f,1.f };
        float shaderNum = 1.f;
        //���[���h�s��
        WorldMatrix matWorld = {};
        Vector3D NORM_FRONT_VEC = {};
        Vector3D nowFrontVec = {0,0,1};
        float frontAngle = 0;
        float animeTime;
        float animationSpeed = 0.1f;
        Model* model = nullptr;
        AnimationModel* animationModel = nullptr;
        bool hited = false;
        Object3d();

        virtual ~Object3d();
        ICamera* camera;
        //�e�I�u�W�F�N�g�ւ̃|�C���^
        Object3d* parent = nullptr;
        bool trackingFlag = false;
        virtual void Init();
        virtual void CreateBuff();
        virtual void Update( const bool& isBillBord = false);
        virtual void UpdateMatrix( const bool& isBillBord = false);

        virtual void Update( const Quaternion& q, const bool& isBillBord = false);
        virtual void UpdateMatrix( const Quaternion& q, const bool& isBillBord = false);

        virtual void Draw();

        virtual void Draw(const uint16_t& incremant);

        virtual void AnimationUpdate(ICamera* camera,const bool& isBillBord = false);

        virtual void AnimationUpdate(ICamera* camera, const Quaternion& q, const bool& isBillBord = false);

        virtual void AnimationDraw();

       virtual void AnimationDraw(const uint16_t& incremant);
       const DirectX::XMMATRIX GetMatWorld() { return matWorld.matWorld; };
       void SetCollider(std::shared_ptr<BaseCollider> collider);
       BaseCollider* GetCollider() { return collider_.get(); };
       virtual void OnCollision(const CollisionInfomation& info) { color = { 1,0,0,1 }; hited = true; }
       virtual void OffCollision(const CollisionInfomation* info = nullptr) { color = { 1,1,1,1 };  hited = false;}
       static void SetLights(LightGroup* light);
       ConstBufferDataTransform* GetConstMapTrans() { return constMapTranceform; };
       ID3D12Resource* GetConstBuffTrans() { return constBuffTranceform.Get(); };
        //void CreateModel(const char* fileName);
    };

}

