#include "Vertex.h"
#include <DirectXMath.h>

using namespace DirectX;

void MCB::Vertex::CreateVertexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
{
    dx12.result = dx12.device->CreateCommittedResource(
        &HeapProp, // ヒープ設定
        flag,
        &Resdesc, // リソース設定
        state,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(dx12.result));
}

//void MCB::Vertex::SetIbView(DXGI_FORMAT format)
//{
//    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
//    ibView.Format = format;
//    ibView.SizeInBytes = sizeIB;
//}

void MCB::Vertex::SetVbView()
{
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeVB;
    vbView.StrideInBytes = sizeof(vertices[0]);
}

//void MCB::Vertex::CreateIndexBuffer(Dx12 &dx12,const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag,const D3D12_RESOURCE_DESC Resdesc ,D3D12_RESOURCE_STATES state)
//{
//    dx12.result = dx12.device->CreateCommittedResource(
//        &HeapProp,
//        flag,
//        &Resdesc,
//        state,
//        nullptr,
//        IID_PPV_ARGS(&indexBuff)
//    );
//
//}

//void MCB::Vertex::CalculationNormalVec()
//{
//    for (int i = 0; i < _countof(boxIndices) / 3; i++)
//    {
//        //三角形1つごとに計算
//
//        //三角形のインデックスを取り出して、一時的な変数に入れる
//        unsigned short index0 = boxIndices[i * 3 + 0];
//        unsigned short index1 = boxIndices[i * 3 + 1];
//        unsigned short index2 = boxIndices[i * 3 + 2];
//
//        //三角形を構成する頂点座標
//        XMVECTOR p0 = XMLoadFloat3(&Box[index0].pos);
//        XMVECTOR p1 = XMLoadFloat3(&Box[index1].pos);
//        XMVECTOR p2 = XMLoadFloat3(&Box[index2].pos);
//
//        //p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
//        XMVECTOR v1 = XMVectorSubtract(p1, p0);
//        XMVECTOR v2 = XMVectorSubtract(p2, p0);
//
//        //外積は両方から垂直なベクトル
//        XMVECTOR normal = XMVector3Cross(v1, v2);
//
//        //正規化（長さを一にする)
//        normal = XMVector3Normalize(normal);
//
//        //求めた法線を頂点データに代入
//        XMStoreFloat3(&Box[index0].normal, normal);
//        XMStoreFloat3(&Box[index1].normal, normal);
//        XMStoreFloat3(&Box[index2].normal, normal);
//
//    }
//}

//HRESULT MCB::Vertex::IndexMaping()
//{
//    HRESULT result;
//    //GPU上のバッファに対応した仮想メモリを取得----------------------------
//    result = indexBuff->Map(0, nullptr, (void**)&indexMap);
//    //---------------------------------------
//
//    //全インデックスに対して-------------------------
//    for (int i = 0; i < _countof(boxIndices); i++)
//    {
//        indexMap[i] = boxIndices[i];
//    }
//    //-----------------------
//
//    //繋がりを解除---------------------
//    indexBuff->Unmap(0, nullptr);
//    //------------------------
//
//    return result;
//}

HRESULT MCB::Vertex::VertexMaping()
{
    HRESULT result;

    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));

    // 全頂点に対して
    for (int i = 0; i < _countof(vertices); i++)
    {
        vertMap[i] = vertices[i];   // 座標をコピー
    }

    // マップを解除
    vertBuff->Unmap(0, nullptr);

    return result;
}


