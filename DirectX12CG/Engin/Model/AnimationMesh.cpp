#include "AnimationMesh.h"
#include "Util.h"
//using namespace MCB;
//using namespace std;
//MCB::AnimationModel::AnimationModel()
//{
//    //material.Init();
//}
//

MCB::AnimationMesh::~AnimationMesh()
{
    for (TextureCell*& tex : textures_)
    {
        if (tex)
        {
            tex->free = true;
        }
    }
}

void MCB::AnimationMesh::CreateVertexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag,
    const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state)
{
    Dx12::GetInstance()->result_ = Dx12::GetInstance()->device_->CreateCommittedResource(
        &HeapProp, // ヒープ設定
        flag,
        &Resdesc, // リソース設定
        state,
        nullptr,
        IID_PPV_ARGS(&vertBuff_));
    assert(SUCCEEDED(Dx12::GetInstance()->result_));
}

void MCB::AnimationMesh::SetIbView(const DXGI_FORMAT& format)
{
    ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
    ibView_.Format = format;
    ibView_.SizeInBytes = static_cast<uint32_t>(sizeIB_);
}

void MCB::AnimationMesh::CreateIndexBuffer(const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag,
    const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state)
{
    Dx12::GetInstance()->result_ = Dx12::GetInstance()->device_->CreateCommittedResource(
        &HeapProp,
        flag,
        &Resdesc,
        state,
        nullptr,
        IID_PPV_ARGS(&indexBuff_)
    );

}

HRESULT MCB::AnimationMesh::IndexMaping()
{
    HRESULT result = S_OK;


    uint16_t* indexMap = nullptr;
    //GPU上のバッファに対応した仮想メモリを取得----------------------------
    result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
    //---------------------------------------

    std::copy(indices_.begin(), indices_.end(), indexMap);

    //繋がりを解除---------------------
    indexBuff_->Unmap(0, nullptr);
    //------------------------

    return result;
}

void MCB::AnimationMesh::SetVbView()
{
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = static_cast<uint32_t>(sizeVB_);
    vbView_.StrideInBytes = sizeof(vertices_[0]);
}

HRESULT MCB::AnimationMesh::VertexMaping()
{
    HRESULT result = S_OK;

    AnimationVertex* vertMap = nullptr;

    result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));

    std::copy(vertices_.begin(), vertices_.end(), vertMap);

    // マップを解除
    vertBuff_->Unmap(0, nullptr);

    return result;
}

void MCB::AnimationMesh::SetSizeIB()
{
    sizeIB_ = static_cast<size_t>(sizeof(uint16_t) * indices_.size());
}



void MCB::AnimationMesh::SetSizeVB()
{
    sizeVB_ = static_cast<size_t>(sizeof(AnimationVertex) * vertices_.size());
}

void MCB::AnimationMesh::Init()
{

    SetSizeIB();
    for (auto& itr : material_)
    {
        itr.SetIndex(D3D12_RESOURCE_DIMENSION_BUFFER, static_cast<uint32_t>(sizeIB_), 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);

        CreateIndexBuffer(itr.HeapProp_, D3D12_HEAP_FLAG_NONE, itr.Resdesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
    }
    Dx12::GetInstance()->result_ = IndexMaping();
    SetIbView(DXGI_FORMAT_R16_UINT);

    SetSizeVB();
    for (auto& itr2 : material_)
    {
        itr2.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER,static_cast<uint32_t>(sizeVB_), 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
        CreateVertexBuffer(itr2.HeapProp_, D3D12_HEAP_FLAG_NONE, itr2.Resdesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
    }
    VertexMaping();
    SetVbView();

    D3D12_RESOURCE_DESC ResdescFbx{};
    ResdescFbx.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    ResdescFbx.Width = (sizeof(ConstBuffSkin) + 0xff) & ~0xff;
    ResdescFbx.Height = 1;
    ResdescFbx.DepthOrArraySize = 1;
    ResdescFbx.MipLevels = 1;
    ResdescFbx.SampleDesc.Count = 1;
    ResdescFbx.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    D3D12_HEAP_PROPERTIES HeapProp{};
    HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    Dx12* dx12 = Dx12::GetInstance();
    dx12->result_ = dx12->device_->CreateCommittedResource
    (
        &HeapProp,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &ResdescFbx,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffSkin_)
    );

    assert(SUCCEEDED(dx12->result_));

    dx12->result_ = constBuffSkin_->Map(0, nullptr, (void**)&constMapSkin_);

}

size_t MCB::AnimationMesh::GetVertexCount()
{
	return vertices_.size();
}

//void MCB::AnimationModel::AddSmoothData(uint16_t indexPosition, uint16_t indexVertex)
//{
//    smoothData[indexPosition].emplace_back(indexVertex);
//}
//
//void MCB::AnimationModel::CalculateSmoothedVertexNormals()
//{
//    auto itr = smoothData.begin();
//    for (; itr != smoothData.end(); ++itr)
//    {
//        std::vector<uint16_t>& v = itr->second;
//        Vector3D normal = {};
//        for (uint16_t index : v)
//        {
//            normal += vertices[index].normal;
//        }
//        normal = normal / (float)v.size();
//        normal.V3Norm();
//        for (uint16_t index : v)
//        {
//            vertices[index].normal = { normal.vec.x, normal.vec.y, normal.vec.z };
//        }
//    }
//}
