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
#include "TextureManager.h"
#include <unordered_map>
#include "Vector3D.h"
namespace MCB
{
    //頂点データ構造体-------------------------------------
    typedef struct ObjectVertex
    {
        Float3 pos;//xyz座標
        Float3 normal;//法線ベクトル
        Float2 uv;//uv座標
    }ObjectVertex;
    //--------------------------------------
    class Model
    {
    public:

        Model(const std::string fileName, bool smooth = false);
        Model();
        ~Model();

        Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;




        TextureManager* Loader = TextureManager::GetInstance();
        TextureCell* texture;
        std::vector<ObjectVertex> vertices;
        std::vector<uint16_t> indices;
        std::unordered_map<uint16_t, std::vector<uint16_t>>smoothData;

        size_t sizeVB = static_cast<size_t>(sizeof(ObjectVertex) * vertices.size());
        size_t sizeIB = static_cast<size_t>(sizeof(uint16_t) * indices.size());

        D3D12_INDEX_BUFFER_VIEW ibView{};

        D3D12_VERTEX_BUFFER_VIEW vbView{};

        //D3D12_HEAP_PROPERTIES heapprop{};   // ヒープ設定

        //D3D12_RESOURCE_DESC resdesc{};  // リソース設定

        ObjectMaterial material;

        void CreateVertexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag, const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state);

        void SetIbView(DXGI_FORMAT format);

        void SetVbView();

        void CreateIndexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag, const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state);

        HRESULT IndexMaping();

        HRESULT VertexMaping();

        void CreateModel(const std::string& fileName,const bool& smooth = false);

        void SetSizeIB();

        void SetSizeVB();

        void LoadMaterial(const std::string& directoryPath,const std::string& filename);

        void Init(const std::string& fileName, const bool& smooth = false);
        
        inline size_t GetVertexCount() { return vertices.size(); }

        void AddSmoothData(const uint16_t& indexPosition, const uint16_t& indexVertex);

        void CalculateSmoothedVertexNormals();
    };

}


