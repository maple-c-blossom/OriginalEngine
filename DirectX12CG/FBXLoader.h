#pragma once
#include "Dx12.h"
#include <string>
#include <memory>
#include "ObjectMaterial.h"
#include "Texture.h"
namespace MCB
{
    //頂点データ構造体-------------------------------------
    typedef struct FbxVertex
    {
        Float3 pos;//xyz座標
        Float3 normal;//法線ベクトル
        Float2 uv;//uv座標
    };
    //--------------------------------------
    typedef struct Mesh
    {
        std::vector<FbxVertex> vertices;
        std::vector<uint32_t> indices;
        std::wstring DiffuseMap;
    };


    typedef struct ImportSetting
    {
        const std::string fileName;//ファイルパス
        std::vector<Mesh>& meshies; //出力先メッシュ配列
        bool inversU = false; //U座標反転フラグ
        bool inversV = false; //V座標反転フラグ
    };

    struct aiMesh;
    struct aiMaterial;

    class AssimpLoader
    {
    private:
        AssimpLoader() {};
        AssimpLoader(const AssimpLoader& dx12) {};
        AssimpLoader& operator=(const AssimpLoader& dx12) {};
        ~AssimpLoader() {  };
        using string = std::string;
        ID3D12Device* device = nullptr;
        void LoadMesh(Mesh& dst,const aiMesh* src,bool inversU, bool inverV);

    public:
        static const string baseDirectory;
        static void DeleteInstance();
        static AssimpLoader* GetInstance();
        static AssimpLoader* GetInitInstance();
        void Initialize();
        void Finalize();

        bool LoadFile(ImportSetting setting);
    };
}
