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

        //頂点データ構造体-------------------------------------
        typedef struct FBXVertex
        {
            Float3 pos;//xyz座標
            Float3 normal;//法線ベクトル
            Float2 uv;//uv座標
        }FBXVertex;
        //--------------------------------------

        typedef struct VertexBoneData
        {
            unsigned int ids[NUM_BONES_PER_VERTEX];
            float weights[NUM_BONES_PER_VERTEX];
        };

	class FBXMesh
	{
        public:

            //Model(const std::string fileName, bool smooth = false);
            //FBXModel();
            ~FBXMesh();

            Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

            Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

            Microsoft::WRL::ComPtr<ID3D12Resource> boneBuff = nullptr;

            std::vector<TextureCell*> textures;
            //MCB::Texture textures;
            std::vector<FBXVertex> vertices;
            std::vector<unsigned short> indices;
            std::vector<VertexBoneData> bones;
            std::unordered_map<unsigned short int, std::vector<unsigned short int>>smoothData;

            unsigned int sizeVB = static_cast<unsigned int>(sizeof(FBXVertex) * vertices.size());
            unsigned int sizeIB = static_cast<unsigned int>(sizeof(unsigned short) * indices.size());
            unsigned int sizeBB = static_cast<unsigned int>(sizeof(VertexBoneData) * bones.size());

            D3D12_INDEX_BUFFER_VIEW ibView{};

            D3D12_VERTEX_BUFFER_VIEW vbView{};

 

            //D3D12_HEAP_PROPERTIES heapprop{};   // ヒープ設定

            //D3D12_RESOURCE_DESC resdesc{};  // リソース設定

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


