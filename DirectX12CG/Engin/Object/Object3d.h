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
namespace MCB
{
    class ICamera;

    class Object3d
    {
    public:

        //�萔�o�b�t�@�p�\����(�s��)------------------------
        typedef struct ConstBufferDataTransform
        {
            //DirectX::XMMATRIX mat;
            DirectX::XMMATRIX viewproj;
            DirectX::XMMATRIX world;
            Float3 cameraPos;

        }ConstBufferDataTransform;
        //---------------------------------

        static LightGroup* lights;

        //�s��p�萔�o�b�t�@
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffSkin = nullptr;
        //�s��p�萔�o�b�t�@�}�b�v
        ConstBufferDataTransform* constMapTranceform = nullptr;
        ConstBuffSkin* constMapSkin = nullptr;

        //�A�t�B���ϊ����
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotasion = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

        //���[���h�s��
        WorldMatrix matWorld = {};

        Vector3D NORM_FRONT_VEC = {};
        Vector3D nowFrontVec = {0,0,1};
        float frontAngle = 0;
        float animeTime;
        Model* model = nullptr;
        AnimationModel* animationModel = nullptr;

        Object3d();

        ~Object3d();

        //�e�I�u�W�F�N�g�ւ̃|�C���^
        Object3d* parent = nullptr;

        bool trackingFlag = false;

        void Init();

        void Update(ICamera* camera, bool isBillBord = false);

        void Update(ICamera* camera, Quaternion q, bool isBillBord = false);

        void Draw();

        void Draw(unsigned short int incremant);


        void AnimationUpdate(ICamera* camera, bool isBillBord = false);

        void AnimationUpdate(ICamera* camera, Quaternion q, bool isBillBord = false);

        void AnimationDraw();

        void AnimationDraw(unsigned short int incremant);

        static void SetLights(LightGroup* light);
        //void CreateModel(const char* fileName);
    };

}
