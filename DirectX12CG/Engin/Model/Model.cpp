#include "Model.h"


using namespace std;

MCB::Model::Model( const std::string fileName,const bool& smooth)
{
    material_.Init();
    fileName_ = fileName;
    Init(fileName,smooth);
    material_.Update();
}

MCB::Model::Model()
{
    material_.Init();
}

MCB::Model::~Model()
{
    texture_->free = true;
    //texture.texfile.scratchImg.Release();
}

void MCB::Model::CreateVertexBuffer( const D3D12_HEAP_PROPERTIES& HeapProp,const D3D12_HEAP_FLAGS& flag,
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

void MCB::Model::SetIbView(const DXGI_FORMAT& format)
{
    ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
    ibView_.Format = format;
    ibView_.SizeInBytes = static_cast<uint32_t>(sizeIB_);
}

void MCB::Model::CreateIndexBuffer( const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag,
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

HRESULT MCB::Model::IndexMaping()
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

void MCB::Model::SetVbView()
{
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = static_cast<uint32_t>(sizeVB_);
    vbView_.StrideInBytes = sizeof(vertices_[0]);
}

HRESULT MCB::Model::VertexMaping()
{
    HRESULT result = S_OK;

    ObjectVertex* vertMap = nullptr;

    result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));

    std::copy(vertices_.begin(), vertices_.end(), vertMap);

    // マップを解除
    vertBuff_->Unmap(0, nullptr);

    return result;
}


void MCB::Model::CreateModel(const string& fileName,  bool smooth)
{
    std::ifstream file;

    std::vector<Float3> positions;//頂点座標
    std::vector<Float3> normals;//法線ベクトル
    std::vector<Float2> texcoords;//テクスチャUV




    //file.open(fileName);

    const string FileName = fileName + ".obj";
    const string directoryPath = "Resources\\" + fileName + "\\";
    file.open(directoryPath + FileName);
    if (file.fail())
    {
        assert(0 && "ObjectFileNotFound");
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
            line_stream >> position.x_;
            line_stream >> position.y_;
            line_stream >> position.z_;
            //座標データに追加
            positions.emplace_back(position);
            ////頂点データに追加
            //ObjectVertex vertex{};
            //vertex.pos = position;
            //vertices.emplace_back(vertex);
        }

        if (key == "vt")
        {
            Float2 texcoord{};
            line_stream >> texcoord.x_;
            line_stream >> texcoord.y_;

            texcoord.y_ = 1.0f - texcoord.y_;

            texcoords.emplace_back(texcoord);
        }

        if (key == "vn")
        {
            Float3 normal{};
            line_stream >> normal.x_;
            line_stream >> normal.y_;
            line_stream >> normal.z_;

            normals.emplace_back(normal);
        }

        if (key == "mtllib")
        {
            string filename;
            line_stream >> filename;
            LoadMaterial(directoryPath, filename);
        }

        if (key == "f")
        {

            string index_string;
            while (getline(line_stream, index_string, ' '))
            {
                istringstream index_stream(index_string);
                uint16_t indexPosition,indexTexcoord, indexNormal;
                index_stream >> indexPosition;
                index_stream.seekg(1, ios_base::cur);
                index_stream >> indexTexcoord;
                index_stream.seekg(1, ios_base::cur);
                index_stream >> indexNormal;


                ObjectVertex vertex{};
                vertex.pos = positions[indexPosition - 1];
                vertex.normal = normals[indexNormal - 1];
                vertex.uv = texcoords[indexTexcoord - 1];
                vertices_.emplace_back(vertex);
                indices_.emplace_back((uint16_t)indices_.size());
                if(smooth) AddSmoothData(indexPosition, (uint16_t)GetVertexCount() - 1);
                //if (smooth) CalculateSmoothedVertexNormals();

            }

        }

    }

    file.close();
    positions.clear();
    normals.clear();
    texcoords.clear();
    if (smooth) CalculateSmoothedVertexNormals();
}

void MCB::Model::SetSizeIB()
{
    sizeIB_ = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());
}



void MCB::Model::SetSizeVB()
{
    sizeVB_ = static_cast<uint32_t>(sizeof(ObjectVertex) * vertices_.size());
}

void MCB::Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
    std::ifstream file;

    file.open(directoryPath + filename);

    if (file.fail())
    {
        assert(0 && "MaterialFileNotFound");
    }

    string line;
    while (getline(file, line))
    {
        std::istringstream line_stream(line);

        string key;
        getline(line_stream, key, ' ');

        if (key[0] == '\t')
        {
            key.erase(key.begin());
        }

        if (key == "newmtl")
        {
            line_stream >> material_.material_.name;
        }

        if (key == "Ka")
        {
            line_stream >> material_.material_.ambient.x_;
            line_stream >> material_.material_.ambient.y_;
            line_stream >> material_.material_.ambient.z_;
        }

        if (key == "Kd")
        {
            line_stream >> material_.material_.diffuse.x_;
            line_stream >> material_.material_.diffuse.y_;
            line_stream >> material_.material_.diffuse.z_;
        }

        if (key == "Ks")
        {
            line_stream >> material_.material_.specular.x_;
            line_stream >> material_.material_.specular.y_;
            line_stream >> material_.material_.specular.z_;
        }

        if (key == "map_Kd")
        {
            line_stream >> material_.material_.textureFileName;

            texture_ = loader_->LoadTexture(directoryPath, material_.material_.textureFileName);
        }

    }
    file.close();

}

void MCB::Model::Init(const std::string& fileName, bool smooth)
{
    CreateModel(fileName,smooth);

    SetSizeIB();
    material_.SetIndex(D3D12_RESOURCE_DIMENSION_BUFFER, static_cast<uint32_t>(sizeIB_), 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
    CreateIndexBuffer(material_.HeapProp_, D3D12_HEAP_FLAG_NONE, material_.Resdesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
    Dx12::GetInstance()->result_ = IndexMaping();
    SetIbView(DXGI_FORMAT_R16_UINT);

    SetSizeVB();
    material_.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, static_cast<uint32_t>(sizeVB_), 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
    CreateVertexBuffer(material_.HeapProp_, D3D12_HEAP_FLAG_NONE, material_.Resdesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
    VertexMaping();
    SetVbView();

}

void MCB::Model::AddSmoothData( uint16_t indexPosition,  uint16_t indexVertex)
{
    smoothData_[indexPosition].emplace_back(indexVertex);
}

void MCB::Model::CalculateSmoothedVertexNormals()
{
    auto itr = smoothData_.begin();
    for (; itr != smoothData_.end(); ++itr)
    {
        std::vector<uint16_t>& v = itr->second;
        Vector3D normal = {};
        for (uint16_t index : v)
        {
            normal += vertices_[index].normal;
        }
        normal = normal / (float)v.size();
        normal.V3Norm();
        for (uint16_t index : v)
        {
            vertices_[index].normal = { normal.vec_.x_, normal.vec_.y_, normal.vec_.z_ };
        }
    }
}
