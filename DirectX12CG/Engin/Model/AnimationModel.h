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
#include "AnimationMesh.h"
#include "TextureManager.h"
#include "Quaternion.h"
#include <unordered_map>
#include "ICamera.h"
#include "Object3d.h"
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
        std::string name;//�m�[�h�̖��O
        std::vector<std::unique_ptr<AnimationMesh>> meshes; //�o�͐惁�b�V���z��
        DirectX::XMVECTOR scale = { 1,1,1,0 };//���[�J���̃X�P�[�����
        DirectX::XMVECTOR rotation = { 0,0,0,0 };//���[�J���̉�]���
        DirectX::XMVECTOR translation = { 0,0,0,1 };//���[�J���̈ʒu���
        DirectX::XMMATRIX localTransform = DirectX::XMMatrixIdentity();//���[�J����srtMatrix
        DirectX::XMMATRIX globalTransform = DirectX::XMMatrixIdentity();//Model��Ԃ�Matrix
        DirectX::XMMATRIX globalInverseTransform = DirectX::XMMatrixIdentity();//Model��Ԃ�Matrix�̋t�s��
        DirectX::XMMATRIX AnimaetionParentMat = DirectX::XMMatrixIdentity();//�A�j���[�V��������Ƃ��Ɏg�p����Model��Ԃ�Matrix
        DirectX::XMVECTOR defaultScale = { 0,0,0,1 };//�����p�����̃��[�J���X�P�[�����
        DirectX::XMVECTOR defaultRotation = { 0,0,0,0 };//�����p�����̃��[�J����]���
        DirectX::XMVECTOR defaultLocalTranslation = { 0,0,0,1 };//�����p�����̃��[�J���ʒu���
        DirectX::XMMATRIX defaultModelTransform = DirectX::XMMatrixIdentity();//�����p������Model��Ԃ̍s��
        Node* parent = nullptr;//�e�m�[�h�ւ̃|�C���^
        std::vector<Node*>children{};//�q�m�[�h�ւ̃|�C���^
        Vector3D endPosition;//�{�[���̐�[�̃|�W�V����(���[�J�����)
        Vector3D startPosition;//�{�[���̍��{�̃|�W�V����(���[�J�����)
        Vector3D boneVec;//�{�[���̃x�N�g��(���[�J�����)
        Vector3D defaultBoneVec;//�����̃{�[���̃x�N�g��(���[�J�����)
        float boneLength;//�{�[���̒���
        bool updated = false;
        struct IKData
        {
            bool isIK = false;//IK���s����
            Vector3D iKEffectorPosition = {};//IK��Effector�ʒu(���[���h���)
            Vector3D constraintVector = {0,1,0};//PoleVector(���[���h���)
            Object3d constraintObj;//PoleVector�\���p�̃I�u�W�F�N�g
        };
        IKData ikData;//IK�Ɋւ���f�[�^
        std::unique_ptr<Object3d> object;//�W���C���g�\���p�̃I�u�W�F�N�g
        void JointObjectMatrixUpdate(ICamera* camera,Object3d* Obj,Model* model);
        void JointObjectDraw();
    }Node;
    //struct aiMesh;
    //struct aiMaterial;

    class Skeleton
    {
    private:
        std::vector< std::unique_ptr<Node>> nodes_;
        
    public:
        Node* rootNode;
     
        //�A�j���[�V�����̕�ԂɎg���Ɨ\�z���č���Ă���----------
        Animation* currentAnimation = nullptr;//����Animation
        Animation* prevAnimation = nullptr;//�O��Animation
        Animation* nextAnimation = nullptr;//����Animation
        //--------------

        //����I�Ɏg�p����Nodes�𒼐ڎ擾���镨(for�őS�Q�Ƃ������Ƃ��Ƃ�)
        std::vector< std::unique_ptr<Node>>* GetNodes_()
        {
            return &nodes_;
        };
        Node* GetNode(std::string name)//�^����ꂽ���O��Node��Ԃ�
        {
            for (auto& node : nodes_)
            {
                if (node->name == name)
                {
                    return node.get();
                }
            }
            return nullptr; 
        }
        void SetNode(std::unique_ptr<Node> node)//Node��ǉ�
        {
            nodes_.push_back(std::move(node));
        }
        /// <summary>
        /// �^����ꂽ�|�W�V�����ɋ߂�Node��Ԃ�
        /// </summary>
        /// <param name="targetPos">�ΏۂƂȂ�|�W�V����</param>
        /// <param name="objectPositoin">���f����ۗL���Ă���I�u�W�F�N�g�̃|�W�V����(���߂���Mesh��ԏ�̍��W��n���ꍇ�s�v)</param>
        /// <param name="closestNum">���Ԗڂɋ߂��m�[�h���~�����̂�</param>
        /// <returns></returns>
        Node* GetNearPositionNode(const Vector3D& targetPos, const Vector3D& objectPositoin = {0,0,0}, uint32_t closestNum = 1);

        void boneAnimTransform(  float& timeInSeconds,Animation* currentAnimation = nullptr, bool loop = true);//Animation�O�̏�����

        void readAnimNodeHeirarchy(  float animationTime, Node* pNode, Animation* currentAnimation = nullptr);//���ۂɊK�w�\���ǂݍ����Animation�̌v�Z������֐�

        static const NodeAnim* findNodeAnim(const Animation* pAnimation, const std::string& NodeName);//�ǂ�Node�̃L�[�t���[��������̂�
        //�L�[�t���[���̕⊮-------------
        static void calcInterpolatedPosition(Vector3D& Out,  float AnimationTime, const NodeAnim* pNodeAnim);

        static void calcInterpolatedRotation(Quaternion& Out,  float AnimationTime, const NodeAnim* pNodeAnim);

        static void calcInterpolatedScaling(Vector3D& Out,  float AnimationTime, const NodeAnim* pNodeAnim);
        //------------
        //�L�[�t���[���̒l���������Ԃ�------------------------
        static size_t findPosition( float AnimationTime, const NodeAnim* pNodeAnim);

        static size_t findRotation( float AnimationTime, const NodeAnim* pNodeAnim);

        static size_t findScaling( float AnimationTime, const NodeAnim* pNodeAnim);
        //---------------------
        //IK�֘A�̑S�ʉ��C���K�v����(Node��IK�̑Ώۂ��ǂ����̏�񎝂��Ă邯��Skeleton���W��Ǘ����������������ۂ��j
        //1BoneIK(����)
        void OneBoneIK(Node& joint);
        //2BoneIK(����j
        void TwoBoneIK(Node& joint1, Node& joint2);

        /// <summary>
        /// CCDIK
        /// </summary>
        /// <param name="effectter">�ڕW�n�_�Ɍ������[��Node</param>
        /// <param name="targetPos">�ڕW�n�_</param>
        /// <param name="numMaxIteration">CCD�̔�����</param>
        /// <param name="errToleranceSq">�덷�̋��e���l(2��j</param>
        void CCDIK(Node& effectter,Vector3D targetPos,int numMaxIteration,float errToleranceSq);//���_����i�K�̂��ߖ���`(�������s�\���̉\������)

        void SetCCDIK(Vector3D targetPos,Vector3D objPos);

        void SetTwoBoneIK(Vector3D objPos, Vector3D targetPos, Vector3D constraintPosition = {0,1,0}, std::string boneName = "NULL");
        //�֐߂̋Ȃ�������𐧌�(����)
        void Vectorconstraiont(Node& joint);

        void ApplyRotation(Node& joint, const DirectX::XMFLOAT3& axis, float angle);

        void DrawHeirarchy(Node* node);

        void UpdateNodeMatrix(Node* node);

        void JointObjectMatrixUpdate(ICamera* camera, Object3d* worldObjMatrix, Model* model);
        void JointObjectDraw();
    };

    //Model���ɂ���Model�̃A�j���[�V�������Ǘ�����p�̃N���X(Model�����Skeleton���̕����������v�Ē�)
    class AnimationManager
    {
        std::unordered_map<std::string, std::unique_ptr<Animation>> animations_;
    public:
        Animation* GetAnimation(std::string name) //�A�j���[�V�������擾
        {
            auto& itr = animations_.find(name);
            if (itr == animations_.end())
            {
                if (!animations_.empty()) return animations_.begin()->second.get();
                return nullptr;
            }
            return animations_[name].get();

        };
        void SetAnimation(std::unique_ptr<Animation> animation) {
            animations_[animation->name] = std::move(animation);
        }//�A�j���[�V������ǉ�
    };

    class AnimationModel
    {
    private:

        //ID3D12Device* device = nullptr;
        //void LoadMesh(Mesh& dst,const aiMesh* src,bool inversU, bool inverV);
    public:
        TextureManager* textureManager_ = TextureManager::GetInstance();
        Skeleton skeleton;//�X�P���g�����
        AnimationManager animationManager;//�A�j���[�V�������
        //CurrentAnimation�̖��O����Animation����������skeleton��Animation����悤�Ɉ˗�����B(AnimationOrder���Ė��O�ɂ��������ǂ��H)
        void AnimationUpdate(float& timeInSeconds, const std::string& currentAnimation = "Null", bool loop = true);
        ~AnimationModel();
        std::string fileName_;//Model�̃t�@�C����
        bool isDelete_ = false;//���S�ɍ폜���Ă������ǂ���
        bool Load( std::string fileName,const std::string& fileType = "gltf");//Model�f�[�^�����[�h
        void CopyNodesWithMeshes( aiNode* node,const aiScene* scene, Node* targetParent = nullptr);//Node�̊K�w�\�����т�mesh�̉�́A���o
        void processMesh(aiMesh* mesh, const aiScene* scene, AnimationMesh& tempmodel);//aiMesh���̃f�[�^����́A���o
        std::vector<TextureCell*> loadMaterialTextures(aiMaterial* mat, const aiTextureType& type, const std::string& typeName, const aiScene* scene);//Material����Texture���̉��,���o
        void TwoBoneIkOrder(Vector3D objPos, Vector3D targetPos);
        void Draw();//Model�̕`��
        void DrawHeirarchy();
    };
}


