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
        static const unsigned short int NUM_BONES_PER_VERTEX = 4;
        static const unsigned short int MAX_BONE = 128;
        //���_�f�[�^�\����-------------------------------------
        typedef struct AnimationVertex
        {
            Float3 pos;//xyz���W
            Float3 normal;//�@���x�N�g��
            Float2 uv;//uv���W
            unsigned int ids[NUM_BONES_PER_VERTEX] = {};
            float weights[NUM_BONES_PER_VERTEX] = {};
        }AnimationVertex;
        //--------------------------------------
        
        typedef struct ConstBuffSkin
        {
            DirectX::XMMATRIX boneMats[MAX_BONE] = {};
        }ConstBuffSkin;
        typedef struct SetWeight
        {
            unsigned int id;
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

            Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

            Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

            Microsoft::WRL::ComPtr<ID3D12Resource> ConstBuffSkin = nullptr;

            std::vector<TextureCell*> textures;
            //MCB::Texture textures;
            std::vector<AnimationVertex> vertices;
            std::vector<unsigned short> indices;
            //std::vector<ConstBuffSkin> vertexBones;
            
            std::unordered_map<unsigned short int, std::vector<unsigned short int>>smoothData;

            unsigned int sizeVB = static_cast<unsigned int>(sizeof(AnimationVertex) * vertices.size());
            unsigned int sizeIB = static_cast<unsigned int>(sizeof(unsigned short) * indices.size());

            D3D12_INDEX_BUFFER_VIEW ibView{};

            D3D12_VERTEX_BUFFER_VIEW vbView{};

 

            //D3D12_HEAP_PROPERTIES heapprop{};   // �q�[�v�ݒ�

            //D3D12_RESOURCE_DESC resdesc{};  // ���\�[�X�ݒ�

            std::vector<ObjectMaterial> material;

            void CreateVertexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);

            void SetIbView(DXGI_FORMAT format);

            void SetVbView();

            void CreateIndexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);

            HRESULT IndexMaping();

            HRESULT VertexMaping();

            void SetSizeIB();

            void SetSizeVB();

            void Init();

            inline size_t GetVertexCount() { return vertices.size(); }

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

