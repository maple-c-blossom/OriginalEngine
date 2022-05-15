#include "ObjVertex.h"
#

using namespace std;

void MCB::ObjVertex::CreateVertexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
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

void MCB::ObjVertex::SetIbView(DXGI_FORMAT format)
{
    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
    ibView.Format = format;
    ibView.SizeInBytes = sizeIB;
}

void MCB::ObjVertex::CreateIndexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
{
    dx12.result = dx12.device->CreateCommittedResource(
        &HeapProp,
        flag,
        &Resdesc,
        state,
        nullptr,
        IID_PPV_ARGS(&indexBuff)
    );

}

HRESULT MCB::ObjVertex::IndexMaping()
{
    HRESULT result = S_OK;
    //GPU上のバッファに対応した仮想メモリを取得----------------------------
    result = indexBuff->Map(0, nullptr, (void**)&indexMap);
    //---------------------------------------
    
    std::copy(indices.begin(), indices.end(), indexMap);
    
    //繋がりを解除---------------------
    indexBuff->Unmap(0, nullptr);
    //------------------------

    return result;
}

void MCB::ObjVertex::SetVbView()
{
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeVB;
    vbView.StrideInBytes = sizeof(vertices[0]);
}

HRESULT MCB::ObjVertex::VertexMaping()
{
    HRESULT result = S_OK;

    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));

    std::copy(vertices.begin(), vertices.end(), vertMap);

    // マップを解除
    vertBuff->Unmap(0, nullptr);

    return result;
}

void MCB::ObjVertex::CreateModel(const char* fileName)
{
    std::ifstream file;

    std::vector<Float3> positions;//頂点座標
    std::vector<Float3> normals;//法線ベクトル
    std::vector<Float3> texcoords;//テクスチャUV




    file.open(fileName);
    if (file.fail())
    {
        assert(0);
    }


    string line;
    while (getline(file, line))
    {
        std::istringstream line_stream(line);

        string key;
        getline(line_stream, key, ' ');
        if (key == "v")
        {
            //各座標読み込み
            Float3 position{};
            line_stream >> position.x;
            line_stream >> position.y;
            line_stream >> position.z;
            //座標データに追加
            positions.emplace_back(position);
            //頂点データに追加
            ObjectVertex vertex{};
            vertex.pos = position;
            vertices.emplace_back(vertex);
        }

        if (key == "f")
        {

            string index_string;
            while (getline(line_stream, index_string, ' '));
            {
                istringstream index_stream(index_string);
                unsigned short indexPosition;
                index_stream >> indexPosition;

                indices.emplace_back(indexPosition - 1);
            }

        }

    }

    file.close();

}

void MCB::ObjVertex::SetSizeIB()
{
    sizeIB = static_cast<unsigned int>(sizeof(unsigned short) * indices.size());
}

void MCB::ObjVertex::SetSizeVB()
{
    sizeVB = static_cast<unsigned int>(sizeof(ObjectVertex) * vertices.size());
}