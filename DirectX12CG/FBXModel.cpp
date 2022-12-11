#include "FBXModel.h"
#include "Util.h"
//using namespace MCB;
//using namespace std;
//MCB::FBXModel::FBXModel()
//{
//    //material.Init();
//}
//
MCB::FBXMesh::~FBXMesh()
{
    TextureManager* instance = TextureManager::GetInstance();
    for (auto& itr : textures)
    {
        itr->free = true;;
    }
}

void MCB::FBXMesh::CreateVertexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
{
    Dx12::GetInstance()->result = Dx12::GetInstance()->device->CreateCommittedResource(
        &HeapProp, // ヒープ設定
        flag,
        &Resdesc, // リソース設定
        state,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(Dx12::GetInstance()->result));
}

void MCB::FBXMesh::SetIbView(DXGI_FORMAT format)
{
    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
    ibView.Format = format;
    ibView.SizeInBytes = sizeIB;
}

void MCB::FBXMesh::CreateIndexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
{
    Dx12::GetInstance()->result = Dx12::GetInstance()->device->CreateCommittedResource(
        &HeapProp,
        flag,
        &Resdesc,
        state,
        nullptr,
        IID_PPV_ARGS(&indexBuff)
    );

}

HRESULT MCB::FBXMesh::IndexMaping()
{
    HRESULT result = S_OK;


    uint16_t* indexMap = nullptr;
    //GPU上のバッファに対応した仮想メモリを取得----------------------------
    result = indexBuff->Map(0, nullptr, (void**)&indexMap);
    //---------------------------------------

    std::copy(indices.begin(), indices.end(), indexMap);

    //繋がりを解除---------------------
    indexBuff->Unmap(0, nullptr);
    //------------------------

    return result;
}

void MCB::FBXMesh::SetVbView()
{
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeVB;
    vbView.StrideInBytes = sizeof(vertices[0]);
}

HRESULT MCB::FBXMesh::VertexMaping()
{
    HRESULT result = S_OK;

    FBXVertex* vertMap = nullptr;

    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));

    std::copy(vertices.begin(), vertices.end(), vertMap);

    // マップを解除
    vertBuff->Unmap(0, nullptr);

    return result;
}

void MCB::FBXMesh::SetSizeIB()
{
    sizeIB = static_cast<unsigned int>(sizeof(unsigned short) * indices.size());
}



void MCB::FBXMesh::SetSizeVB()
{
    sizeVB = static_cast<unsigned int>(sizeof(FBXVertex) * vertices.size());
}

void MCB::FBXMesh::Init()
{

    SetSizeIB();
    for (auto& itr : material)
    {
        itr.SetIndex(D3D12_RESOURCE_DIMENSION_BUFFER, sizeIB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
        CreateIndexBuffer(itr.HeapProp, D3D12_HEAP_FLAG_NONE, itr.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

    }
    Dx12::GetInstance()->result = IndexMaping();
    SetIbView(DXGI_FORMAT_R16_UINT);

    SetSizeVB();
    for (auto& itr : material)
    {
        itr.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, sizeVB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
        CreateVertexBuffer(itr.HeapProp, D3D12_HEAP_FLAG_NONE, itr.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);
    }
    VertexMaping();
    SetVbView();

}

//void MCB::FBXModel::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
//{
//    smoothData[indexPosition].emplace_back(indexVertex);
//}
//
//void MCB::FBXModel::CalculateSmoothedVertexNormals()
//{
//    auto itr = smoothData.begin();
//    for (; itr != smoothData.end(); ++itr)
//    {
//        std::vector<unsigned short>& v = itr->second;
//        Vector3D normal = {};
//        for (unsigned short index : v)
//        {
//            normal += vertices[index].normal;
//        }
//        normal = normal / (float)v.size();
//        normal.V3Norm();
//        for (unsigned short index : v)
//        {
//            vertices[index].normal = { normal.vec.x, normal.vec.y, normal.vec.z };
//        }
//    }
//}
