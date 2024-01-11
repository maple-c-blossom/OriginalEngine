#pragma once

#include "IgnoreWarning.h"
WarningIgnoreBegin

#include <string>
#include <memory>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <unordered_map>
WarningIgnoreEnd

#include "Dx12.h"
#include "ObjectMaterial.h"
#include "Texture.h"
#include "AnimationMesh.h"
#include "TextureManager.h"
#include "Quaternion.h"
#include "ICamera.h"
#include "Object3d.h"
#include "RayCollider.h"
#include <CollisionManager.h>

namespace MCB
{

PragmaPush
PragmaWarningNum(4324)
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
		Byte6 pad;
		ImportSetting(const ImportSetting&) = delete;
		ImportSetting& operator=( const ImportSetting&) =delete;
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
		std::string animationRootNode = "NULL";
    }Animation;

    typedef struct Node
    {

        std::string name;//ノードの名前
        std::vector<std::unique_ptr<AnimationMesh>> meshes; //出力先メッシュ配列
		int64_t pad;
		DirectX::XMVECTOR scale = { { 1,1,1,0 } };//ローカルのスケール情報
		DirectX::XMVECTOR rotation = { { 0,0,0,0 } };//ローカルの回転情報
		DirectX::XMVECTOR translation = { { 0,0,0,1 } };//ローカルの位置情報
		DirectX::XMMATRIX localTransform = { DirectX::XMMatrixIdentity() };//ローカルのsrtMatrix
		DirectX::XMMATRIX globalTransform = { DirectX::XMMatrixIdentity() };//Model空間のMatrix
		DirectX::XMMATRIX globalInverseTransform = { DirectX::XMMatrixIdentity() };//Model空間のMatrixの逆行列
		DirectX::XMMATRIX AnimaetionParentMat = { DirectX::XMMatrixIdentity() };//アニメーションするときに使用するModel空間のMatrix
		DirectX::XMVECTOR defaultScale = { { 0,0,0,1 } };//初期姿勢時のローカルスケール情報
		DirectX::XMVECTOR defaultRotation = { { 0,0,0,0 } };//初期姿勢時のローカル回転情報
		DirectX::XMVECTOR defaultLocalTranslation = { { 0,0,0,1 } };//初期姿勢時のローカル位置情報
		DirectX::XMMATRIX defaultModelTransform = { DirectX::XMMatrixIdentity() };//初期姿勢時のModel空間の行列
        Node* parent = nullptr;//親ノードへのポインタ
        std::vector<Node*>children{};//子ノードへのポインタ
		Vector3D worldPosition;
        Vector3D endPosition;//ボーンの先端のポジション(ローカル空間)
        Vector3D startPosition;//ボーンの根本のポジション(ローカル空間)
        Vector3D boneVec;//ボーンのベクトル(ローカル空間)
        Vector3D defaultBoneVec;//初期のボーンのベクトル(ローカル空間)
		Ray worldBoneRay;//ボーンをレイに見立てて当たり判定を行う。
        float boneLength;//ボーンの長さ
        bool updated = false;
        bool jointView = true;
		bool lineView = false;
		Byte1 pad4;
        PrimitiveFigure::Line boneLine;
        struct IKData
        {
            bool isIK = false;//IKを行うか（自分がendJointかどうか）
			bool isCollisionIk = false;
			bool triangleDraw = false;
			bool IkUseConstraintIsLocalFromRoot = false;
			Byte4 pad1;
            Node* middleJointNode = nullptr;//ミドルジョイントのポインタ
            Node* rootJointNode = nullptr;//ルートジョイントのポインタ
            Vector3D iKEffectorPosition = {};//IKのEffector位置(Obj空間)
            Vector3D constraintModelVector = {0,1,0};//PoleVector(Model空間)
            Vector3D constraintFromRoot = {0,1,0};//PoleVector(Model空間)
            Vector3D constraintWorldVector = {0,1,0};//PoleVector(ワールド空間)
			int64_t pad2;
            Object3d constraintObj;//PoleVector表示用のオブジェクト
            Vector3D effectorPosFromRoot = {};//PoleVector(Model空間)
            Vector3D effectorPosFromMiddle = {};//PoleVector(Model空間)
            Vector3D effectorWorldPos = {};//PoleVector(Model空間)
			Byte6 pad3;
			Byte6 pad4;
			PrimitiveFigure::Line triangleNormalLine;
            PrimitiveFigure::Line constraintLine;
            PrimitiveFigure::Line effectorVecFromRoot;
            PrimitiveFigure::Line effectorVecFromMiddle;
            PrimitiveFigure::Triangle rootAndEffectorAndConstraintTriangle;
            PrimitiveFigure::Triangle jointTriangle;
            Vector3D constraintLocalPositionFromRoot;
			Byte4 pad5;

        };

        struct IKDebugData
        {
            std::array<std::string, 3> triangleNodeName;
            std::array<Vector3D, 3> triangleNodeLocalVector;
            Vector3D defaultTriangleNormal;
            Vector3D taregetTriangleNormal;
            Vector3D nowTriangleNormal;
            Vector3D constraintModelVector;
            Vector3D constraintWorldVector;

			Byte4 pad1;
        };
        IKData ikData;//IKに関するデータ
        IKDebugData ikDebugData;
        bool lineColorEqualObject = false;
		Byte7 pad9;
        Float4 lineDefaultColor = {0.f,0.f,0.f,1.f};
		int32_t pad7;
        bool chengeObjectColor;

		Byte7 pad8;
		Byte4 pad10;
        std::unique_ptr<Object3d> object;//ジョイント表示用のオブジェクト

        void JointObjectMatrixUpdate(ICamera* camera,Object3d* Obj,Model* model,const Float3& scale = {1.0f,1.0f,1.0f});
        void JointObjectDraw();
        void JointLineDraw();
		Node(const Node&) = delete;
		Node();
		Node& operator=(const Node&) = delete;
    }Node;
    //struct aiMesh;
    //struct aiMaterial;
PragmaPop
    class Skeleton
    {
    private:
        std::vector< std::unique_ptr<Node>> nodes_;
		std::vector<Node*> ikNodes_;
        
    public:
        Node* rootNode;
     
        //アニメーションの補間に使うと予想して作っておく----------
        Animation* currentAnimation = nullptr;//現在Animation
        Animation* prevAnimation = nullptr;//前回Animation
        Animation* nextAnimation = nullptr;//次のAnimation
        //--------------

        //限定的に使用するNodesを直接取得する物(forで全参照したいときとか)
		std::vector< std::unique_ptr<Node>>* GetNodes_();
		Node* GetNode(std::string name);//与えられた名前のNodeを返す

		void SetNode(std::unique_ptr<Node> node);//Nodeを追加

        /// <summary>
        /// 与えられたポジションに近いNodeを返す
        /// </summary>
        /// <param name="targetPos">対象となるポジション</param>
        /// <param name="objectPositoin">モデルを保有しているオブジェクトのポジション(初めからMesh空間上の座標を渡す場合不要)</param>
        /// <param name="closestNum">何番目に近いノードが欲しいのか</param>
        /// <returns></returns>
        Node* GetNearPositionNode(const Vector3D& targetPos, const Vector3D& objectPositoin = {0,0,0}, uint32_t closestNum = 1);

        void boneAnimTransform(  float& timeInSeconds,Animation* currentAnimation = nullptr, Object3d* obj = nullptr,bool loop = true,bool animtionPositionRock = false);//Animation前の準備等

        void readAnimNodeHeirarchy(  float animationTime, Node* pNode,bool& animationPositionRock,Animation* currentAnimationPtr = nullptr);//実際に階層構造読み込んでAnimationの計算をする関数

        static const NodeAnim* findNodeAnim(const Animation* pAnimation, const std::string& NodeName);//どのNodeのキーフレームを見るのか
        //キーフレームの補完-------------
        static void calcInterpolatedPosition(Vector3D& Out,  float AnimationTime, const NodeAnim* pNodeAnim);

        static void calcInterpolatedRotation(Quaternion& Out,  float AnimationTime, const NodeAnim* pNodeAnim);

        static void calcInterpolatedScaling(Vector3D& Out,  float AnimationTime, const NodeAnim* pNodeAnim);
        //------------
        //キーフレームの値を検索し返す------------------------
        static size_t findPosition( float AnimationTime, const NodeAnim* pNodeAnim);

        static size_t findRotation( float AnimationTime, const NodeAnim* pNodeAnim);

        static size_t findScaling( float AnimationTime, const NodeAnim* pNodeAnim);
        //---------------------
        //IK関連の全面改修が必要そう(NodeがIKの対象かどうかの情報持ってるけどSkeletonが集約管理した方がいいっぽい）
        //1BoneIK(試作)
        void OneBoneIK(Node& joint);
        //2BoneIK(試作）
        void TwoBoneIK(Node& joint1);

		void AllNodeMatrixForModelToBone();
        /// <summary>
        /// CCDIK
        /// </summary>
        /// <param name="effectter">目標地点に向ける先端のNode</param>
        /// <param name="targetPos">目標地点</param>
        /// <param name="numMaxIteration">CCDの反復回数</param>
        /// <param name="errToleranceSq">誤差の許容数値(2乗）</param>
        void CCDIK(Node& effectter,Vector3D targetPos,int32_t numMaxIteration,float errToleranceSq);//理論理解段階のため未定義(引数も不十分の可能性あり)

        void SetCCDIK(Vector3D targetPos,Vector3D objPos);

		void SetTwoBoneIK(const Object3d& obj,const Vector3D& targetPos,const Vector3D& constraintPosition = { 0,1,0 },
			const std::string& boneName = "NULL",
			const std::string& middleJointName = "NULL",const std::string& rootJointName = "NULL",bool useConstraintFromEffector = false);

        void TwoBoneIKOff(const std::string& boneName);


        //関節の曲がる方向を制限(試作)
        void Vectorconstraiont(Node& joint);

        void ApplyRotation(Node& joint, const DirectX::XMFLOAT3& axis, float angle);

        void DrawHeirarchy(Node* node);
        void DrawIkNode();

        void UpdateNodeMatrix(Node* node);

        void JointObjectMatrixUpdate(ICamera* camera, Object3d* worldObjMatrix, Model* model, const Float3& scale = {1.0f,1.0f,1.0f});
        void JointObjectDraw();
        void JointLineDraw();
        Float4 lineDefaultColor = { 0.f,0.f,0.f,1.f };
    };

    //Model事にそのModelのアニメーションを管理する用のクラス(Model事よりSkeleton毎の方がいいか思案中)
    class AnimationManager
    {
		std::unordered_map<std::string,std::unique_ptr<Animation>> animations_;
    public:
		Animation* GetAnimation(std::string name); //アニメーションを取得

		void SetAnimation(std::unique_ptr<Animation> animation);//アニメーションを追加
		AnimationManager();
		AnimationManager(const AnimationManager&) = delete;
		AnimationManager& operator= (const AnimationManager&) = delete;
    };

    class AnimationModel
    {
    private:

        //ID3D12Device* device = nullptr;
        //void LoadMesh(Mesh& dst,const aiMesh* src,bool inversU, bool inverV);
    public:
        TextureManager* textureManager_ = TextureManager::GetInstance();
        Skeleton skeleton;//スケルトン情報
        AnimationManager animationManager;//アニメーション情報
        //CurrentAnimationの名前からAnimationを検索してskeletonにAnimationするように依頼する。(AnimationOrderって名前にした方が良い？)
		void AnimationUpdate(float& timeInSeconds,const std::string& currentAnimation = "Null",Object3d* obj = nullptr,bool loop = true,bool animtionPositionRock = false);
        ~AnimationModel();

		AnimationModel();
        std::string fileName_;//Modelのファイル名
        bool isDelete_ = false;//完全に削除していいかどうか

		private:
		Byte7 pad;
	public:
        bool Load( std::string fileName,const std::string& fileType = "gltf");//Modelデータをロード
        void CopyNodesWithMeshes( aiNode* node,const aiScene* scene, Node* targetParent = nullptr);//Nodeの階層構造並びにmeshの解析、抽出
        void processMesh(aiMesh* mesh, const aiScene* scene, AnimationMesh& tempmodel);//aiMesh内のデータを解析、抽出
        std::vector<TextureCell*> loadMaterialTextures(aiMaterial* mat, const aiTextureType& type, const std::string& typeName, const aiScene* scene);//Material内のTexture情報の解析,抽出
        void TwoBoneIkOrder(Object3d& objPos, Vector3D targetPos);
        void Draw();//Modelの描画
        void DrawHeirarchy();
		AnimationModel(const AnimationModel&) = delete;
		AnimationModel& operator=(const AnimationModel&) = delete;
    };
}


