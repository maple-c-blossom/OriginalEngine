#pragma once
#include "Dx12.h"
#include <string>
#include <memory>
#include "ObjectMaterial.h"
#include "Texture.h"
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

namespace MCB
{

    //// My own implementation of IOStream
    //class MyIOStream : public Assimp::IOStream {
    //    friend class MyIOSystem;

    //protected:
    //    // Constructor protected for private usage by MyIOSystem
    //    MyIOStream();

    //public:
    //    ~MyIOStream();
    //    size_t Read(void* pvBuffer, size_t pSize, size_t pCount) {  }
    //    size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) {  }
    //    aiReturn Seek(size_t pOffset, aiOrigin pOrigin) {  }
    //    size_t Tell() const {  }
    //    size_t FileSize() const {  }
    //    void Flush() {  }
    //};

    //// Fisher Price - My First Filesystem
    //class MyIOSystem : public Assimp::IOSystem {
    //    MyIOSystem() {  }
    //    ~MyIOSystem() {  }

    //    // Check whether a specific file exists
    //    bool Exists(const std::string& pFile) const  {
    //    };

    //    // Get the path delimiter character we'd like to see
    //    char GetOsSeparator() const  {
    //        return '/';
    //    }

    //    // ... and finally a method to open a custom stream
    //    Assimp::IOStream* Open(const std::string& pFile, const std::string& pMode) {
    //        return new MyIOStream();
    //    }

    //    void Close(Assimp::IOStream* pFile) { delete pFile; }
    //};

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

    //struct aiMesh;
    //struct aiMaterial;

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
        bool MCB::AssimpLoader::DoTheImportThing(const std::string& pFile);
        bool LoadFile(ImportSetting setting);
    };
}
