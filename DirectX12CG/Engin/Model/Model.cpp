#include "Model.h"


using namespace std;

MCB::Model::Model( const std::string fileName,bool smooth)
{
    material.Init();
    Init(fileName,smooth);
    material.Update();
}

MCB::Model::Model()
{
    material.Init();
}

MCB::Model::~Model()
{
    texture->free = true;
    //texture.texfile.scratchImg.Release();
}

void MCB::Model::CreateVertexBuffer( const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
{
    Dx12::GetInstance()->result = Dx12::GetInstance()->device->CreateCommittedResource(
        &HeapProp, // �q�[�v�ݒ�
        flag,
        &Resdesc, // ���\�[�X�ݒ�
        state,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(Dx12::GetInstance()->result));
}

void MCB::Model::SetIbView(DXGI_FORMAT format)
{
    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
    ibView.Format = format;
    ibView.SizeInBytes = sizeIB;
}

void MCB::Model::CreateIndexBuffer( const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
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

HRESULT MCB::Model::IndexMaping()
{
    HRESULT result = S_OK;


    uint16_t* indexMap = nullptr;
    //GPU��̃o�b�t�@�ɑΉ��������z���������擾----------------------------
    result = indexBuff->Map(0, nullptr, (void**)&indexMap);
    //---------------------------------------
    
    std::copy(indices.begin(), indices.end(), indexMap);
    
    //�q���������---------------------
    indexBuff->Unmap(0, nullptr);
    //------------------------

    return result;
}

void MCB::Model::SetVbView()
{
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeVB;
    vbView.StrideInBytes = sizeof(vertices[0]);
}

HRESULT MCB::Model::VertexMaping()
{
    HRESULT result = S_OK;

    ObjectVertex* vertMap = nullptr;

    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));

    std::copy(vertices.begin(), vertices.end(), vertMap);

    // �}�b�v������
    vertBuff->Unmap(0, nullptr);

    return result;
}


void MCB::Model::CreateModel(const string fileName, bool smooth)
{
    std::ifstream file;

    std::vector<Float3> positions;//���_���W
    std::vector<Float3> normals;//�@���x�N�g��
    std::vector<Float2> texcoords;//�e�N�X�`��UV




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
            //�e���W�ǂݍ���
            Float3 position{};
            line_stream >> position.x;
            line_stream >> position.y;
            line_stream >> position.z;
            //���W�f�[�^�ɒǉ�
            positions.emplace_back(position);
            ////���_�f�[�^�ɒǉ�
            //ObjectVertex vertex{};
            //vertex.pos = position;
            //vertices.emplace_back(vertex);
        }

        if (key == "vt")
        {
            Float2 texcoord{};
            line_stream >> texcoord.x;
            line_stream >> texcoord.y;

            texcoord.y = 1.0f - texcoord.y;

            texcoords.emplace_back(texcoord);
        }

        if (key == "vn")
        {
            Float3 normal{};
            line_stream >> normal.x;
            line_stream >> normal.y;
            line_stream >> normal.z;

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
                unsigned short indexPosition,indexTexcoord, indexNormal;
                index_stream >> indexPosition;
                index_stream.seekg(1, ios_base::cur);
                index_stream >> indexTexcoord;
                index_stream.seekg(1, ios_base::cur);
                index_stream >> indexNormal;


                ObjectVertex vertex{};
                vertex.pos = positions[indexPosition - 1];
                vertex.normal = normals[indexNormal - 1];
                vertex.uv = texcoords[indexTexcoord - 1];
                vertices.emplace_back(vertex);
                indices.emplace_back((unsigned short)indices.size());
                if(smooth) AddSmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
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
    sizeIB = static_cast<unsigned int>(sizeof(unsigned short) * indices.size());
}



void MCB::Model::SetSizeVB()
{
    sizeVB = static_cast<unsigned int>(sizeof(ObjectVertex) * vertices.size());
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
            line_stream >> material.material.name;
        }

        if (key == "Ka")
        {
            line_stream >> material.material.ambient.x;
            line_stream >> material.material.ambient.y;
            line_stream >> material.material.ambient.z;
        }

        if (key == "Kd")
        {
            line_stream >> material.material.diffuse.x;
            line_stream >> material.material.diffuse.y;
            line_stream >> material.material.diffuse.z;
        }

        if (key == "Ks")
        {
            line_stream >> material.material.specular.x;
            line_stream >> material.material.specular.y;
            line_stream >> material.material.specular.z;
        }

        if (key == "map_Kd")
        {
            line_stream >> material.material.textureFileName;

            texture = Loader->LoadTexture(directoryPath, material.material.textureFileName);
        }

    }
    file.close();

}

void MCB::Model::Init(const std::string fileName, bool smooth)
{
    CreateModel(fileName,smooth);

    SetSizeIB();
    material.SetIndex(D3D12_RESOURCE_DIMENSION_BUFFER, sizeIB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
    CreateIndexBuffer(material.HeapProp, D3D12_HEAP_FLAG_NONE, material.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);
    Dx12::GetInstance()->result = IndexMaping();
    SetIbView(DXGI_FORMAT_R16_UINT);

    SetSizeVB();
    material.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, sizeVB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
    CreateVertexBuffer(material.HeapProp, D3D12_HEAP_FLAG_NONE, material.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);
    VertexMaping();
    SetVbView();

}

void MCB::Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
    smoothData[indexPosition].emplace_back(indexVertex);
}

void MCB::Model::CalculateSmoothedVertexNormals()
{
    auto itr = smoothData.begin();
    for (; itr != smoothData.end(); ++itr)
    {
        std::vector<unsigned short>& v = itr->second;
        Vector3D normal = {};
        for (unsigned short index : v)
        {
            normal += vertices[index].normal;
        }
        normal = normal / (float)v.size();
        normal.V3Norm();
        for (unsigned short index : v)
        {
            vertices[index].normal = { normal.vec.x, normal.vec.y, normal.vec.z };
        }
    }
}
