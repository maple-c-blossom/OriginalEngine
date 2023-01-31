#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include "Dx12.h"
#include <d3d12.h>
#include "ObjectMaterial.h"
namespace MCB
{
    //���_�f�[�^�\����-------------------------------------
    typedef struct StructVertex
    {
        DirectX::XMFLOAT3 pos;//xyz���W
        DirectX::XMFLOAT3 normal;//�@���x�N�g��
        DirectX::XMFLOAT2 uv;//uv���W
    };
    //--------------------------------------
    class Vertex
    {
    public:
        //StructVertex Box[24] =
        //{
        //    //�O
        //    {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        //    {{-5.0f,5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// ����
        //    {{5.0f,-5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        //    {{5.0f,5.0f,-5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //    //���
        //    {{-5.0f,5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// ����
        //    {{-5.0f,-5.0f,5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        //    {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //    {{5.0f,-5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        //    //��
        //    {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        //    {{-5.0f,-5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// ����
        //    {{-5.0f,5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        //    {{-5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //    //�E
        //    {{5.0f,-5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// ����
        //    {{5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        //    {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //    {{5.0f,5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        //    //��
        //    {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        //    {{5.0f,-5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// ����
        //    {{-5.0f,-5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        //    {{5.0f,-5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //    //��
        //    {{5.0f,5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// ����
        //    {{-5.0f,5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        //    {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //    {{-5.0f,5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        //};

        StructVertex vertices[1] =
        {
            {{0.0f,0.0f,0.0f},{0,0,1},{0,1}},
        };


        //unsigned short boxIndices[36] =
        //{
        //    //�O
        //    0,1,2,
        //    2,1,3,
        //    //���
        //    4,5,6,
        //    6,5,7,
        //    //��
        //    8,9,10,
        //    10,9,11,
        //    //�E
        //    12,13,14,
        //    14,13,15,
        //    //��
        //    16,17,18,
        //    18,17,19,
        //    //��
        //    20,21,22,
        //    22,21,23,
        //};
    
        //Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

        uint16_t* indexMap = nullptr;

        StructVertex* vertMap = nullptr;

        D3D12_VERTEX_BUFFER_VIEW vbView{};

         UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

         //�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
         //UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(boxIndices));

         //D3D12_INDEX_BUFFER_VIEW ibView{};

         //D3D12_HEAP_PROPERTIES heapprop{};   // �q�[�v�ݒ�
         static const int vertexCount = 1;

         ObjectMaterial material;

         D3D12_HEAP_PROPERTIES heapprop{};   // �q�[�v�ݒ�

         //D3D12_RESOURCE_DESC resdesc{};  // ���\�[�X�ݒ�

         void CreateVertexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);

         void SetIbView(DXGI_FORMAT format);

         void SetVbView();

         void CreateIndexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);

         void CalculationNormalVec();

         HRESULT IndexMaping();

         HRESULT VertexMaping();

        
    };

}

