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
namespace MCB
{
    class ICamera;
    class BaseCollider;

    class Object3d
    {
    protected:
        std::string name;
        BaseCollider* collider;
        //定数バッファ用構造体(行列)------------------------
        typedef struct ConstBufferDataTransform
        {
            //DirectX::XMMATRIX mat;
            DirectX::XMMATRIX viewproj;
            DirectX::XMMATRIX world;
            Float4 color = {1.f,1.f,1.f,1.f};
            Float3 cameraPos;
            float shaderNum;

        }ConstBufferDataTransform;
        //---------------------------------
        //行列用定数バッファ
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffSkin = nullptr;
        //行列用定数バッファマップ
        ConstBufferDataTransform* constMapTranceform = nullptr;
        ConstBuffSkin* constMapSkin = nullptr;
    public:
        static LightGroup* lights;
        //アフィン変換情報
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotasion = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
        Float4 color = { 1.f,1.f,1.f,1.f };
        float shaderNum = 1.f;
        //ワールド行列
        WorldMatrix matWorld = {};
        Vector3D NORM_FRONT_VEC = {};
        Vector3D nowFrontVec = {0,0,1};
        float frontAngle = 0;
        float animeTime;
        Model* model = nullptr;
        AnimationModel* animationModel = nullptr;
        bool hited = false;
        Object3d();

        virtual ~Object3d();
        ICamera* camera;
        //親オブジェクトへのポインタ
        Object3d* parent = nullptr;
        bool trackingFlag = false;
        virtual void Init();
        virtual void CreateBuff();
        virtual void Update( bool isBillBord = false);
        virtual void UpdateMatrix( bool isBillBord = false);

        virtual void Update( Quaternion q, bool isBillBord = false);
        virtual void UpdateMatrix( Quaternion q, bool isBillBord = false);

        virtual void Draw();

        virtual void Draw(unsigned short int incremant);

        virtual void AnimationUpdate(ICamera* camera, bool isBillBord = false);

        virtual void AnimationUpdate(ICamera* camera, Quaternion q, bool isBillBord = false);

        virtual void AnimationDraw();

       virtual void AnimationDraw(unsigned short int incremant);
       const DirectX::XMMATRIX GetMatWorld() { return matWorld.matWorld; };
       void SetCollider(BaseCollider* collider);
       BaseCollider* GetCollider() { return collider; };
       virtual void OnCollision(const CollisionInfomation& info) { color = { 1,0,0,1 }; hited = true; }
       virtual void OffCollision(const CollisionInfomation* info = nullptr) { color = { 1,1,1,1 };  hited = false;}
       static void SetLights(LightGroup* light);
       ConstBufferDataTransform* GetConstMapTrans() { return constMapTranceform; };
       ID3D12Resource* GetConstBuffTrans() { return constBuffTranceform.Get(); };
        //void CreateModel(const char* fileName);
    };

}

