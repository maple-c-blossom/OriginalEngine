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
#include "Quaternion.h"
#include <unordered_map>
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

    typedef struct NodeAnim
    {
        std::string name;
        std::vector<MCB::Vector3D> position;
        std::vector<double> positionTime;
        std::vector<MCB::Quaternion> rotation;
        std::vector<double> rotationTime;
        std::vector<MCB::Vector3D> scale;
        std::vector<double> scaleTime;
    }NodeAnim;

    typedef struct Animation
    {
        std::string name;
        double duration;
        double ticksPerSecond;
        std::vector<NodeAnim> channels;
    }Animation;

    typedef struct Node
    {
        std::string name;
        std::vector<std::unique_ptr<AnimationMesh>> meshes; //出力先メッシュ配列
        DirectX::XMVECTOR scale = { 1,1,1,0 };
        DirectX::XMVECTOR rotation = { 0,0,0,0 };
        DirectX::XMVECTOR translation = { 0,0,0,1 };
        DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX globalTransform = DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX globalInverseTransform = DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX AnimaetionParentMat = DirectX::XMMatrixIdentity();
        Node* parent = nullptr;
        std::vector<Node*>children{};
        Vector3D endPosition;
        Vector3D startPosition;
        Vector3D boneVec;
        float boneLength;
        struct IKData
        {
            bool isIK = false;
            bool rotationInv = false;
            Vector3D iKTargetPosition = {};
            Vector3D constraintVector = {0,1,0};
        };
        IKData ikData;
    }Node;
    //struct aiMesh;
    //struct aiMaterial;

    class AnimationModel
    {
    private:
        using string = std::string;
        //ID3D12Device* device = nullptr;
        //void LoadMesh(Mesh& dst,const aiMesh* src,bool inversU, bool inverV);
    public:
        TextureManager* textureManager_ = TextureManager::GetInstance();
        std::vector<std::unique_ptr<Node>> nodes_;
        std::unordered_map<std::string,std::unique_ptr<Animation>> animations_;
        
        std::string prevAnimName_ = "NoAnimation";
        Animation* prevAnim = nullptr;
        ~AnimationModel();
        string fileName_;
        bool isDelete_ = false;
        bool Load( std::string fileName,const std::string& fileType = "gltf");
        void CopyNodesWithMeshes( aiNode* node,const aiScene* scene, Node* targetParent = nullptr);
        void processMesh(aiMesh* mesh, const aiScene* scene, AnimationMesh& tempmodel);
        std::vector<TextureCell*> loadMaterialTextures(aiMaterial* mat, const aiTextureType& type, const std::string& typeName, const aiScene* scene);

        void boneAnimTransform(  float& timeInSeconds,const std::string& currentAnimation = "Null", bool loop = true);

        void readAnimNodeHeirarchy(  float animationTime, Node* pNode, DirectX::XMMATRIX *parentTransform, const DirectX::XMMATRIX& globalInverseTransform, const std::string& currentAnimation = "Null");

        static const NodeAnim* findNodeAnim(const Animation* pAnimation, const std::string& NodeName);

        static void calcInterpolatedPosition(Vector3D& Out,  float AnimationTime, const NodeAnim* pNodeAnim);

        static void calcInterpolatedRotation(Quaternion& Out,  float AnimationTime, const NodeAnim* pNodeAnim);

        static void calcInterpolatedScaling(Vector3D& Out,  float AnimationTime, const NodeAnim* pNodeAnim);

        static size_t findPosition( float AnimationTime, const NodeAnim* pNodeAnim);

        static size_t findRotation( float AnimationTime, const NodeAnim* pNodeAnim);

        static size_t findScaling( float AnimationTime, const NodeAnim* pNodeAnim);

        void OneBoneIK(Node& joint);

        void TowBoneIK(Node& joint1, Node& joint2);

        //bool LoadFile(ImportSetting setting);
        void Draw();
    };
}


