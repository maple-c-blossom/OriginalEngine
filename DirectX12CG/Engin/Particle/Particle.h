#pragma once
#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "WorldMatrix.h"
#include "Dx12.h"
#include "View.h"
#include "Projection.h"
#include "Vector3D.h"
#include "TextureManager.h"
#include <vector>
#include "ParticleMaterial.h"
#include "PIpelineRootSignature.h"

namespace MCB
{
    class Particle
    {
    public:


        //定数バッファ用構造体(行列)------------------------
        typedef struct ConstBufferDataTransform
        {
            //DirectX::XMMATRIX mat;
            DirectX::XMMATRIX viewproj;
            DirectX::XMMATRIX world;
            Float3 cameraPos;
        }ConstBufferDataTransform;
        //---------------------------------
        typedef struct Vertex
        {
            Float3 pos;//xyz座標
        }Vertex;
        //行列用定数バッファ
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform_ = nullptr;

        //行列用定数バッファマップ
        ConstBufferDataTransform* constMapTranceform_ = nullptr;

        //アフィン変換情報
        DirectX::XMFLOAT3 scale_ = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotation_ = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position_ = { 0.0f, 0.0f, 0.0f };
        TextureManager* textureManager_ = TextureManager::GetInstance();
        //ワールド行列
        WorldMatrix matWorld_ = {};
        size_t sizeVB_;
        Vector3D NORM_FRONT_VEC_ = {};
        Vector3D nowFrontVec_ = {};
        float frontAngle_ = 0;
        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
        D3D12_VERTEX_BUFFER_VIEW vbView_{};
        Vertex vertex_;
        int32_t vertNum_ = 1;
        TextureCell* tex_;
        Particle();

        ~Particle();


        //親オブジェクトへのポインタ
        Particle* parent_ = nullptr;
        
        ParticleMaterial material_;

        bool trackingFlag_ = false;

        void SetColor(const Float4& color);

        void Init(TextureCell* tex);

        void Update(View& view, Projection& projection, bool isBillBord = false);

        void Draw();

        //void CreateModel(const char* fileName);
    };

}







