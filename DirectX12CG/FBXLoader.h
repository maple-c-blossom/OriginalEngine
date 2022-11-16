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
namespace MCB
{

    //頂点データ構造体-------------------------------------
    typedef struct FbxVertex
    {
        Float3 pos;//xyz座標
        Float3 normal;//法線ベクトル
        Float2 uv;//uv座標
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
        const std::string fileName;//ファイルパス
        std::vector<Mesh> meshies; //出力先メッシュ配列
        bool inversU = false; //U座標反転フラグ
        bool inversV = false; //V座標反転フラグ
    }ImportSetting;

    typedef struct Node
    {
        std::string name;
        std::vector<FBXModel> meshes; //出力先メッシュ配列
        DirectX::XMVECTOR scale = { 1,1,1,0 };
        DirectX::XMVECTOR rotation = { 0,0,0,0 };
        DirectX::XMVECTOR translation = { 0,0,0,1 };
        DirectX::XMMATRIX transform;
        DirectX::XMMATRIX globalTransform;
        Node* parent = nullptr;

    }Node;
    //struct aiMesh;
    //struct aiMaterial;

    class AssimpLoader
    {
    private:
        using string = std::string;
        ID3D12Device* device = nullptr;
        //void LoadMesh(Mesh& dst,const aiMesh* src,bool inversU, bool inverV);
        std::vector<std::unique_ptr<Node>> nodes;
    public:

        static const string baseDirectory;
        static void DeleteInstance();
        static AssimpLoader* GetInstance();
        static AssimpLoader* GetInitInstance();
        void Initialize();
        void Finalize();
        bool Load(std::string fileName);
        void CopyNodesWithMeshes( aiNode node,const aiScene* scene, Node* targetParent = nullptr);
        FBXModel processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
        //bool LoadFile(ImportSetting setting);
        void Draw();
    };
}
