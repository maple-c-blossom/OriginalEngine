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


        //�萔�o�b�t�@�p�\����(�s��)------------------------
        typedef struct ConstBufferDataTransform
        {
            //DirectX::XMMATRIX mat;
            DirectX::XMMATRIX viewproj;
            DirectX::XMMATRIX world;
            Float3 cameraPos;
        };
        //---------------------------------
        typedef struct Vertex
        {
            Float3 pos;//xyz���W
        };
        //�s��p�萔�o�b�t�@
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;

        //�s��p�萔�o�b�t�@�}�b�v
        ConstBufferDataTransform* constMapTranceform = nullptr;

        //�A�t�B���ϊ����
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotasion = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
        TextureManager* textureManager = TextureManager::GetInstance();
        //���[���h�s��
        WorldMatrix matWorld = {};
        unsigned int sizeVB;
        Vector3D NORM_FRONT_VEC = {};
        Vector3D nowFrontVec = {};
        float frontAngle = 0;
        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
        D3D12_VERTEX_BUFFER_VIEW vbView{};
        Vertex vertex;
        int vertNum = 1;
        TextureCell* tex;
        Particle();

        ~Particle();


        //�e�I�u�W�F�N�g�ւ̃|�C���^
        Particle* parent = nullptr;
        
        ParticleMaterial material;

        bool trackingFlag = false;

        void SetColor(Float4 color);

        void Init(TextureCell* tex);

        void Update(View& view, Projection& projection, bool isBillBord = false);

        void Draw();

        //void CreateModel(const char* fileName);
    };

}







