#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include "Dx12.h"
#include <d3d12.h>
#include "ObjectMaterial.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "MCBFloat.h"
#include <unordered_map>
#include "Vector3D.h"
#include "TextureManager.h"

namespace MCB
{
        static const uint16_t NUM_BONES_PER_VERTEX = 4;
        static const uint16_t MAX_BONE = 128;
        //���_�f�[�^�\����-------------------------------------
        typedef struct AnimationVertex
        {
            Float3 pos;//xyz���W
            Float3 normal;//�@���x�N�g��
            Float2 uv;//uv���W
            int32_t ids[NUM_BONES_PER_VERTEX] = {};
            float weights[NUM_BONES_PER_VERTEX] = {};
        }AnimationVertex;
        //--------------------------------------
        
        typedef struct ConstBuffSkin
        {
            DirectX::XMMATRIX boneMats[MAX_BONE] = {};
        }ConstBuffSkin;
        typedef struct SetWeight
        {
            int32_t id;
            float weight;
        }SetWeight;


        typedef struct Bone
        {
            std::string name;
            DirectX::XMMATRIX offsetMatrix;
            DirectX::XMMATRIX finalMatrix;
        }Bone;

	class AnimationMesh
	{
        public:

            //Model(const std::string fileName, bool smooth = false);
            //AnimationModel();
            ~AnimationMesh();

            Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;

            Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;

            Microsoft::WRL::ComPtr<ID3D12Resource> ConstBuffSkin_ = nullptr;

            std::vector<TextureCell*> textures_;
            //MCB::Texture textures;
            std::vector<AnimationVertex> vertices_;
            std::vector<uint16_t> indices_;
            //std::vector<ConstBuffSkin> vertexBones;
            
            std::unordered_map<uint16_t, std::vector<uint16_t>>smoothData_;

            size_t sizeVB_ = static_cast<size_t>(sizeof(AnimationVertex) * vertices_.size());
            size_t sizeIB_ = static_cast<size_t>(sizeof(uint16_t) * indices_.size());

            D3D12_INDEX_BUFFER_VIEW ibView_{};

            D3D12_VERTEX_BUFFER_VIEW vbView_{};

 

            //D3D12_HEAP_PROPERTIES heapprop{};   // �q�[�v�ݒ�

            //D3D12_RESOURCE_DESC resdesc{};  // ���\�[�X�ݒ�

            std::vector<ObjectMaterial> material_;

            void CreateVertexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag, const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state);

            void SetIbView(const DXGI_FORMAT& format);



            void SetVbView();

            void CreateIndexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag, const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state);

            HRESULT IndexMaping();

            HRESULT VertexMaping();

            void SetSizeIB();

            void SetSizeVB();

            void Init();

            inline size_t GetVertexCount() { return vertices_.size(); }

        };
	};

	//struct Node
//{
//	std::string name;
//	Vector3D scale = {1.f,1.f,1.f};
//	Vector3D rotation = { 0.f,0,0 };
//	Vector3D translasion = { 0.f,0.f,0.f };
//	DirectX::XMMATRIX transform;
//	DirectX::XMMATRIX  globalTransform;
//	Node* parent = nullptr;
//};


