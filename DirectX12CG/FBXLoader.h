#pragma once
#include "Dx12.h"
#include <string>
#include <memory>
#include "ObjectMaterial.h"
#include "Texture.h"
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "FBXModel.h"
#include "TextureManager.h"
namespace MCB
{

    //���_�f�[�^�\����-------------------------------------
    typedef struct FbxVertex
    {
        Float3 pos;//xyz���W
        Float3 normal;//�@���x�N�g��
        Float2 uv;//uv���W
    }FbxVertex;
    //--------------------------------------
    typedef struct Mesh
    {
        std::vector<FbxVertex> vertices;
        std::vector<uint32_t> indices;
        std::wstring DiffuseMap;
    }Mesh;


    typedef struct ImportSetting
    {
        const std::string fileName;//�t�@�C���p�X
        std::vector<Mesh> meshies; //�o�͐惁�b�V���z��
        bool inversU = false; //U���W���]�t���O
        bool inversV = false; //V���W���]�t���O
    }ImportSetting;

    typedef struct Node
    {
        std::string name;
        std::vector<FBXMesh> meshes; //�o�͐惁�b�V���z��
        DirectX::XMVECTOR scale = { 1,1,1,0 };
        DirectX::XMVECTOR rotation = { 0,0,0,0 };
        DirectX::XMVECTOR translation = { 0,0,0,1 };
        DirectX::XMMATRIX transform;
        DirectX::XMMATRIX globalTransform;
        Node* parent = nullptr;

    }Node;
    //struct aiMesh;
    //struct aiMaterial;

    class FBXModel
    {
    private:
        using string = std::string;
        //ID3D12Device* device = nullptr;
        //void LoadMesh(Mesh& dst,const aiMesh* src,bool inversU, bool inverV);
    public:
        TextureManager* textureManager = TextureManager::GetInstance();
        std::vector<std::unique_ptr<Node>> nodes;
        ~FBXModel();
        string fileName;
        bool Load(std::string fileName);
        void CopyNodesWithMeshes( aiNode* node,const aiScene* scene, Node* targetParent = nullptr);
        FBXMesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<TextureCell*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
        //bool LoadFile(ImportSetting setting);
        void Draw();
    };
}


