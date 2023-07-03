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
    }StructVertex;
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

        StructVertex vertices_[1] =
        {
            {{0.0f,0.0f,0.0f},{0,0,1},{0,1}},
        };


        //uint16_t boxIndices[36] =
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

        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;

        uint16_t* indexMap_ = nullptr;

        StructVertex* vertMap_ = nullptr;

        D3D12_VERTEX_BUFFER_VIEW vbView_{};

         uint32_t sizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]) * _countof(vertices_));

         //�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
         //uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * _countof(boxIndices));

         //D3D12_INDEX_BUFFER_VIEW ibView{};

         //D3D12_HEAP_PROPERTIES heapprop{};   // �q�[�v�ݒ�
         static const int32_t vertexCount_ = 1;

         ObjectMaterial material_;

         D3D12_HEAP_PROPERTIES heapprop_{};   // �q�[�v�ݒ�

         //D3D12_RESOURCE_DESC resdesc{};  // ���\�[�X�ݒ�

         void CreateVertexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp,const D3D12_HEAP_FLAGS& flag, const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state);

         void SetIbView(const DXGI_FORMAT& format);

         void SetVbView();

         void CreateIndexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp,const D3D12_HEAP_FLAGS& flag, const D3D12_RESOURCE_DESC& Resdesc,const D3D12_RESOURCE_STATES& state);

         void CalculationNormalVec();

         HRESULT IndexMaping();

         HRESULT VertexMaping();

        
    };

}

