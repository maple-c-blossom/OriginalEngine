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
#include <string>
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

        Model(const std::string fileName,const bool& smooth = false);
        Model();
        ~Model();

        Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;

        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;

        bool isDelete_ = false;
        std::string fileName_;

        TextureManager* loader_ = TextureManager::GetInstance();
        TextureCell* texture_;
        std::vector<ObjectVertex> vertices_;
        std::vector<uint16_t> indices_;
        std::unordered_map<uint16_t, std::vector<uint16_t>>smoothData_;

        size_t sizeVB_ = static_cast<size_t>(sizeof(ObjectVertex) * vertices_.size());
        size_t sizeIB_ = static_cast<size_t>(sizeof(uint16_t) * indices_.size());

        D3D12_INDEX_BUFFER_VIEW ibView_{};

        D3D12_VERTEX_BUFFER_VIEW vbView_{};

        //D3D12_HEAP_PROPERTIES heapprop{};   // ヒープ設定

        //D3D12_RESOURCE_DESC resdesc{};  // リソース設定

        ObjectMaterial material_;

        void CreateVertexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag, const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state);

        void SetIbView(const DXGI_FORMAT& format);

        void SetVbView();

        void CreateIndexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag, const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state);

        HRESULT IndexMaping();

        HRESULT VertexMaping();

        void CreateModel(const std::string& fileName, bool smooth = false);

        void SetSizeIB();

        void SetSizeVB();

        void LoadMaterial(const std::string& directoryPath,const std::string& filename);

        void Init(const std::string& fileName,  bool smooth = false);
        
        inline size_t GetVertexCount() { return vertices_.size(); }

        void AddSmoothData( uint16_t indexPosition,  uint16_t indexVertex);

        void CalculateSmoothedVertexNormals();
    };

}


